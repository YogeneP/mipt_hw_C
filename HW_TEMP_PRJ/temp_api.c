#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "temp_api.h"

#define SIZEOF(TLOG) TLOG[0].data.entries_count
#define SEPARATOR "%*[;, \t]"
// #define DEBUG

#ifdef DEBUG 
#define D if(1) 
#else
#define D if(0) 
#endif



//max data string length, e.g.: 2025,11,12,14,23,-1 (19 + \n + \0)
#define BUF_SIZE 50 

char *month_name[13] = {
    [1] = "January",
    [2] = "February",
    [3] = "March",
    [4] = "April",
    [5] = "May",
    [6] = "June",
    [7] = "July",
    [8] = "August",
    [9] = "September",
    [10] = "October",
    [11] = "November",
    [12] = "December"
};

int8_t validate_timedate(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute);
int8_t validate_year(int32_t year);
int8_t validate_month(int32_t month);
int compare_entries(const void * i1, const void * i2 );

int8_t add_entry(Temp_log t_log[], int32_t y, int32_t m, int32_t d, int32_t h, int32_t mn, int32_t t) {
    if (t_log == NULL) return 1;

    // metadata validation (t_log[0].data.entries_count - entries count, the rest - full with bit 1):
    if (t_log[0].hour != 255 ||
        t_log[0].minute != 255 ||
        t_log[0].temperature != -1) 
    return 1; 

    if (validate_timedate(y, m, d, h, mn)) return 2;
    if (t < MIN_TEMP || t > MAX_TEMP) return 4;
    
    t_log[0].data.entries_count++;
    int index = t_log[0].data.entries_count;

    t_log[index].data.date.year = (uint16_t)y;
    t_log[index].data.date.month = (uint8_t)m;
    t_log[index].data.date.day = (uint8_t)d;
    t_log[index].hour = (uint8_t)h;
    t_log[index].minute = (uint8_t)mn;
    t_log[index].temperature = (int8_t)t;
    
    return 0;
}

int8_t delete_entry(Temp_log t_log[], uint32_t index) {
    if(index < 1 || index > t_log[0].data.entries_count) return 1;
    if(index < t_log[0].data.entries_count) memmove(&t_log[index], &t_log[index+1], sizeof(Temp_log)*(t_log[0].data.entries_count-index));
    t_log[0].data.entries_count--;
    return 0;
} 

/*
    returns a count of the faulty lines in data-file
*/
int16_t read_log(Temp_log** t_log, uint32_t* capacity, FILE* data) {

    //metadata initializing
    (*t_log)[0].data.entries_count = 0;
    (*t_log)[0].hour = 255;
    (*t_log)[0].minute = 255;
    (*t_log)[0].temperature = -1;

    int32_t y = 0;
    int32_t m = 0;
    int32_t d = 0;
    int32_t h = 0;
    int32_t mn = 0;
    int32_t t = 0;

    char buf[BUF_SIZE] = {'0'};
    uint32_t i = 1;
    uint32_t err_count = 0;
    while (fgets(buf, BUF_SIZE, data)) {
        if(i >= *capacity) {
            (*capacity) *= 2;
            Temp_log* temp = realloc(*t_log,(*capacity)*sizeof(Temp_log));
            if(temp == NULL) { return -1; }
            *t_log = temp;
        }
        if(sscanf(buf, "%"SCNd32 SEPARATOR "%"SCNd32 SEPARATOR "%"SCNd32 SEPARATOR "%"SCNd32 SEPARATOR "%"SCNd32 SEPARATOR "%"SCNd32, 
                &y, &m, &d, &h, &mn, &t) == 6 ) {
            if(validate_timedate(y, m, d, h, mn) == 0) {
                if(t <= MAX_TEMP && t >= MIN_TEMP) {
                    (*t_log)[i].data.date.year = (uint16_t)y;
                    (*t_log)[i].data.date.month = (uint8_t)m;
                    (*t_log)[i].data.date.day = (uint8_t)d;
                    (*t_log)[i].hour = (uint8_t)h;
                    (*t_log)[i].minute = (uint8_t)mn;
                    (*t_log)[i].temperature = (int8_t)t;
                    i++;
                    continue;
                }
            }
        }
        err_count++;
        D printf("invalid line #%u: %s", i+err_count, buf); 
    }
    (*t_log)[0].data.entries_count = i-1;
    return err_count;
}

void print_log(Temp_log t_log[], uint32_t start, uint32_t length) {
    if(t_log[0].data.entries_count == 0) { 
        printf("No data to display\n");
        return;
    }  
    printf("********************************\n");
    printf("| # |    Date, time    | Temp. |\n");
    printf("********************************\n");
    uint32_t i = start; 
    while (i <= t_log[0].data.entries_count && i <= start+length-1) { //t_log[0].data.entries_count - metadata, entries count
        printf("|%03u| %04u-%02u-%02u %02u:%02u |  %02d  |\n", 
            i, t_log[i].data.date.year, t_log[i].data.date.month, t_log[i].data.date.day, t_log[i].hour, t_log[i].minute, t_log[i].temperature);
        i++;
    }
    printf("*******************************\n");

}

