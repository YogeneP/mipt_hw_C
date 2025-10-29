#include <stdio.h>
#include <limits.h>

int a = 0;
int b = 0;
int c = 0;
double res = 0;

int main() {
	scanf("%d %d %d", &a, &b, &c);
	res = ((double)(a)+b+c)/3;
	printf("%.2lf\n", res);
 
	return 0;
}
