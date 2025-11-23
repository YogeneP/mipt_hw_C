#include <stdio.h>

long long int recurs_power(int n, int p) {
    return (p > 1) ? n*recurs_power(n,p-1) : (p > 0) ? n : 1;
}

int main()
{
	int a, power;
	scanf("%d %d", &a, &power);
    printf("%lld ",recurs_power(a, power));
	return 0;
}
