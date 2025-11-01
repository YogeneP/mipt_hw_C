#include <stdio.h>
#include <inttypes.h>

#define INPUT_LENGTH 3
#define YES 1
#define NO 0

int32_t val[INPUT_LENGTH];
char* yesno[] = { "NO\n", "YES\n" };

int main() {
	scanf("%" SCNd32, &val[0]);
	for (int i = 1; i < INPUT_LENGTH; i++) {
		scanf("%" SCNd32, &val[i]);
		if (val[i] <= val[i-1]) {
			printf ("%s", yesno[NO]);
			uint32_t c;
			while ((c = getchar()) != '\n');

			return 0;
		}
	}
	printf ("%s", yesno[YES]);

	return 0;
}
