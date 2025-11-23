#include <stdio.h>

int sum_digits(int n) {
    if(n >= 10) {
        return (n%10) + sum_digits(n/10);
    }
    return n;
}

int main() {
	int a;
	scanf("%d", &a);
    printf("%d", sum_digits(a));
	return 0;
}