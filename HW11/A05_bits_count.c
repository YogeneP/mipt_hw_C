#include <stdio.h>
#include <inttypes.h>

uint8_t count_bits(uint32_t bin) {
	uint8_t count;
	for(int i = 0; i < 32; i++) {
		if ((bin >> i) & 1) count++;
	}
	return count;
}

int main(void) {
	uint32_t n = 0;

    scanf("%"SCNu32, &n);
    
	printf("%" PRIu8, count_bits(n));
}