int8_t get_month_average_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    uint32_t count = 0;
    float mid = .0;
    for (uint32_t i = 1; i <= t_log[0].data.entries_count; i++) {
        if (t_log[i].data.date.year == year && t_log[i].data.date.month == month) {
            count++;
            mid = (mid * ((count == 1) ? 1 : count - 1) + (float)t_log[i].temperature)/(float)count;   
        }
    }
    if (count == 0) return INT8_MIN;
    return (int8_t)mid;
}

int8_t get_month_min_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    int8_t min = INT8_MAX;
    for (uint32_t i = 1; i <= t_log[0].data.entries_count; i++) {
        if (t_log[i].data.date.year == year && t_log[i].data.date.month == month) {
            min = t_log[i].temperature < min ? t_log[i].temperature : min;   
        }
    }
    return (min == INT8_MAX ? INT8_MIN : min);
}

int8_t get_month_max_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    int8_t max = INT8_MIN;
    for (uint32_t i = 1; i <= t_log[0].data.entries_count; i++) {
        if (t_log[i].data.date.year == year && t_log[i].data.date.month == month) {
            max = t_log[i].temperature > max ? t_log[i].temperature : max;   
        }
    }
    return max;
}

int print_month_stats(Temp_log* t_log, int32_t year, int32_t month) {
    if(!t_log) return -1;
    if(validate_year(year)) return 1;
    if(validate_month(month)) return 2;

    int8_t mid_t = get_month_average_temp(t_log, (uint16_t)year, (uint8_t)month);
    if (mid_t != INT8_MIN) {
        printf("Year %"PRIu16" %s statistics: \n", year, month_name[month]);
        printf("Average temperature: %"PRId8"\n", mid_t);
        printf("Highest temperature: %"PRId8"\n", get_month_max_temp(t_log,year,month));
        printf("Lowest temperature: %"PRId8"\n", get_month_min_temp(t_log,year,month));
        return 0;
    }
    return 3;
}

int print_year_stats(Temp_log* t_log, int32_t year) {
    if(!t_log) return -1;
    if(validate_year(year)) return 1;

    float mid_t = .0;
    int8_t max_t = INT8_MIN;
    int8_t min_t = INT8_MAX;
    uint8_t count = 0;
    int8_t temp = 0;

    for (uint8_t m = 1; m <= 12; m++) {
        temp = get_month_average_temp(t_log,(uint16_t)year,m);
        if(temp != INT8_MIN) { 
            count++;
            mid_t = (mid_t * ((count == 1) ? 1 : count - 1) + (float)temp)/(float)count;
            temp = get_month_max_temp(t_log, (uint16_t)year, m);
            max_t = temp > max_t ? temp : max_t;
            temp = get_month_min_temp(t_log, (uint16_t)year, m);
            min_t = temp < min_t ? temp : min_t;
        }
    }
    if (count) {
        printf("Year %"PRIu16" statistics: \n", (uint16_t)year);
        printf("Average temperature: %"PRId8"\n", (int8_t)mid_t);
        printf("Lowest temperature: %"PRId8"\n", min_t);
        printf("Highest temperature: %"PRId8"\n", max_t);
        return 0;
    }
    return 3;
}

int print_entry(Temp_log* t_log, uint32_t index) {
    if(!t_log) return -1;
    if(index < 1 || index > t_log[0].data.entries_count) return 1;
    printf("%"PRIu16";%"PRIu8";%"PRIu8";%"PRIu8";%"PRIu8";%"PRId8"\n", 
        t_log[index].data.date.year, t_log[index].data.date.month, t_log[index].data.date.day, t_log[index].hour, t_log[index].minute, t_log[index].temperature);
    return 0;
}

void sort_log (Temp_log* t_log) {
    qsort((void*)(t_log + 1), t_log[0].data.entries_count, sizeof(Temp_log), compare_entries); //t_log[0].data.entries_count - entries count metadata
}

int8_t validate_timedate(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute) {
    if (validate_year(year)) { return 1; }
    if (validate_month(month)) { return 2; }
    if (day < 1) { return 3; }
    if (hour > 23) { return 4; }
    if (minute > 59) { return 5; }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) { return 3; }
    } else {
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                if (day > 29) { return 3; }
            } else {
                if (day > 28) { return 3; }
            }
        } else {
            if (day > 31) { return 3; }
        }
    }
    return 0;
}

int8_t validate_month(int32_t month) {
    if (month < 1 || month > 12) { return 2; }
    return 0;
}

int8_t validate_year(int32_t year) {
    if (year < 1000 || year > 9999) { return 1; }
    return 0;
}

int compare_entries(const void * i1, const void * i2 ) {
    if (((Temp_log*)i1)->data.date.year != ((Temp_log*)i2)->data.date.year) { return ((Temp_log*)i1)->data.date.year - ((Temp_log*)i2)->data.date.year; }
    if (((Temp_log*)i1)->data.date.month != ((Temp_log*)i2)->data.date.month) { return ((Temp_log*)i1)->data.date.month - ((Temp_log*)i2)->data.date.month; }
    if (((Temp_log*)i1)->data.date.day != ((Temp_log*)i2)->data.date.day) { return ((Temp_log*)i1)->data.date.day - ((Temp_log*)i2)->data.date.day; }
    if (((Temp_log*)i1)->hour != ((Temp_log*)i2)->hour) { return ((Temp_log*)i1)->hour - ((Temp_log*)i2)->hour; }
    return ((Temp_log*)i1)->minute - ((Temp_log*)i2)->minute;
}
