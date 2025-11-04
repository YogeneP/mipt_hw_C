#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int32_t a = 0;

int main() {
	scanf("%" SCNd32, &a);
	a = abs(a);
	while (a > 0) {
		if ((a % 10) % 2) {
			printf("%s", "NO\n");
			return 0;
		}
		a /= 10;
	}

	printf("%s", "YES\n");
 	return 0;
}
