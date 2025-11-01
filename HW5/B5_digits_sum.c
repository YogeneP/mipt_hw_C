#include <stdio.h>
#include <inttypes.h>

uint32_t a = 0;

int main() {
	scanf("%" SCNd32, &a);

	uint32_t sum = 0;
	while (a > 0) {
		sum += a % 10;
		a /= 10;
	}
	printf("%" PRId32, sum);
 	return 0;
}
