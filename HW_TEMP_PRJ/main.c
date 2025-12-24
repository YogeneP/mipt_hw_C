#include <stdio.h>
#include <time.h>
#include "temp_api.h"

#define ENTRY_FIELDS_COUNT 6
#define CURR_YEAR t_struct->tm_year + 1900;
#define CURR_MONTH t_struct->tm_mon+1;
#define CURR_DAY t_struct->tm_mday;
#define CURR_HOUR t_struct->tm_hour;
#define CURR_MINUTE t_struct->tm_min;

#define DEBUG

#ifdef DEBUG 
#define D if(1) 
#else
#define D if(0) 
#endif

Temp_log temp_log[LOG_LEN];

void print_help(void);

int main(int argc, char** argv) {
    FILE* csv = NULL;
    char zero = '\0';
    char* filename = &zero;

    uint8_t sort_needed = 0;

    int32_t raw_data[ENTRY_FIELDS_COUNT] = { 0 };
    raw_data[5] = MAX_TEMP + 1;
    int32_t raw_index_del = 0;

    time_t now = time(NULL);
    struct tm *t_struct = localtime(&now);

    int32_t stat_month = 0;
    int32_t stat_year = 0;

    int res = 0;
    int i = 0;

// ***** Args and parameters parsing ******
    while(i < argc-1) {
        i++;
        if (argv[i][0] == '-') {
            D printf("Argument processing: %s\n", argv[i]);
            switch ((argv[i])[1]) {
                case 'f':   // essential argument + parameter
                    i++;
                    if (i < argc) {
                        filename = argv[i];
                        continue;
                    }
                    break;
                case 'm':   // statistic month: optional argument, optional parameter
                    stat_month = 255;
                    if (i+1 < argc) {                      
                        if ((sscanf(argv[i+1], "%"SCNd32, &stat_month)) == 1) {
                            i++;
                        }
                        continue;
                    }
                    break;
                case 'y':   // statistics year: optional argument, optional parameter
                    stat_year = 1;
                    if (i+1 < argc) {
                        if ((sscanf(argv[i+1], "%"SCNd32, &stat_year)) == 1) {
                            i++;
                        }
                        continue;
                    }
                    break;
                case 'a': // add entry: optional argument, essential ENTRY_FIELDS_COUNT parameters
                    D printf("-a - processing\n");
                    i++;
                    D printf("i = %u; argc-ENTRY_FIELDS_COUNT = %u\n", i, argc-ENTRY_FIELDS_COUNT);    
                    int j = 0;
                    for (j = 0; j < ENTRY_FIELDS_COUNT; j++) {
                        if(i+j > argc || sscanf(argv[i+j], "%"SCNd32, &raw_data[j]) != 1) {
                            D printf("Parameter %u(%u): %s read error\n",j,j+i,argv[i+j]);
                            if(j == 1) { // special condition: if the first parameter read successfully, trying to assume it as temperature
                                D printf("But j = %u, checking value: %d....",j,raw_data[0]);
                                if(raw_data[0] >= MIN_TEMP && raw_data[0] <= MAX_TEMP ) {
                                    D printf("looks like a temperature, processing\n");
                                    raw_data[5] = raw_data[0];
                                    raw_data[0] = CURR_YEAR;
                                    raw_data[1] = CURR_MONTH;
                                    raw_data[2] = CURR_DAY;
                                    raw_data[3] = CURR_HOUR;
                                    raw_data[4] = CURR_MINUTE;
                                    break; 
                                }
                            }
                            raw_data[0] = 1;
                            break;
                        }
                        D printf("Read: arg[%d]: \"%s\"; data = %"PRId32"\n", i+j, argv[i+j], raw_data[j]);
                    }
                    if (j == ENTRY_FIELDS_COUNT) {
                        i += j - 1;
                        continue;
                    }
                    if (j == 1 && raw_data[5] < MAX_TEMP) {
                        D printf("raw_data[] filled with current time/date, keep parsing\n"); 
                        continue;
                    }
                    raw_data[0] = 1;
                    break;
                case 'd': //optional argument, essential parameter
                    i++;
                    if(i < argc) {
                        if((sscanf(argv[i], "%"SCNd32, &raw_index_del) != 1)) {
                            raw_index_del = -1;
                            break;                        
                        };
                    }
                    continue;
                case 's': //optional argument, no parameter
                    sort_needed = 1;
                    continue;
                default:
                    print_help();
                    return 0;
            }
        } 
        break;
    }

    if(*filename == '\0') {
        printf("Data file is not specified, exiting");
        print_help();
        return -1;
    }
    csv = fopen(filename, "r");
    if (csv == NULL) {
        printf("Data file %s is missing, will be created if -a specified.\n", filename);
    }

    printf("Reading data from \"%s\"...", filename); 
    uint16_t err_count = read_log(temp_log, csv);
    fclose(csv);
    if(temp_log[0].year) printf(" done:\n\t%"PRIu16" entries read, %"PRIu16" invalid lines ignored\n", temp_log[0].year, err_count); 
    else printf(" no data found.\n");

    // ******* Database memory update section *******
    if (raw_data[0]) {
        if (raw_data[0] == 1) {
            printf("Failed to add an entry: invalid input, run -h for help\n");
            return 1;
        } else {
            if ((res = add_entry(temp_log, raw_data[0], raw_data[1], raw_data[2], raw_data[3], raw_data[4], raw_data[5]))) { 
                printf("Failed to create a record: ");
                switch (res) {
                    case 2:
                        printf("invalid date/time data!\n");
                        break;
                    case 3:
                        printf("data base capacity exceeded!\n");
                        break;   
                    case 4: 
                        printf("temperature out of range (%d*C...%d*C expected)\n", MIN_TEMP, MAX_TEMP);
                        break;      
                };
                return 1;
            }
            printf("Entry added: ");
            print_entry(temp_log, temp_log[0].year);
        }
    }
    if(raw_index_del > 0) {
        if ((res = delete_entry(temp_log,raw_index_del))) {
            printf("Unable to delete an entry: index out of range\n");
            printf("All changes cancelled if any. Data file is not modified");
            return 2;
        }
        printf("Entry #%"PRIu16" deleted\n", raw_index_del);
    } 

    if (sort_needed) {
        sort_log(temp_log);
        printf("Database arranged by data/time ascending\n");
    }

// ****** Write file *******
    if(raw_index_del || sort_needed) { // if delete or sort performed - rewrite the file
        if ((csv = fopen(filename, "w")) == NULL) {
            printf("File write error. Exiting...");
            return 3;
        }
        for (uint16_t i = 1; i <= temp_log[0].year; i++) {
            fprintf(csv, "%"SCNu16",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNd8,
                temp_log[i].year, temp_log[i].month, temp_log[i].day, temp_log[i].hour, temp_log[i].minute, temp_log[i].temperature);
            if (i < temp_log[0].year) fprintf(csv, "\n");
        }
        fclose(csv);
    } else { // if only add entry triggered - just write a new entry to the end
        if (raw_data[0] > 999) {
            if ((csv = fopen(filename, "a")) == NULL) {
                printf("File write error. Exiting...");
                return 3;
            }
            fprintf(csv, "\n%"SCNu16",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNd8,
                (uint16_t)raw_data[0], (uint8_t)raw_data[1], (uint8_t)raw_data[2], (uint8_t)raw_data[3], 
                (uint8_t)raw_data[4], (int8_t)raw_data[5]);
            fclose(csv);
        }
    }

// ******* Display stats *******
    if (stat_month == 0) {
        if (stat_year == 0) {
            print_log(temp_log);
            return 0;
        }
        if(stat_year == 1) { 
            stat_year = CURR_YEAR; 
        }
        if((res = print_year_stats(temp_log,stat_year))) {
            switch (res) {
                case 1:
                    printf("Invalid year value");
                    break;
                case 3:
                    printf("No data for year %"PRIu16, stat_year);
                    break;
            }
        } 
        return 0;
    }

    if (stat_year == 0) { //if no YYYY specified assume current year
        stat_year = CURR_YEAR;
    }
    if (stat_month == 255) { //if no YYYY specified assume current year
        stat_month = CURR_MONTH;
    }

    if ((res = print_month_stats(temp_log, stat_year, stat_month))) {
        switch (res) {
            case 1:
            case 2:
                printf("Invalid year/month value");
                break;
            case 3:
                printf("No data for %"PRIu16", %s", stat_year, month_name[stat_month]);
                break;
        }
        return 0;
    }
    return 0;
}

void print_help(void) {
    printf("CLI temperature log processing usage:\n");
    printf("\t-f <filename>: temperature database CSV-file (no spaces); entry format: <YYYY>,<MM>,<DD>,<HH>,<MM>,<temperature>\n");
    printf("\t\t If the file doesn't exist it will be created if data entry added with -a");
    printf("Statistics:\n");
    printf("\t-y <YYYY>: print statistics for year YYYY. Current year is assumed if a value not specified.\n");
    printf("\t-m <MM>: print statistics for month MM. Current year is assumed if -y <year> not specified.\n");
    printf("If no -m and -y specified the full database will be printed.\n");
    printf("\nDatabase management:\n");
    printf("\t-a <YYYY>,<MM>,<DD>,<HH>,<mm>,<temperature>: add record to the database.\n");
    printf("\t-a <temperature>: add record to the database with current date/time.\n");
    printf("\t-d <index>: delete record from database by index (#).\n");
    printf("\t-s: sort database by date/time, ascending.\n\n");
}