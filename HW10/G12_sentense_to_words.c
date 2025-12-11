#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int open_file( const char *filename, FILE **fptr, const char *mode);  

int main() {

	FILE* fi;
	FILE* fo;
	
	if(open_file("input.txt", &fi, "rt")) { return 1; }
	if(open_file("output.txt", &fo, "wt")) { return 2; }
	
	char c;
	int new_word = 1;
	while (((c = fgetc(fi)) != EOF) && (c != '\n')) {
		if (c == ' ') {
			if (!new_word) {
				new_word = 1;
				fputc('\n', fo);
			}
			continue;
		}
		new_word = 0;
		fputc(c, fo);
	}
	fclose(fi);
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
