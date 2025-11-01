#include <stdio.h>
#include <inttypes.h>

char* seasons[4] = { "winter", "spring", "summer", "autumn" };

int main() {
	uint8_t month = 0;

	scanf("%" SCNd8, &month);
	if (month < 1 || month > 12) return -1;
	if (month == 12) month = 0; 
	printf("%s", seasons[month/3]);

	return 0;
} 
