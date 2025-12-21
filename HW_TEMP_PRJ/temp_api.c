#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "temp_api.h"

#define SIZEOF(TLOG) TLOG[0].year

//max data string length, e.g.: 2025,11,12,14,23,-1 (19 + \n + \0):
#define BUF_SIZE 21 

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

    // metadata validation (t_log[0].year - entries count, the rest - full with bit 1):
    if (t_log[0].month != 255 || 
        t_log[0].day != 255 ||
        t_log[0].hour != 255 ||
        t_log[0].minute != 255 ||
        t_log[0].temperature != -1) 
    return 1; 

    if (t_log[0].year >= LOG_LEN-1) return 3;
    if (validate_timedate(y, m, d, h, mn)) return 2;
    if (t < -99 || t > 99) return 4;
    
    t_log[0].year++;
    int index = t_log[0].year;

    t_log[index].year = (uint16_t)y;
    t_log[index].month = (uint8_t)m;
    t_log[index].day = (uint8_t)d;
    t_log[index].hour = (uint8_t)h;
    t_log[index].minute = (uint8_t)mn;
    t_log[index].temperature = (int8_t)t;
    
    return 0;
}

int8_t delete_entry(Temp_log t_log[], uint16_t index) {
    if(index < 1 || index > t_log[0].year) return 1;
    if(index < t_log[0].year) memmove(&t_log[index], &t_log[index+1], sizeof(Temp_log)*(t_log[0].year-(index+1)));
    t_log[0].year--;
    return 0;
} 

uint16_t create_log(Temp_log t_log[], FILE* data) {
    //metadata initializing
    t_log[0].year = 0;
    t_log[0].month = 255;
    t_log[0].day = 255;
    t_log[0].hour = 255;
    t_log[0].minute = 255;
    t_log[0].temperature = -1;

    int32_t y = 0;
    int32_t m = 0;
    int32_t d = 0;
    int32_t h = 0;
    int32_t mn = 0;
    int32_t t = 0;

    char buf[BUF_SIZE] = {'0'};
    uint16_t i = 1;
    uint16_t err_count = 0;
    while (fgets(buf, BUF_SIZE, data)) {
        if(sscanf(buf, "%"SCNd32",%"SCNd32",%"SCNd32",%"SCNd32",%"SCNd32",%"SCNd32, 
                &y, &m, &d, &h, &mn, &t) == 6 ) {
            if(validate_timedate(y, m, d, h, mn) == 0) {
                if(t < 100 && t > -100) {
                    if(i < LOG_LEN-1) {
                        t_log[i].year = (uint16_t)y;
                        t_log[i].month = (uint8_t)m;
                        t_log[i].day = (uint8_t)d;
                        t_log[i].hour = (uint8_t)h;
                        t_log[i].minute = (uint8_t)mn;
                        t_log[i].temperature = (int8_t)t;
                        i++;
                        continue;
                    }
                }
            }
        }
        err_count++;
    }
    t_log[0].year = i-1;
    return err_count;
}

void print_log(Temp_log t_log[]) {
    printf("********************************\n");
    printf("| # |    Date, time    | Temp. |\n");
    printf("********************************\n");
    for (uint16_t i = 1; i <= t_log[0].year; i++) { //t_log[0].year - metadata, entries count
        printf("|%03u| %04u-%02u-%02u %02u:%02u |  %02d  |\n", 
            i, t_log[i].year, t_log[i].month, t_log[i].day, t_log[i].hour, t_log[i].minute, t_log[i].temperature);
    }
    printf("*******************************\n");

}

int8_t get_month_average_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    uint16_t count = 0;
    float mid = .0;
    for (uint16_t i = 1; i < t_log[0].year; i++) {
        if (t_log[i].year == year && t_log[i].month == month) {
            count++;
            mid = (mid * ((count == 1) ? 1 : count - 1) + (float)t_log[i].temperature)/(float)count;   
        }
    }
    if (count == 0) return INT8_MIN;
    return (int8_t)mid;
}

int8_t get_month_min_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    int8_t min = INT8_MAX;
    for (uint16_t i = 1; i < t_log[0].year; i++) {
        if (t_log[i].year == year && t_log[i].month == month) {
            min = t_log[i].temperature < min ? t_log[i].temperature : min;   
        }
    }
    return (min == INT8_MAX ? INT8_MIN : min);
}

int8_t get_month_max_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    int8_t max = INT8_MIN;
    for (uint16_t i = 1; i < t_log[0].year; i++) {
        if (t_log[i].year == year && t_log[i].month == month) {
            max = t_log[i].temperature > max ? t_log[i].temperature : max;   
        }
    }
    return max;
}

int print_month_stats(Temp_log* t_log, uint16_t year, uint8_t month) {
    if(!t_log) return -1;
    if(validate_year(year)) return 1;
    if(validate_month(month)) return 2;

    int8_t mid_t = get_month_average_temp(t_log,year,month);
    if (mid_t != INT8_MIN) {
        printf("Year %"PRIu16" %s statistics: \n", year, month_name[month]);
        printf("Average temperature: %"PRId8"\n", mid_t);
        printf("Highest temperature: %"PRId8"\n", get_month_max_temp(t_log,year,month));
        printf("Lowest temperature: %"PRId8"\n", get_month_min_temp(t_log,year,month));
        return 0;
    }
    return 3;
}

int print_year_stats(Temp_log* t_log, uint16_t year) {
    if(!t_log) return -1;
    if(validate_year(year)) return 1;

    float mid_t = .0;
    int8_t max_t = INT8_MIN;
    int8_t min_t = INT8_MAX;
    uint8_t count = 0;
    int res = 0;

    for (int m = 1; m <= 12; m++) {
        res = get_month_average_temp(t_log,year,m);
        if(res != INT8_MIN) { 
            count++;
            mid_t = (mid_t * ((count == 1) ? 1 : count - 1) + (float)res)/(float)count;
            res = get_month_max_temp(t_log,year,m);
            max_t = res > max_t ? res : max_t;
            res = get_month_min_temp(t_log, year, m);
            min_t = res < min_t ? res : min_t;
        }
    }
    if (count) {
        printf("Year %"PRIu16" statistics: \n", year);
        printf("Average temperature: %"PRId8"\n", (int8_t)mid_t);
        printf("Lowest temperature: %"PRId8"\n", min_t);
        printf("Highest temperature: %"PRId8"\n", max_t);
        return 0;
    }
    return 3;
}

void sort_log (Temp_log* t_log) {
    qsort((void*)(t_log + 1), t_log[0].year, sizeof(Temp_log), compare_entries); //t_log[0].year - entries count metadata
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
    if (((Temp_log*)i1)->year != ((Temp_log*)i2)->year) { return ((Temp_log*)i1)->year - ((Temp_log*)i2)->year; }
    if (((Temp_log*)i1)->month != ((Temp_log*)i2)->month) { return ((Temp_log*)i1)->month - ((Temp_log*)i2)->month; }
    if (((Temp_log*)i1)->day != ((Temp_log*)i2)->day) { return ((Temp_log*)i1)->day - ((Temp_log*)i2)->day; }
    if (((Temp_log*)i1)->hour != ((Temp_log*)i2)->hour) { return ((Temp_log*)i1)->hour - ((Temp_log*)i2)->hour; }
    return ((Temp_log*)i1)->minute - ((Temp_log*)i2)->minute;
}
