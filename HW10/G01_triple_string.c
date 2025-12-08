#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	FILE* fi;
	if( !(fi = fopen("input.txt", "rt")) ) {
		printf("Cant't open file");
		return 1;
	}
	
	char str[1000] = {0};
	fgets(str,999,fi);
	str[strlen(str)-1] = '\0';
	FILE* fo = fopen("output.txt", "wt");
	fprintf(fo, "%s, %s, %s", str, str, str);
	fprintf(fo, " %ld", strlen(str)); 
	fclose(fi);
	fclose(fo);
/*	
	fo = fopen("output.txt", "rt");
	char buffer[500];
	while(fgets(buffer,500,fo)) {
		printf("%s", buffer);
	}
	fclose(fo);
*/	
}
	
	
	
	
	
