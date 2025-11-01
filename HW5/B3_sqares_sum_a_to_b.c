#include <stdio.h>
#include <inttypes.h>

uint8_t a, b;

int main () {
	scanf("%" SCNd8 "%" SCNd8, &a, &b);
	if (a > b) return -1;

	uint32_t sum = 0;
	for (int i=a; i<=b; i++) {
		sum += i*i;
	}

	printf("% " PRId32, sum);
	return 0;
} 
