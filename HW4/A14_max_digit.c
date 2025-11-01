#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#define INPUT_LENGTH 3 //should be 2..10
uint32_t a = 0;

int main() {
	uint32_t max = 0;
	uint8_t digit = 0;
	uint8_t i = 0;
	scanf("%" SCNd32, &a);
	if ((a < pow(10,INPUT_LENGTH-1)) || (a > pow(10,INPUT_LENGTH) - 1)) 
		return -1;

	while (i < INPUT_LENGTH) {
		digit = a % 10;
		a /= 10;
		i++;
		if (digit > max) max = digit;
	}
	printf("%" PRId32, max);
 	return 0;
}
