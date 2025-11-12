#include <stdio.h>
#include <inttypes.h>

#define STR_LENGTH 100
#define CAPS_SHIFT 30

char upper_case(char);

int main() {
	char str[STR_LENGTH+1] = "";

	uint32_t i = 0;
	int c = 0;
	do {
		c = getchar();
		if (c == '.' || c == '\n' || c == 0 || c == EOF) { str[i] = '\0'; break; }
		if (c >= 'a' && c <= 'z') { 
			str[i] = upper_case(c); 
		} else { str[i] = c; }
	} while (++i < STR_LENGTH); 
	while ( (c = getchar() ) != '\n' && c != EOF ) {}
	
	printf("%s\n", str);
	
	return 0;
}

char upper_case(char ch) {
	return ch - 0x20;
}
