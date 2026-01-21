#include <stdio.h>
#include <inttypes.h>

#define LETTERS_COUNT 26
int main(void) {
	uint32_t n;
	char c;
	scanf("%" SCNu32, &n);
	
	while (((c = getchar()) >= 'A' && c <='Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '.') {
		if (c == '.') {
			printf("%c", c);
			break;
		}
		if (c != ' ') {
			uint8_t num = (c < 'a') ? c - 'A' : c - 'a';
			c = (num + n) % LETTERS_COUNT + (c - num);
		}
		printf("%c", c);
	}    
	return 0;
}
