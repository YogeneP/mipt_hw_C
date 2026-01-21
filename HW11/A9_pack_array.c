#include <stdio.h>
#include <inttypes.h>

#define BYTE_BITS 8
#define BITS 32
typedef struct {
	uint32_t array; // поле для хранения упакованного массива из 0 и 1
	uint32_t count0 : 8; // счетчик нулей в array
	uint32_t count1 : 8; // счетчик единиц в array
} pack_array; 

void array2struct(int [], pack_array *);

int main(void) {
	pack_array pa = {
		.array = 0,
		.count0 = 0,
		.count1 = 0
	};
	int a[BITS] = { 0 };
	for (int i = 0; i < BITS; i++) {
		scanf("%" SCNu32, a + i);
	}
	array2struct(a, &pa);

	printf("%" PRIu32 " %" PRIu32 " %" PRIu32, pa.array, pa.count0, pa.count1);
	return 0;
}

void array2struct(int a[], pack_array * pa) {
	int i = 0;
	while(1) {
		if (a[i]) {
			pa->array += !!a[i];
			pa->count1++;
		} else {
			pa->count0++;
		}
		if (i >= BITS - 1) break;
		pa->array <<= 1;
		i++;
	}
}
