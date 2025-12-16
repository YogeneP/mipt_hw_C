#include <stdio.h>
#include "temp_api.h"

int8_t validate_timedate(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);

int8_t add_item(Temp_log t_log[], uint16_t index, uint16_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t mn, int8_t t) {
    if (t_log == NULL) return 1;
    if (index > LOG_LEN) return 3; 
    if (validate_timedate(y, m, d, h, mn)) return 2;
    t_log[index].year = y;
    t_log[index].month = m;
    t_log[index].day = d;
    t_log[index].hour = h;
    t_log[index].minute = mn;
    t_log[index].temperature = t;
    return 0;
}

uint16_t create_log(Temp_log t_log[]) {
    uint16_t i = 0;
    add_item(t_log, i++, 2024, 12, 3, 9, 0, -5);
    add_item(t_log, i++, 2025, 1, 1, 8, 0, -10);
    add_item(t_log, i++, 2025, 2, 10, 8, 0, -15);
    add_item(t_log, i++, 2025, 3, 5, 8, 30, -2);
    add_item(t_log, i++, 2025, 4, 2, 7, 50, 5);
    add_item(t_log, i++, 2025, 5, 1, 8, 10, 10);
    add_item(t_log, i++, 2025, 6, 1, 8, 0, 13);
    add_item(t_log, i++, 2025, 7, 1, 8, 0, 13);
    add_item(t_log, i++, 2025, 8, 1, 8, 0, 16);
    add_item(t_log, i++, 2025, 9, 1, 8, 0, 13);
    return i;
}

void print_log(Temp_log t_log[], uint16_t count) {
    printf("**************************");
    printf("|   Date, time   | Temp. |");
    printf("**************************");
    for (int i = 0; i < count; i++) {
        printf("| %04ud-%02ud-%02ud %02ud:%02ud |  %02d  |", 
            t_log[i].year, t_log[i].month, t_log[i].day, t_log[i].hour, t_log[i].minute, t_log[i].temperature);
    }
    printf("**************************");

}

uint8_t get_month_average_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    return 0;
}

uint8_t get_month_min_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    return 0;
}

uint8_t get_month_max_temp(Temp_log* t_log, uint16_t year, uint8_t month) {
    return 0;
}

void print_year_stats(Temp_log* t_log, uint16_t year) {
    printf("Stats will be here");
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
