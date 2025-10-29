#include <stdio.h>

unsigned int a = 0;

int main() {
	unsigned int prod;
	scanf("%d", &a);
	if ((a > 99) && (a < 1000)) {
		prod = (a/100) * ((a%100)/10) * (a%10);
		printf("%d\n", prod);
		return 0;
	}
	printf("Invalid input!");
 	return 1;
}
