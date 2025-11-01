#include <stdio.h>
#include <inttypes.h>

int32_t a = 0;
int32_t b = 0;

int main() {
	scanf("%" SCNd32 "%" SCNd32, &a, &b);
	a<b ? printf("%" PRId32 " %" PRId32, a, b) : printf("%" PRId32 " %" PRId32, b, a);
 
	return 0;
}
