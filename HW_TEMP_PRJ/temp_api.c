#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "temp_api.h"

#define SIZEOF(TLOG) TLOG[0].year

int8_t validate_timedate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
int compare_items(const void * i1, const void * i2 );

int8_t add_item(Temp_log t_log[], uint16_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t mn, int8_t t) {
    if (t_log == NULL) return 1;

    // metadata validation (t_log[0].year - items count, the rest - full with bit 1):
    if (t_log[0].month != 255 || 
        t_log[0].day != 255 ||
        t_log[0].hour != 255 ||
        t_log[0].minute != 255 ||
        t_log[0].temperature != -1) 
    return 1; 

    if (t_log[0].year >= LOG_LEN-1) return 3;
    if (validate_timedate(y, m, d, h, mn)) return 2;
    
    t_log[0].year++;
    int index = t_log[0].year;

    t_log[index].year = y;
    t_log[index].month = m;
    t_log[index].day = d;
    t_log[index].hour = h;
    t_log[index].minute = mn;
    t_log[index].temperature = t;
    
    return 0;
}

int8_t delete_item(Temp_log t_log[], uint16_t index) {
    if(index < 1 || index > t_log[0].year) return 1;
    memmove(&t_log[index], &t_log[index+1], sizeof(Temp_log)*(t_log[0].year-(index+1)));
    t_log[0].year--;
    return 0;
} 

uint16_t create_log(Temp_log t_log[]) {
    //metadata initializing
    t_log[0].year = 0;
    t_log[0].month = 255;
    t_log[0].day = 255;
    t_log[0].hour = 255;
    t_log[0].minute = 255;
    t_log[0].temperature = -1;

    uint16_t i = 0;
    if (add_item(t_log, 2024, 12, 3, 9, 0, -5) == 0) i++;
    if (add_item(t_log, 2025, 1, 1, 8, 0, -10) == 0) i++;
    if (add_item(t_log, 2025, 2, 10, 8, 0, -15) == 0) i++;
    if (add_item(t_log, 2025, 3, 5, 8, 30, -2) == 0) i++;
    if (add_item(t_log, 2025, 4, 2, 7, 50, 5) == 0) i++;
    if (add_item(t_log, 2025, 3, 6, 8, 0, 5) == 0) i++;
    if (add_item(t_log, 2025, 5, 1, 8, 10, 10) == 0) i++;
    if (add_item(t_log, 2025, 6, 1, 8, 0, 13) == 0) i++;
    if (add_item(t_log, 2025, 3, 10, 9, 0, 3) == 0) i++;
    if (add_item(t_log, 2025, 7, 1, 8, 0, 13) == 0) i++;
    if (add_item(t_log, 2025, 3, 12, 8, 20, 7) == 0) i++;
    if (add_item(t_log, 2025, 8, 1, 8, 0, 16) == 0) i++;
    if (add_item(t_log, 2025, 9, 1, 8, 0, 13) == 0) i++;
    return i;
}

void print_log(Temp_log t_log[]) {
    printf("****************************\n");
    printf("|    Date, time    | Temp. |\n");
    printf("****************************\n");
    for (uint16_t i = 1; i < t_log[0].year; i++) { //t_log[0].year - metadata, items count
        printf("| %04u-%02u-%02u %02u:%02u |  %02d  |\n", 
            t_log[i].year, t_log[i].month, t_log[i].day, t_log[i].hour, t_log[i].minute, t_log[i].temperature);
    }
    printf("***************************\n");

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
    if (count == 0) return -128;
    return (int8_t)mid;
}

int8_t get_month_min_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    int8_t min = INT8_MAX;
    for (uint16_t i = 1; i < t_log[0].year; i++) {
        if (t_log[i].year == year && t_log[i].month == month) {
            min = t_log[i].temperature < min ? t_log[i].temperature : min;   
        }
    }
    return (min == 127 ? -128 : min);
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

void print_year_stats(Temp_log* t_log, uint16_t year) {
    printf("Stats will be here");
}

void sort_log (Temp_log* t_log) {
    qsort((void*)(t_log + 1), t_log[0].year, sizeof(Temp_log), compare_items);
}

int8_t validate_timedate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute) {
    if (year < 1000 || year > 9999) { return 1; }
    if (month > 12 || month < 1) { return 2; }
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

int compare_items(const void * i1, const void * i2 ) {
    if (((Temp_log*)i1)->year != ((Temp_log*)i2)->year) { return ((Temp_log*)i1)->year - ((Temp_log*)i2)->year; }
    if (((Temp_log*)i1)->month != ((Temp_log*)i2)->month) { return ((Temp_log*)i1)->month - ((Temp_log*)i2)->month; }
    if (((Temp_log*)i1)->day != ((Temp_log*)i2)->day) { return ((Temp_log*)i1)->day - ((Temp_log*)i2)->day; }
    if (((Temp_log*)i1)->hour != ((Temp_log*)i2)->hour) { return ((Temp_log*)i1)->hour - ((Temp_log*)i2)->hour; }
    return ((Temp_log*)i1)->minute - ((Temp_log*)i2)->minute;
}
