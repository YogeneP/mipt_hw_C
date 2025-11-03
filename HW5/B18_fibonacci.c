#include <stdio.h>
#include <inttypes.h>

uint32_t n;
uint64_t f[] = { 0, 1 };


int main () {
	scanf("%" SCNu32, &n);
	
	printf("%" PRIu64, f[1]);
	uint8_t b = 0;
	for (uint32_t i=0; i < n-1; i++) {
		f[b] = f[0] + f[1];
		printf(" %" PRIu64, f[b]);
		b = !b;
	}
	
	printf("\n");
	return 0;
}
