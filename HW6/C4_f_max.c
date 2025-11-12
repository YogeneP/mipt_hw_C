#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

int32_t foo(int32_t);

int main() {
	int32_t x = 0;
	uint32_t max = 0; //foo > 0
	uint32_t f = 0;
	int8_t sign = 1; //use sign = 0 if invalid sequence is going until next ' ' or \n
	int c = 0;
	while (1) {
		c = getchar();
		if(sign != 0) {
			if (c == '0' && x == 0) break; // weak point - leading zeroes (e.g. 065)
			if (c >= '0' && c <= '9') {
				x = x*10 + (c - '0'); 
				continue;
			}
			if (c == '-') {
				if (x == 0) sign = -1; 
				else sign = 0; 
				continue;
			} 
			if (c == ' ' || c == '\n' || c == 0 || c == EOF ) { // /n is recognized as splitter as well
				if (x != 0) {
					x *= sign; 
					if ((f = foo(x)) > max) max = f;
// debug					printf("f(%" PRId32 ") = %" PRId32 "\n", x, f);
					x = 0;
					sign = 1;
				}
			} else sign = 0;
		} else if (c == ' ' || c == '\n') { // /n is recognized as splitter as well
			x = 0;
			sign = 1;
		}
		if (c == 0 || c == EOF) break;
	}
	
	while ( (c = getchar() ) != '\n' && c != EOF ) {}
	
	if (max > 0) printf("%" PRId32 "\n", max);
	
	return 0;
}

int32_t foo(int32_t arg) {
	if (arg < -2 ) return 4;
	if (arg >= 2) return arg*arg+4*arg+5;
	return arg*arg;
}


