#include <stdio.h>
#include <inttypes.h>

uint32_t a;
char* yesno[] = { "NO\n", "YES\n" };

int main() {
	scanf("%" SCNu32, &a);
	
	uint8_t sum;
	while (a > 0 && sum < 10) {
		sum += (a % 10);
		a /= 10;
	}
	
	printf ("%s", yesno[sum==10]);
	return 0;
}
