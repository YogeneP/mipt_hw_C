#include <stdio.h>
#include "temp_api.h"

Temp_log temp_log[LOG_LEN];

int main(int argc, char** argv) {
    FILE* csv = NULL;
    uint8_t stat_month = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch ((argv[i])[1]) {
                case 'm':
                    sscanf( argv[i+1], "%"SCNu8, &stat_month);
                    i++;
                    continue;
                case 'f':
                    csv = fopen(argv[i+1], "rt");
                    i++;
                    continue;
            }
        };
        break;
    }
    if(csv == NULL) {
        printf("Data file is not specified or missing\n");
        printf("$ Help message $\n");
        return 0;
    }
    if(stat_month == 0) {
        printf("$ Help message $");
        fclose(csv);
        return 0;
    }
    create_log(temp_log, csv);
    print_log(temp_log);

    int res = 0;
    if ((res = add_entry(temp_log, 2025, 12, 14, 9, 0, -4)) == 0) {
    } else { 
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
        }
    };
    fclose(csv);

    print_log(temp_log);
    printf("Average temp in March 2025: %d *C\n", get_month_average_temp(temp_log, 2025, 3));
    printf("Maximal temp in March 2025: %d *C\n", get_month_max_temp(temp_log, 2025, 3));
    printf("Minimal temp in March 2025: %d *C\n", get_month_min_temp(temp_log, 2025, 3));

    delete_entry(temp_log, 1);

    printf("\nDeleted first record:\n");
    print_log(temp_log);

    sort_log(temp_log);
    printf("\nLog sorted by date/time:\n");
    print_log(temp_log);

    return 0;
}

