#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int32_t a = 0;

int main() {
	scanf("%" SCNd32, &a);
	a = abs(a);
	uint8_t digit = 0;
	uint16_t used_digits = 0;

	while (a > 0) {
		digit = a % 10;
		if ((used_digits >> digit) & 1U) {
			printf("%s", "YES");
			return 0;
		}
		used_digits = used_digits | (1U << digit);
		a /= 10;
	}

	printf("%s", "NO");
 	return 0;
}
