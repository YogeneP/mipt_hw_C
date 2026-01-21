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
	uint32_t n = 0;
	uint32_t p = 0;
	uint32_t hash = 0;
	scanf("%"SCNu32, &n);
	for (uint32_t i = 0; i < n; i++) {
		scanf("%"SCNu32, &p);
		hash ^= p;
//		print32bin(hash);
	};
    
	printf("%" PRIu32, hash);
}
