#include <stdio.h>

void print_upto_n(int n) {
	if (n > 1) {
		print_upto_n(n-1);
	}
	printf("%d ", n);
}

int main()
{
	int a;
	scanf("%d", &a);
	print_upto_n(a);
	return 0;
}