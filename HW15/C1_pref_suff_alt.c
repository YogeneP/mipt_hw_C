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
 * Complexity: O(n^2) <= n<for>*(2*n/2<memcmp>)
 * Memory usage: O(n) <= 2*n<strings>+4*n<pref/suff>
 */
	uint16_t s1len = strlen(s1);
	uint16_t s2len = strlen(s2);
	uint16_t s_p_len = min(s1len,s2len);
    char* suff1 = calloc(s_p_len,sizeof(char)); 
    char* pref1 = calloc(s_p_len,sizeof(char));
    char* suff2 = calloc(s_p_len,sizeof(char)); 
    char* pref2 = calloc(s_p_len,sizeof(char));
    uint16_t max_length1 = 0;
    uint16_t max_length2 = 0;

	for(int i = 0; i < min(s1len,s2len); i++) {
        memcpy(pref1+i, s1+i, 1);
        memcpy(suff1+s_p_len-1-i, s2+s2len-i-1, 1);
        if(!memcmp(pref1,suff1+s_p_len-1-i,i+1)) max_length1 = i+1 ;
		printf("%" PRIu16 ". s1pref: %s; s2suff: %s; max: %" PRIu16 "\n", i, pref1, suff1+s_p_len-1-i, max_length1);
        memcpy(pref2+i, s2+i, 1);
        memcpy(suff2+s_p_len-1-i, s1+s1len-i-1, 1);
        if(!memcmp(pref2,suff2+s_p_len-1-i,i+1)) max_length2 = i+1;
		printf("%" PRIu16 ". s2pref: %s; s1suff: %s; max: %" PRIu16 "\n", i, pref2, suff2+s_p_len-1-i, max_length2);
    } 
	printf("%" PRIu16 " %" PRIu16, max_length1, max_length2);	
	free(suff1);
	free(pref1);
	free(suff2);
	free(pref2);
}
