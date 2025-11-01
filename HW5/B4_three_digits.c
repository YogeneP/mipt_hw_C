#include <stdio.h>
#include <inttypes.h>

uint32_t a;
char* yesno[] = { "NO\n", "YES\n" };

int main () {
	scanf("%" SCNd32, &a);
	printf("%s\n", yesno[(a>=100 && a<=999)]);
	return 0;
} 
