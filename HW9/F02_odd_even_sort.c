/* #include <stdio.h>

#define ARR_LEN 10

int arr[ARR_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

void sort_even_odd(int n, int a[]);

int main() {
	sort_even_odd(ARR_LEN, arr);
	for (int i = 0; i < ARR_LEN; i++ ) {
		printf("%d ", arr[i]);
	}
}
*/

void sort_even_odd(int n, int a[]) {
	int odd[n];
	int even[n];
	int i_odd = 0;
	int i_even = 0;
	
	for (int i = 0; i < n; i++) {
		if (a[i] & 1U) {
			odd[i_odd] = a[i];
			i_odd++;
		} else {
			even[i_even] = a[i];
			i_even++;
		}
	}
	
	int odd_start = i_even;
	i_even--;
	i_odd = 0;

	while ( (i_even >= 0) || ((odd_start + i_odd) < n) ) {
		if (i_even >= 0) {
			a[i_even] = even[i_even];
			i_even--;
		} else {
			a[odd_start+i_odd] = odd[i_odd]; 
			i_odd++;
		}
	}
}


