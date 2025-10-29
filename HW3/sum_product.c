#include <stdio.h>
#include <limits.h>

int a = 0;
int b = 0;
int c = 0;
long long int res = 0;

int main() {
	scanf("%d %d %d", &a, &b, &c);
	res = (long long int)(a)+b+c;
	printf("%d+%d+%d=%lld\n", a, b, c, res);
	res = (long long int)(a)*b*c;
	printf("%d*%d*%d=%lld\n", a, b, c, res);
 
	return 0;
}
