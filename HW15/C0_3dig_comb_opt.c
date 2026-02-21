#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

// #define DEBUG

#ifdef DEBUG
#define D if(1) 
#else 
#define D if(0) 
#endif

/*
capacity - max possible count of array elements according to allocated memory
count, length - actual count of array elements
size - allocated amount of memory in bytes
*/
size_t expandMemory(uint16_t** arr, size_t* arr_size) 
{
	if(!(*arr)) 
	{ 
		return 0; 
	}

	size_t new_size = 2*(*arr_size);
	uint16_t *temp;

	temp = realloc(*arr, new_size);
	if (temp == NULL) 
	{
		return 0;
	}

	*arr_size = new_size;
	*arr = temp;

	return new_size;
}

size_t inputDigits(uint16_t **n_set, size_t *size) 
{
	int i = 0;
	char c = 0;
	while((c = getchar()) != '\n' && c != EOF) {
		D printf ("character found '%c'", c);
		if(c < '0' || c > '9') {
			memset(*n_set, 0, *size);
			*size = 0;
			return 0;
		}
		if (i*sizeof((*n_set[0])) >= *size) {
			if (expandMemory(n_set, size) == 0) {
				return SIZE_MAX;
			}
		}
		(*n_set)[i] = c - '0';
		D printf ("added [%d] %" PRIu16 "\n", i, (*n_set)[i]);
		i++;	
	} 
	D printf ("done. %d added in total\n", i);
	return i;
}

int canBeFormed(uint16_t num, uint16_t *digits, uint16_t digits_len) {
	if(num < 100 || num > 999) { return -1; }
	uint8_t ref[3];
	uint8_t ref_i = 0;
	ref[0] = num / 100;
	ref[1] = (num / 10) % 10;
	ref[2] = num % 10;
	
	for (int i = 0; i < digits_len; i++) {
		if(digits[i] == ref[ref_i]) {
			ref_i++;
		}
		if(ref_i > 2) {
			return 1;
		}
	}
	return 0; 
}

int main(void) 
{
	uint16_t* digits;
	size_t digits_size = 1 * sizeof(*digits);
	size_t digits_len = 0;
	D printf("begin...\n");

	digits = malloc(digits_size);
	size_t res = inputDigits(&digits, &digits_size);
	D printf("inputDigits res = %zu\n", res);
	switch (res) {
		case 0:
			printf("Invalid input");
			return 1;
		case SIZE_MAX:
			printf("Memory allocation error");
			free(digits);
			return -1;
		case 1:
		case 2:
			printf("0");
			free(digits);
			return 0;
		default:
			digits_len = res;
			D printf("length: %zu digits\n", digits_len);
	}

	uint16_t n_count = 0;
	for (uint16_t n = 100; n < 1000; n++) {
		if(canBeFormed(n, digits, digits_len)) {
			n_count++;
		}
	}

	printf("%" PRIu16, n_count);
	D printf(" unique numbers found\n");
	free(digits);	
	return 0;
}
