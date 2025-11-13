#include <stdio.h>
#include <inttypes.h>

uint8_t n;

uint64_t factorial(uint8_t);

int main() {
	scanf("%" SCNd8, &n);
	
	printf("%" PRId64, factorial(n));
}

uint64_t factorial(uint8_t a) {
	return a > 0 ? a * factorial (a-1) : 1;
}
