#include <stdio.h>
#include <inttypes.h>

#define MAX_LENGTH 100

char s[MAX_LENGTH];
const uint8_t Aa = 'a' - 'A';
int main() {
	fgets(s, MAX_LENGTH, stdin);

	for (uint8_t i = 0; i < MAX_LENGTH; i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') 
			s[i] += Aa;
		else if (s[i] == '.') {
			s[i] = 0x00;
			break;
		}
	}
	
	printf("%s", s);
}
