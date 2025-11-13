#include <stdio.h>
#include <inttypes.h>
#include <math.h>
uint32_t a;

void print_simple(int);

int main() {
	scanf("%" SCNu32, &a);
	
	print_simple(a);

	return 0;
}

//deliberately recursive implementation according to lessons subject :)
void print_simple(int n) { 
	// n < 2 - exclusion of 1 and 0
	if (n < 2) {
		return;
	}	
	// n < 4 - 2 and 3 are simple
	if (n < 4) {
		printf("%d ", n);
		return;
	}
	// Trying 2 and 3 as divider
	for (uint32_t i = 2; i <= 3; i++) {
		if (!(n%i)) {
			printf("%d ", i);
			print_simple(n/i);
			return;
		}
	}
	// Trying all potential simple 6k+-1 numbers up to sqrt(n)+1 as divider for n, 
	// to cover the numbers of (6k-1)^2 kind as well.
	// No need to check 6k+-1 numbers for simplicity before dividing: the smaller simple range members will 
	// trigger the condition of being simple components before it reaches any non-simple 6k+-1 number.
	// After sqrt(n) the multiplier pairs repeat in reversed order - no need to check.  
	
	uint32_t c = sqrt(n)+1;
 	for (uint32_t i = 6; i <= c; i+=6) {
		if (!(n%(i+1))) {
			printf("%d ", i+1);
			print_simple(n/(i+1));
			return;
		}
		if (!(n%(i-1))) {
			printf("%d ", i-1);
			print_simple(n/(i-1));
			return;
		}
	}
	printf("%d ", n);
}
