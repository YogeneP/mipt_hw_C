#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

void build_triangle(uint16_t n, FILE** f);
int open_file(const char* filename, FILE** f, const char* mode);

int main() {
	FILE* fi;
	FILE* fo;	
	 
	if ( open_file("input.txt", &fi, "rt") ) return -1;
	if ( open_file("output.txt", &fo, "wt")) return 1;	

	uint16_t count = 0;
	char c = 0;
	
	while (((c = fgetc(fi)) != EOF)) {
		if (c == '*') count++;
	}

	fclose(fi);

	if (count >= 3) {

//		uint64_t f[2] = {1, 2};
		uint64_t b = 1;
		uint16_t num = 1;
				
		for (uint16_t i=0; i < UINT_MAX; i++) {
			b += num + 1;
			num++;
//			printf("%d: %lld\n", num, f[b]);
			if (b > count) break;
			if (b == count) { 
				build_triangle(num, &fo);
				return 0;
			}
		}
	} 
//	printf("NO");
	fprintf(fo, "NO");
	
	fclose(fo);
	
	return 0;
}


int open_file( const char *filename, FILE **fptr, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        printf("Can't open file: %s\n", filename);
        return 1;
    }
    *fptr = fp;
    return 0;
}

void build_triangle(uint16_t n, FILE** f) {
	if (*f == NULL) return;
	 
	uint16_t s_pos = n-1;
	char c[2] = {' ', '*'};
	uint8_t ci = 0;
	for(uint16_t i = 0; i < n; i++) {
		ci = 0; 
		for(uint16_t j = 0; j < n*2; j++) {
			if (j >= s_pos) { 
				if (j > n + i - 1) break;
				ci = !ci;
			}
//			printf("%c",c[ci]);
			fputc(c[ci],*f); 
		}
//		printf("\n");
		fputc('\n',*f);
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


