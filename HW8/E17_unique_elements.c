#include <stdio.h>

#define ARR_LEN 10

int arr[ARR_LEN];

int main() {
	int double_element = 0;
	
	for(int i = 0; i < ARR_LEN; i++) {
		scanf("%d", &arr[i]);
	}
	
	// using naive algorithm, for small array it is OK:
	// no sort, no hashmap
	for (int i = 0; i < ARR_LEN; i++) {
		for (int j = 0; j < ARR_LEN; j++) {
			if (i != j) {
				if (arr[i] == arr[j]) {
					double_element = 1;
					break;
				}
			}
		}
		if (!double_element) printf("%d ", arr[i]);
		double_element = 0;
	}
	printf("\n");
	
	return 0;
}
