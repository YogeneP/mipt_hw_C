#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

char s1[1025];
char s2[1025];

size_t min(size_t a, size_t b) {
	return((a <= b)? a : b);
}

int main(void) {
	scanf("%s\n%s", s1, s2);
	
/* naive implementation:	
 * increase s1 prefix by one and check if it is a suffix of s2
 * and visa versa
 * Complexity: O(n^2) <= n<for>*(4*n/2<memcpy>)
 * Memory usage: O(n) <= 2*n<strings>+2*n<pref/suff>
 */
	uint16_t s1len = strlen(s1);
	uint16_t s2len = strlen(s2);
    char* suff = calloc(min(s1len,s2len),sizeof(char)); 
    char* pref = calloc(min(s1len,s2len),sizeof(char));
    uint16_t max_length1 = 0;
    uint16_t max_length2 = 0;

	for(int i = 1; i <= min(s1len,s2len); i++) {
        memcpy(pref, s1, i);
        memcpy(suff, s2+s2len-i, i);
        if(!memcmp(pref,suff,i)) max_length1 = i;
//		printf("%" PRIu16 ". s1pref: %s; s2suff: %s; max: %" PRIu16 "\n", i, pref, suff, max_length1);
        memcpy(pref, s2, i);
        memcpy(suff, s1+s1len-i, i);
        if(!memcmp(pref,suff,i)) max_length2 = i;
//		printf("%" PRIu16 ". s2pref: %s; s1suff: %s; max: %" PRIu16 "\n", i, pref, suff, max_length2);
    } 
	printf("%" PRIu16 " %" PRIu16, max_length1, max_length2);	
	free(suff);
	free(pref);
}
