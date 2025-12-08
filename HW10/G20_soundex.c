#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
0: a, e, h, i, o, u, w, y
1: b, f, p, v
2: c, g, j, k, q, s, x, z
3: d, t
4: l
5: m, n
6: r
 */



static const char letter_codes[] = { 
	['a'-'a'] = 0, ['b' - 'a'] = 1, ['c' - 'a'] = 2, ['d' - 'a'] = 3, 
	['e'-'a'] = 0, ['f' - 'a'] = 1, ['g' - 'a'] = 2, ['h' - 'a'] = 0,
	['i'-'a'] = 0, ['j' - 'a'] = 2, ['k' - 'a'] = 2, ['l' - 'a'] = 4,
	['m'-'a'] = 5, ['n' - 'a'] = 5, ['o' - 'a'] = 0, ['p' - 'a'] = 1,
	['q'-'a'] = 2, ['r' - 'a'] = 6, ['s' - 'a'] = 2, ['t' - 'a'] = 3,
	['u'-'a'] = 0, ['v' - 'a'] = 1, ['w' - 'a'] = 0, ['x' - 'a'] = 2,
	['y'-'a'] = 0, ['z' - 'a'] = 2
};   

int main() {

	FILE* fi;
	FILE* fo;
	if( !(fi = fopen("input.txt", "rt")) && !(fo = fopen("output.txt", "wt")) ) {
		printf("Cant't open file");
		return 1;
	}
	
	unsigned char c = fgetc(fi);
	if (c > 'a' && c < 'z') fputc(c, fo);
	else if (c > 'A' && c < 'Z') fputc((c-('A'-'a')), fo);
	else return -1;

	int i = 1;
	while ((c = fgetc(fi)) != EOF && (c != '\0') && (c != '\n') && (i < 4)) {
		if(c >'a' && c < 'z') { 
			fputc(('0' + letter_codes[c - 'a']), fo);
			i++;
			break;
		}
		if(c >'A' && c < 'Z') { 
			fputc(('0' + letter_codes[c - 'A']), fo);
			i++;
			break;
		}
	}
		
	while (i < 4) {
		fputc('0', fo);
		i++;
	}
	
	fclose(fi);
	fclose(fo);

/*	fo = fopen("output.txt", "rt");
	char buffer[500];
	while(fgets(buffer,500,fo)) {
		printf("%s", buffer);
	}
	fclose(fo);
*/ }
	
	
	
	
	
