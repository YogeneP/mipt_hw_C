#include <stdio.h>
#include <inttypes.h>

#define INPUT_LENGTH 3

int32_t val[INPUT_LENGTH];

int main() {
	int32_t max;
	uint8_t i = 1;
	scanf("%" SCNd32, val);
	max = val[0]; 

	while (i < INPUT_LENGTH) {
		scanf("%" SCNd32, val+i);
		if (val[i] > max) max = val[i];
		i++;
	}
	printf("%" PRId32 , max);
	
	return 0;
}
