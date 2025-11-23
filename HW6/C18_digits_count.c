#include <stdio.h>
#define STRING_LENGTH 10000

int is_digit(char*);

int main() {
	char str[STRING_LENGTH] = ""; 	

	int i = 0;
	char c;

	while ((c = getchar()) != '\n' && c != EOF && i < STRING_LENGTH) {
		if (c == '.') {
			str[i] = '\0';
 			printf("%d", is_digit(str));
			return 0;
		}
		str[i] = c;
		i++;
	} 

	printf("Invalid input");
	return 1;
}

int is_digit(char *str) {
	int i = 0;
	int s = 0;
	while (str[i] != '\0') { 
		if (str[i] >= '0' && str[i] <= '9') {
			s++;
		}
		i++;
	}
	
	return s;
}
