#include <stdio.h>
#include <inttypes.h>

int main(void) {
	uint32_t n;
	uint8_t k;
	scanf("%" SCNu32 "%" SCNu8, &n, &k);
	if (k < 1 || k > 31) {
		printf("Invalid input");
		return 1;
	}
	printf("%" PRIu32 "\n", n & ((1 << k)-1));
    return 0;
}
