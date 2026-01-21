#include <stdio.h>
#include <inttypes.h>

int main(void) {
	uint32_t n;
	int32_t p;
	int32_t max = INT32_MIN;
	uint32_t max_count = 0;
	scanf("%" SCNu32, &n);
	for (int i = 0; i < n; i++) {
		scanf("%" SCNu32, &p);
		if (p == max) {
			max_count++;
			continue;
		}
		if (p > max) {
			max = p;
			max_count = 1;
		}
	}
	printf("%" PRIu32 "\n", max_count);
    
}
