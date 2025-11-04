#include <stdio.h>
#include <inttypes.h>

uint32_t a = 0;

int main() {
	scanf("%" SCNd32, &a);
	if (!a) {
		printf("0");
		return 0;
	}
	while (a > 0) {
		printf("%" PRId8, a % 10);
		a /= 10;
	}
 	return 0;
}
