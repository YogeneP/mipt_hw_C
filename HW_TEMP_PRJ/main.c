#include <stdio.h>
#include "temp_api.h"

Temp_log temp_log[LOG_LEN];

int main() {
    create_log(temp_log);
    int res = 0;
    if ((res = add_item(temp_log, 2025, 12, 14, 9, 0, -4)) == 0) {
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
    print_log(temp_log);
    printf("Average temp in March 2025: %d *C\n", get_month_average_temp(temp_log, 2025, 3));
    printf("Maximal temp in March 2025: %d *C\n", get_month_max_temp(temp_log, 2025, 3));
    printf("Minimal temp in March 2025: %d *C\n", get_month_min_temp(temp_log, 2025, 3));

    delete_item(temp_log, 1);

    printf("\nDeleted first record:\n");
    print_log(temp_log);

    sort_log(temp_log);
    printf("\nLog sorted by date/time:\n");
    print_log(temp_log);
    return 0;
}

