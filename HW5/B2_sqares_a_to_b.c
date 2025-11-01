#include <stdio.h>
#include <inttypes.h>

uint8_t a, b;

int main () {
	scanf("%" SCNd8 "%" SCNd8, &a, &b);
	for (int i=a; i<=b; i++) {
		printf("% " PRId32, i*i);
	}
	
	return 0;
} 
