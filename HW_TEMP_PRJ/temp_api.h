#include <inttypes.h>

#ifndef TEMP_API_H
#define TEMP_API_H

struct temp_log {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    int8_t temperature;    
};

uint8_t get_month_average_temp(struct temp_log* log, uint8_t year, uint8_t month);
uint8_t get_month_min_temp(struct temp_log* log, uint8_t year, uint8_t month);
uint8_t get_month_max_temp(struct temp_log* log, uint8_t year, uint8_t month);
void print_year_stats(struct temp_log* log, uint8_t year);

#endif