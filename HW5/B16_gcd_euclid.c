#include <stdio.h>
#include <inttypes.h>

uint32_t a = 0;
uint32_t b = 0;

int main() {
	scanf("%" SCNd32 " %" SCNd32, &a, &b);
 	if (a < b) { uint32_t t = a; a = b; b = t; }

 	uint32_t rem;
	do {
		rem = a % b;
		a = b;
		b = rem;
	} while (b > 0);
		
	printf("%" PRId32 "\n", a);
 	return 0;
}
