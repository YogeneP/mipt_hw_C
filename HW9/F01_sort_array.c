// #include <stdio.h>

// int arr[10];

void sort_array(int size, int a[]) {
	int min = a[0];
	int min_i = 0;
	
	if (size < 2) return;
	//bubble had been processed on the lession, let's follow min-method
	for (int i = 0; i < size; i++) {
		int j = i;
		min = a[i];
		min_i = i;	 
		for (; j < size; j++) {
			if (a[j] < min) {
				min = a[j];
				min_i = j;
			}
		}

		if (j != i) {
			int t = a[i];
			a[i] = a[min_i];
			a[min_i] = t;
		}
	}	
}
/*
int main() {
	int arr[10] = { 10, 6, 8, 4, 5, 7, 33, 6, 6, 9 };
	int size = sizeof(arr)/sizeof(arr[0]);
	sort_array(size, arr);

	for (int j = 0; j < size; j++) {
		printf("%d ", arr[j]);
	}
}
*/
