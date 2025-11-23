#include <stdio.h>
#define STRING_LENGTH 10000

int digit_to_num(char*);

int main() {
	char str[STRING_LENGTH] = ""; 	

	int i = 0;
	char c;

	while ((c = getchar()) != '\n' && c != EOF && i < STRING_LENGTH) {
		if (c == '.') {
			str[i] = '\0';
 			printf("%d", digit_to_num(str));
			return 0;
		}
		str[i] = c;
		i++;
	} 

	printf("Invalid input");
	return 1;
}

int digit_to_num(char *str) {
	int s = 0;
	int i = 0;

	while (str[i] != '\0') { 
		if (str[i] >= '0' && str[i] <= '9') {
			s += str[i] - '0';
		}
		i++;
	}
	
	return s;
}
