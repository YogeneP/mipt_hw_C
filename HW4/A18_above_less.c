#include <stdio.h>
#include <inttypes.h>

int32_t a, b;
char* relation[] = { "Equal\n", "Above\n", "Less\n" };

int main() {
	scanf("%" SCNd32 "%" SCNd32, &a, &b);
	int32_t comp = a-b;
	printf ("%s", relation[(comp != 0) + (comp < 0)]); 
	//pure comparision-free alternative [((uint32_t)comp >> 31) + !!comp]

	
	return 0;
}
