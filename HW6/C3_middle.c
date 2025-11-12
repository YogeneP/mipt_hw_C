#include <stdio.h>
#include <inttypes.h>

int32_t middle(uint32_t, uint32_t);

int main() {
	uint32_t a;
	uint32_t b;
	scanf("%" SCNd32 "%" SCNd32, &a, &b);
	printf("%" PRId32 "\n", middle(a,b) );
}

int32_t middle(uint32_t a, uint32_t b) {
	return (a + b) / 2;
}
