#include <stdio.h>
#include <string.h>

#define STR_LEN 2000 //need twice as big string: according to task even to the one character the repeating index must be assigned - seems it is wrong problem description  

char strg[STR_LEN];

void process_str(char* str);

int main() {
	scanf("%999s", strg);
	process_str(strg);
	printf("%s", strg);
}

void process_str(char* str) {
	char s[STR_LEN] = {'\0'};
	s[0] = str[0];
	char prev_char = s[0];
	int i = 1;
	int j = 1;
	int c = 1;
	while ( str[i] != '\0' && str[i] != '.' ) {
		if(str[i] != prev_char) {
//			if(c > 1) {
				char c_str[12] = {'\0'};
				snprintf(c_str, sizeof(c_str), "%d", c);
				j += strcat(s, c_str);
				c = 1;
//			}
			s[j] = str[i]; 
			j++;	
			prev_char = str[i];
		} else {
			c++;
		}
		i++;
	}
//	if (c > 1) {
		char c_str[12] = {'\0'};
		snprintf(c_str, sizeof(c_str), "%d", c);		
		strcat(s, c_str);
		j += strlen(c_str);
//	}
	s[j] = '\0';
	strcpy(str, s);
}

