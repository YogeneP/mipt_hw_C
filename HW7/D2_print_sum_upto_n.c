#include <stdio.h>

int print_sum_upto_n(int n) {
	return (n > 1) ? n + print_sum_upto_n(n-1) : 1;
}

int main()
{
	int a;
	scanf("%d", &a);
	printf("%d ", print_sum_upto_n(a));
	return 0;
}