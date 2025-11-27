#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int n;

int comp(const void *a, const void *b) {
	return *(int*)b-*(int*)a;
}

int main() {
	int32_t arr[10] = {0}; // int32 up to 10 digits
	int i = 0;
	scanf("%d", &n);
	while (n > 0) {
		arr[i] = n%10;
		n /= 10;
		i++;
	}
	qsort(arr, i, sizeof(arr[0]), comp);
	for (int j = 0; j < i; j++) {
		putchar(arr[j] + '0');
	}
}
