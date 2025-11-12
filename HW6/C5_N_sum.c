#include <stdio.h>
#include <inttypes.h>

uint32_t a;

uint64_t getSum(uint32_t);

int main() {
	scanf("%" SCNd32, &a);
	
	printf("%" PRId64, getSum(a));
}

uint64_t getSum(uint32_t n) {
	uint32_t nd2 = n >> 1;
	
	return (n+1)*nd2+n%2*(nd2+1);
}
