#include <stdio.h>
#include <inttypes.h>

int32_t a, b, c;
char* yesno[] = { "NO\n", "YES\n" };

int main() {
	scanf("%" SCNd32 "%" SCNd32 "%" SCNd32, &a, &b, &c);
	if (a+b > c) 
		if (b+c > a) { 
			printf ("%s", yesno[c+a > b]);
			return 0;
		}
	
	printf ("%s", yesno[0]);
	return 0;
}
