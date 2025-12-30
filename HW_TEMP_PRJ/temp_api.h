#include <inttypes.h>

#ifndef TEMP_API_H
#define TEMP_API_H

#define MIN_TEMP (-99)
#define MAX_TEMP (99)

typedef struct Temp_log { // Temp_log[0].year is reserved for current database length
    union {
        uint32_t entries_count;
        struct {
            uint16_t year;
            uint8_t month;
            uint8_t day; 
        } date;
    } data;
    uint8_t hour;
    uint8_t minute;
    int8_t temperature;    
} Temp_log;

extern char *month_name[13];

int8_t add_entry(Temp_log log[], int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minutes, int32_t t);
int8_t delete_entry(Temp_log t_log[], uint32_t index);
int16_t read_log(Temp_log** t_log, uint32_t* capacity, FILE* data);
void print_log(Temp_log t_log[], uint32_t start, uint32_t length);
void sort_log(Temp_log t_log[]);
int8_t get_month_average_temp(Temp_log *log, uint16_t year, uint8_t month);
int8_t get_month_min_temp(Temp_log* log, uint16_t year, uint8_t month);
int8_t get_month_max_temp(Temp_log* log, uint16_t year, uint8_t month);
int print_year_stats(Temp_log* log, int32_t year);
int print_month_stats(Temp_log* t_log, int32_t year, int32_t month);
int print_entry(Temp_log* t_log, uint32_t index);

#endif