#include <stdio.h>
#include <inttypes.h>

uint32_t a;

int main() {
	scanf("%" SCNd32, &a);

	uint32_t t;
	uint32_t prod;
	uint8_t sum;
	uint8_t digit;
	for (uint32_t i = 10; i <= a; i++) {
		t = i;
		prod = 1;
		sum = 0;
		while (t > 0) { 
			digit = t % 10;
			sum += digit;
			prod *= digit;
			t /= 10;
		}
		if (!(sum - prod)) 
			printf ("%" PRId32 " ", i);
	}

	printf ("\n");
	return 0;
}
