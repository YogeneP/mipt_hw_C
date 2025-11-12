#include <stdio.h>
#include <inttypes.h>

int64_t power(int16_t, uint8_t);

int main() {
	int16_t a;
	uint8_t b;
	scanf("%" SCNd16 "%" SCNu8, &a, &b);
	printf("%" PRId64 "\n", power(a,b) );
}

//recursive function - I do not like :( , but just for educational purpose: 
int64_t power(int16_t n, uint8_t p) {
	return p > 0 ? n*power(n, --p) : 1;
}


