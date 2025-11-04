#include <stdio.h>
#include <inttypes.h>

uint32_t a = 1;

int main() {
	uint32_t count = 0;
	
	while (a > 0) {
		scanf("%" SCNd32, &a);
		count++;
	}
		
	printf("%" PRId32 "\n", count-1);
 	return 0;
}
