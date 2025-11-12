#include <stdio.h>
#include <inttypes.h>

uint64_t power2(uint8_t);

int main() {
	uint8_t a;
	scanf("%" SCNu8, &a);
	printf("%" PRIu64 "\n", power2(a-1) );
}

// using C2 - power function
uint64_t power2(uint8_t p) {
	return p > 0 ? 2*power2(--p) : 1;
}


