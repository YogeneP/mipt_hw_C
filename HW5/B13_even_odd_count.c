#include <stdio.h>
#include <inttypes.h>

uint32_t a = 0;

int main() {
	uint8_t odd = 0;
	uint8_t even = 0;
	
	scanf("%" SCNd32, &a);

	while (a > 0) {
		if ((a % 10) % 2) odd++;
		else even++;
		a /= 10;
	}

	printf("%" PRId8 " %" PRId8 "\n", even, odd);
 	return 0;
}
