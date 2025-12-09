#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void build_triangle(uint16_t c);

int main() {
	build_triangle(10);
/*
	FILE* fi;
	 
	if( !(fi = fopen("input.txt", "rt")) ) {
		printf("Cant't open file");
		return 1;
	}
	
	uint16_t count = 0;
	
	while (((c = fgetc(fi)) != EOF)) {
		if (c == '*') count++;
	}
	
	if (count >= 3) {

		uint64_t f[2] = {1, 2};
		uint16_t n = 2;
		
		for (uint16_t i=0; i < UINT_MAX; i++) {
			f[b] = f[0] + f[1];
			num++;
			printf(" %" PRIu64"\n", f[b]);
			if (f[b] > count) break;
			if (f[b] = count) { 
				build_triangle(num);
				return 0;
			}
			b = !b;
		}
	} 
	
	printf("NO");
	return 0;
	
		printf("%c; code = %d\n", c, code = get_letter_code(&c));
		if( ((code = get_letter_code(&c)) > 0) && (code + '0' != str[i-1]) ) {
			str[i] = code + '0';
			i++;
		}
	}
*/ }

void build_triangle(uint16_t n) {
	FILE* fo;
	
	if( !(fo = fopen("output.txt", "wt")) ) {
		printf("Cant't open file");
		return;
	}
	uint16_t s_pos = n-1;
	char c[2] = {' ', '*'};
	uint8_t ci = 0;
	for(uint16_t i = 0; i < n; i++) {
		ci = 0;
		for(uint16_t j = 0; j < n*2; j++) {
			if (j >= s_pos) { 
				if (j > n + i) break;
				ci = !ci;
			}
			printf("%c",c[ci]);
			fputc(c[ci],fo); 
		}
		printf("\n");
		fputc('\n',fo);
		s_pos--;
	}
}
/*			
 0123456789
0     *
1    * * 
2   * * *
3  * * * * 
4 * * * * *
5* * * * * *
*/


