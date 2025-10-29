#include <stdio.h>

int a = 0;

int main() {
	unsigned char sum;
	scanf("%d", &a);
	if ((a > 99) && (a < 1000)) {
		sum = a/100 + (a%100)/10 + a%10;
		printf("%d\n", sum);
		return 0;
	}
	printf("Invalid input!");
 	return 1;
}
