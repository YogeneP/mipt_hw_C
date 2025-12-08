#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	unsigned int N;
	
	FILE* fi;
	if( !(fi = fopen("input.txt", "rt")) ) {
		printf("Cant't open file");
		return 1;
	}
	
	fscanf(fi, "%d", &N);
	FILE* fo = fopen("output.txt", "wt");
	
	int i = 0;
	int n = 0;
	while (i < N / 2) {
		char c = ('A' + (i++));
		n += 2;
		fprintf(fo, "%c%d", c, n%10 == 0 ? (n = 2) : n );
	}
	
	fclose(fi);
	fclose(fo);
/*
	fo = fopen("output.txt", "rt");
	char buffer[500];
	while(fgets(buffer,500,fo)) {
		printf("%s", buffer);
	}
	fclose(fo);
*/ }
	
	
	
	
	
