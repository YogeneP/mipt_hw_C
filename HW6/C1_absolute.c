#include <stdio.h>
#include <inttypes.h>

uint32_t absolute(int32_t);

int main() {
	int32_t a;
	scanf("%" SCNd32, &a);
	printf("%" PRIu32 "\n", absolute(a) );
}

uint32_t absolute(int32_t val) {
	return val*(1-2*((val >> (sizeof(val)*8-1)) & 1U)); // :)
}
// the following is more optimal, but it is not mine, it is taken from internet:
// uint32_t fast_abs(int32_t val) {
//	uint32_t mask = val >> (sizeof(val)*8-1);
//	return (val ^ mask) - mask;
// }

