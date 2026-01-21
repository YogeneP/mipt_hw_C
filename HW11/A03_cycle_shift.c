#include <stdio.h>
#include <inttypes.h>

/* void print32bin(uint32_t bin) {
	for(int i = 1; i <= 32; i++) {
		printf("%"PRIu32, (bin >> (32-i)) & 1);
	}
	printf("\n");
}
*/
int main(void) {
	uint32_t n = 19;
	uint8_t s = 3;
    scanf("%"SCNu32 "%"SCNu8, &n, &s);
    
    if (s > 31) {
		printf("Incorrect input");
		return 1;
	}
/*	print32bin(n);
	print32bin((n << (32 - s)));
	print32bin(((n >> s) & ((1 << (32-s))-1)));
	print32bin(((n) << (32 - s)) | ((n >> s) & ((1 << (32 - s)) - 1)));
*/

// explicit msb-part mask ((1 << (32 - s)) - 1) is redunant, but assures 
// signed n to be processed correctly (just in case) 
	printf("%" PRIu32, ((n) << (32 - s)) | ((n >> s) & ((1 << (32 - s)) - 1)) );
}
