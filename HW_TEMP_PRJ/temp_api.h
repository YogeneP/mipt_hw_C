#include <inttypes.h>

#ifndef TEMP_API_H
#define TEMP_API_H

#define LOG_LEN 1000

typedef struct Temp_log { // Temp_log[0].year is reserved for current database length
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    int8_t temperature;    
} Temp_log;


int8_t add_entry(Temp_log log[], uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minutes, int8_t t);
int8_t delete_entry(Temp_log t_log[], uint16_t index);
uint16_t create_log(Temp_log t_log[], FILE* datafile);
void print_log(Temp_log t_log[]);
void sort_log(Temp_log t_log[]);
int8_t get_month_average_temp(Temp_log *log, uint16_t year, uint8_t month);
int8_t get_month_min_temp(Temp_log* log, uint16_t year, uint8_t month);
int8_t get_month_max_temp(Temp_log* log, uint16_t year, uint8_t month);
void print_year_stats(Temp_log* log, uint16_t year);

#endif