#include <stdio.h>
#include <time.h>
#include "temp_api.h"

Temp_log temp_log[LOG_LEN];

void print_help(void);

int main(int argc, char** argv) {
    FILE* csv = NULL;
    char* filename;
    uint8_t sort_needed = 0;

    Temp_log temp_add = { .year = 0 };
    uint16_t index_del = 0;

    uint8_t stat_month = 0;
    uint16_t stat_year = 0;

    int res = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
//            printf("Argument processing: %s\n", argv[i]);
            switch ((argv[i])[1]) {
                case 'f':
                    filename = argv[i+1];
                    i++;
                    continue;
                case 'm':
                    if ((sscanf(argv[i+1], "%"SCNu8, &stat_month)) == 1) {
                        i++;
                        continue;
                    }
                    break;
                case 'y':
                    if((sscanf(argv[i+1], "%"SCNu16, &stat_year) == 1)) {
                        i++;
                        continue;
                    }
                    break;
                case 'a':
                    res = 0;
                    res += sscanf(argv[++i], "%"SCNu16, &temp_add.year);
                    res += sscanf(argv[++i], "%"SCNu8, &temp_add.month);
                    res += sscanf(argv[++i], "%"SCNu8, &temp_add.day);
                    res += sscanf(argv[++i], "%"SCNu8, &temp_add.hour);
                    res += sscanf(argv[++i], "%"SCNu8, &temp_add.minute);
                    res += sscanf(argv[++i], "%"SCNd8, &temp_add.temperature);
//                    printf("Read: arg[%d]: \"%s\"; temp_add.temperature = %"PRId8": res = %d\n", i, argv[i], temp_add.temperature, res);
                    if( res != 6 ) {       
                        temp_add.year = 1;
                        break;
                    }
                    continue;
                case 's':
                    sort_needed = 1;
                    continue;
                case 'd':
                    if((sscanf(argv[i+1], "%"SCNu16, &index_del) == 1)) {
                        i++;
                        continue;                        
                    };
                    break;
                default:
                    print_help();
                    return 0;
            }
        };
        break;
    }
    csv = fopen(filename, "r");
    if(csv == NULL) {
        printf("Data file is not specified or missing, exiting.\n");
        print_help();
        return 0;
    }
    printf("Reading data from \"%s\"...", filename); 
    uint16_t err_count = create_log(temp_log, csv);
    fclose(csv);
    if(temp_log[0].year == 0) {
        printf(" no data found, exiting\n");
        return 0;
    }
    printf(" done:\n %"PRIu16" records read, %"PRIu16" invalid lines ignored\n", temp_log[0].year, err_count); 

    if (temp_add.year) {
        if (temp_add.year == 1) {
            printf("Failed to add: invalid data format\n");
        } else {
            if ((res = add_entry(temp_log, temp_add.year, temp_add.month, temp_add.day, temp_add.hour, temp_add.minute, temp_add.temperature))) { 
                printf("Failed to create a record: ");
                switch (res) {
                    case 1:
                        printf("invalid database reference!\n");
                        break;
                    case 2:
                        printf("invalid date/time data!\n");
                        break;
                    case 3:
                        printf("data base capacity exceeded!\n");
                        break;   
                    case 4: 
                        printf("temperature oute of range (-99*C...99*C expected)\n");
                        break;      
                };
            }
            if (!sort_needed) {
                csv = fopen(filename,"a");
                fprintf(csv, "\n%"SCNu16",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNd8,
                    temp_add.year, temp_add.month, temp_add.day, temp_add.hour, temp_add.minute, temp_add.temperature);
                fclose(csv);
            } 
        }
    }
    if(index_del) {
        if ((res = delete_entry(temp_log,index_del))) {
            printf("Unable to delete an entry: index out of range");
            index_del = 0;
        }
    }

    if (sort_needed) {
        sort_log(temp_log);
    }

    if(index_del || sort_needed) {
        csv = fopen(filename, "w+");
        for (uint16_t i = 1; i <= temp_log[0].year; i++) {
            fprintf(csv, "%"SCNu16",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNu8",%"SCNd8,
                temp_log[i].year, temp_log[i].month, temp_log[i].day, temp_log[i].hour, temp_log[i].minute, temp_log[i].temperature);
            if (i < temp_log[0].year) fprintf(csv, "\n");
        }
        fclose(csv);
    }

    if (stat_month == 0) {
        if (stat_year == 0) {
           print_log(temp_log);
            return 0;
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
        time_t now = time(NULL);         
        struct tm *t = localtime(&now);  
        stat_year = (t->tm_year) + 1900;
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
    printf("-f <filename>: temperature database CSV-file (no spaces); entry format: <YYYY>,<MM>,<DD>,<HH>,<MM>,<temperature>\n");
    printf("-y <YYYY>: print statistics for year YYYY.\n");
    printf("-m <MM>: print statistics for month MM. Current year is assumed if not specified.\n");
    printf("If no -m specified the yearly stats will be printed.\n");
    printf("If no -m and -y specified the full database will be printed.\n");
    printf("\n-a <YYYY>,<MM>,<DD>,<HH>,<MM>,<temperature>: add record to the database.\n");
    printf("-d <index>: delete record from database by index (#).\n");
    printf("-s: sort database by date/time, ascending.\n");
}