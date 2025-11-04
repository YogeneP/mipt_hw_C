#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int32_t a = 0;

int main() {
	scanf("%" SCNd32, &a);
	a = abs(a);
	
	uint8_t prev_digit = a % 10;
	uint8_t digit = 0;
	
	while (a > 10) {
		a /= 10;
		digit = a % 10;
		if (digit >= prev_digit) {
			printf("%s", "NO\n");
			return 0;
		}
		prev_digit = digit;
	}

	printf("%s", "YES\n");
 	return 0;
}
