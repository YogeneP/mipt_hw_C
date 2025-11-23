#include <stdio.h>

void print_digits(int n) {
    if(n >= 10) {
        print_digits(n/10);
        printf("%d ", n%10);
    } else printf("%d ", n);
}

int main() {
	int a;
	scanf("%d", &a);
    print_digits(a);
	return 0;
}
