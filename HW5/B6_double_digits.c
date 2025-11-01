#include <stdio.h>
#include <inttypes.h>

uint32_t a = 0;

int main() {
	scanf("%" SCNd32, &a);

	uint32_t prev_digit = a % 10;
	uint32_t curr_digit = 0;
	while (a > 10) {
		a /= 10;
		curr_digit = a % 10;
		if (prev_digit == curr_digit) {
			printf("%s", "YES");
			return 0;
		}
		prev_digit = curr_digit;
	}

	printf("%s", "NO");
 	return 0;
}
