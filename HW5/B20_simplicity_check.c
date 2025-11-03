#include <stdio.h>
#include <inttypes.h>
#include <math.h>
uint32_t a;
char* yesno[] = { "NO\n", "YES\n"};

int main() {
	scanf("%" SCNu32, &a);
	/*
	 * Possible simple numbers belong to [2,3,6k+-1] set
	 * BUT not every member of 6k+-1 set is simple - test #9: 121 = 11*11 :(  
	*/	 
	// a < 4 - 2 and 3 are simple
	if (a < 4) {
	// a > 1 - exclusion of 1 and 0
		printf("%s", yesno[a > 1]); 
		return 0;
	}
	// Trying 2 and 3 as divider
	if (!(a%2 && a%3)) {
		printf("%s", yesno[0]);
		return 0;
	}
	// Trying all potential simple 6k+-1 numbers up to sqrt(a)+1 as divider for a, 
	// to cover the numbers of (6k-1)^2 kind
	// after sqrt(a) the multiplier pairs repeat in reversed order - no need to check.  
	uint32_t c = sqrt(a)+1;
 	for (uint32_t i = 6; i <= c; i+=6) {
		if (!(a%(i+1) && a%(i-1))) {
			printf("%s", yesno[0]);
			return 0;
		}
	}
	printf("%s", yesno[1]); 
	return 0;
}
