#include <stdio.h>
#include <inttypes.h>

uint32_t a = 0;

int main() {
	uint8_t max = 0;
	uint8_t min = 9;
	uint8_t digit = 0;

	scanf("%" SCNd32, &a);

	while (a > 0) {
		digit = a % 10;
		if (digit > max) max = digit;
		if (digit < min) min = digit;
		a /= 10;
	}

	printf("%" PRId8 " %" PRId8 "\n", min, max);
 	return 0;
}
