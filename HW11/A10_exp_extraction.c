#include <stdio.h>
#include <inttypes.h>

int extractExp(float f) {
	typedef union {
		uint32_t i;
		float f;
	} fl_t;
	fl_t fl;
	fl.f = f;
	return ((fl.i >> 23) & 0xFF);
}


int main(void) {
	float n = 0.0;
	scanf("%f", &n);
	printf("%d", extractExp(n) );
}
