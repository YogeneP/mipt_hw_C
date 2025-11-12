#include <stdio.h>
#include <inttypes.h>

uint64_t convert10(uint32_t n, uint8_t b);

int main() {
	uint32_t num;
	uint8_t base;
	scanf("%" SCNu32 "%" SCNu8, &num, &base);
	printf( "%" PRIu64 "\n", convert10(num, base) );
}

uint64_t convert10(uint32_t n, uint8_t b) {
	// THE AIM: not to save all the digits separately to asseble the number from the reversed digits sequence. 
	// The function produces decimal numer looking in a n-base way, applicable for bases up to 10 

	uint64_t o = 1;
	uint64_t c = 0;
	while (n >= b) {
		c += (n%b) * o;
		n /= b;
		o *= 10;
// debug		printf("%" PRIu64 " %" PRIu32 " %" PRIu64 "\n", c, n, o);
	}
	
	return c + n*o; 
}


