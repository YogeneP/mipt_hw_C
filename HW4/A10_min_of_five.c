#include <stdio.h>
#include <inttypes.h>

#define INPUT_LENGTH 5

int32_t val[INPUT_LENGTH];

int main() {
	int32_t min;
	uint8_t i = 1;
	scanf("%" SCNd32, val);
	min = val[0]; 

	while (i < INPUT_LENGTH) {
		scanf("%" SCNd32, val+i);
		if (val[i] < min) min = val[i];
		i++;
	}
	printf("%" PRId32 , min);
	
	return 0;
}
