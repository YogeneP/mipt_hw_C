#include <stdio.h>

void print_digits_reversed(int n) {
    if(n >= 10) {
        printf("%d ", n%10);
        print_digits_reversed(n/10);
    } else printf("%d ", n);
}

int main()
{
	int a;
	scanf("%d", &a);
    print_digits_reversed(a);
	return 0;
}
