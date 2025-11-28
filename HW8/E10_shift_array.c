#include <stdio.h>

#define ARR_LEN 12
#define SHIFT 4
#define SHIFTED_I(I) (((i) - SHIFT + ARR_LEN) % ARR_LEN)

int arr[ARR_LEN];

int main() {	
	for(int i = 0; i < ARR_LEN; i++) {
		scanf("%d", &arr[i]);
	}
	
	for (int i = 0; i < ARR_LEN; i++) {
		printf("%d ", arr[SHIFTED_I(i)]);
	}
	printf("\n");
	
	return 0;
}
