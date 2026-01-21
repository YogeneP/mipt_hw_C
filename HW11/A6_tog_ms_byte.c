#include <stdio.h>
#include <inttypes.h>

#define BYTE_BITS 8
#define BITS 32

int main(void) {
	uint32_t n;
	scanf("%" SCNu32, &n);
	printf ("%" PRIu32, n ^ (((1 << BYTE_BITS) - 1) << (BITS - BYTE_BITS)));	
}
