#include <stdio.h>
#include <inttypes.h>

#define STR_LENGTH 1000

char* yesno[] = { "NO" ,"YES"};

int main() {
	unsigned int i = 0;
	int c = 0;
	int p = 0;
	
	c = getchar();
	do {
		if (c == '.' || c == '\n' || c == 0 || c == EOF)  break; 
		if (c == '(') p++; 
		else {
			if (c == ')') {
				if (p == 0) { 
					printf("%s\n", yesno[0]); 
					return 0; 
				}
				p--;
			}
		}
		i++;
		c = getchar();
	} while (i < STR_LENGTH-1);  
	while ( (c = getchar() ) != '\n' && c != EOF ) {}
	
	printf("%s\n", yesno[p==0]);
	
	return 0;
}
