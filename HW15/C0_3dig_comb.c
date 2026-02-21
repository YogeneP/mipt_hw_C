#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define DEBUG

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

size_t extractDigits(uint16_t **n_set, size_t *size) 
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

int main(void) 
{
	uint16_t* digits;
	size_t digits_size = 1 * sizeof(*digits);
	size_t digits_len = 0;
	uint16_t* numbers;
	size_t numbers_size = 5 * sizeof(*numbers);
	D printf("begin...\n");

	digits = malloc(digits_size);
	numbers = malloc(numbers_size);
	size_t res = extractDigits(&digits, &digits_size);
	D printf("extractDigits res = %zu\n", res);
	switch (res) {
		case 0:
			printf("Invalid input");
			return 1;
		case SIZE_MAX:
			printf("Memory allocation error");
			free(numbers);
			free(digits);
			return -1;
		case 1:
		case 2:
			printf("0");
			return 0;
		default:
			digits_len = res;
			D printf("length: %zu digits\n", digits_len);
	}

	uint16_t c = 0;
	uint16_t b = 0;
	for (uint16_t k = 0; k < digits_len-2; k++) {
		for (uint16_t j = k+1; j < digits_len-1; j++) {
			for (uint16_t i = j+1; i < digits_len; i++) {
				if((digits[k] == 0) || ((digits[k] == 0) & (digits[j] == 0))) {
					D printf("No number with leading zero(s) possible, passing by\n");
					continue;
				}
				if(numbers_size <= c*sizeof(*numbers)) {
					D printf("! numbers[] is out of memory: capacity %zu, c index %" PRIu16 "... ", numbers_size/sizeof(*numbers), c);
					if(expandMemory(&numbers, &numbers_size) == 0) {
						D printf("memory reallocation failure, exiting!\n");
						free(numbers);
						free(digits);
						return -1;
					}
					D printf("new numbers array size: %zu\n", numbers_size);
				}

				numbers[c] = digits[i] + 10*digits[j] + 100*digits[k];
				D printf("number %" PRIu16 " composed...\n", numbers[c]);
				b = 0;
				while (b < c) {
					if(numbers[c] == numbers[b]) {
						D printf("repeating number: %" PRIu16 " met at %" PRIu16 ", passing by\n", numbers[c], c);
						break;
					}
					b++;
				}
				if (b >= c) {
					D printf("new number: %" PRIu16 " stored at %" PRIu16 "\n ", numbers[c], c);
					c++;
				}
			}
		}	
	}
	printf("%" PRIu16, c);
	D printf(" unique numbers found\n");
	free(numbers);
	free(digits);	
	return 0;
}
