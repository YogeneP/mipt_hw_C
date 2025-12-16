#include <stdio.h>
#include "temp_api.h"

Temp_log temp_log[LOG_LEN];
uint16_t items_count = 0;

int main() {
    items_count = create_log(temp_log);
    add_item(temp_log, items_count++, 2025, 12, 14, 9, 0, -4);
    print_log(temp_log, items_count);
    return 0;
}

