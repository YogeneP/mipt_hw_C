#include <stdio.h>
#include <inttypes.h>

#define BITS 32

int main(void) {
	uint32_t n;
	uint8_t k;
	uint32_t max = 0;
	scanf("%" SCNu32 "%" SCNu8, &n, &k);
	if (k < 1 || k > 31) { return -1; }
	uint32_t c;
	for(int i = 0; i <= BITS - k; i++) {
		c = (n >> i) & ((1 << k) - 1);
		if ( c > max ) {
			max = c;
		}
//		printf("%" PRIu8 " %" PRIu32 " %" PRIu32 "\n", i, c, max);
	}
	printf ("%" PRIu32, max);	
}
