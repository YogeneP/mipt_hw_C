#include <stdio.h>
#include <inttypes.h>

uint8_t a;

int main () {
	uint32_t sq; 
	scanf("%" SCNd8, &a);
	for (int i=1; i<=a; i++) {
		sq = i*i;
		printf("% " PRId32 "% " PRId32 "% " PRId32 "\n", i, sq, sq*i);
	}
	
	return 0;
} 
