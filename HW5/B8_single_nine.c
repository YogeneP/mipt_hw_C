#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define DIGIT 9

int32_t a = 0;
char* yesno[] = { "NO\n", "YES\n" };

int main() {
	scanf("%" SCNd32, &a);
	a = abs(a);
	uint8_t digit_used_once = 0;
	uint8_t digit;

	while (a > 0) {
		digit = a % 10;
		if (digit == DIGIT) {
			if (digit_used_once) {
				digit_used_once = 0;
				break;
			} else 
				digit_used_once = 1;
		}
		a /= 10;
	}

	printf("%s", yesno[digit_used_once]);
 	return 0;
}
