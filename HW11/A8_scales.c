#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#define BOWL_CAPACITY 1000000
//3^13 > BOWL_CAPACITY
#define MAX_STEP 12  

// #define DEBUG

#ifdef DEBUG
#define D if(1)
#else 
#define D if(0)
#endif

int main(void) {
	uint32_t w;
	uint8_t count = 0;

	
	scanf("%" SCNu32, &w);
	int32_t bal = -w; 
	uint8_t rem = 0;
	int8_t e = 0;
	uint8_t step = 0;
D	{ printf( "weight: %" PRIu32 " on the LEFT bowl, balance %" PRId32 " \n", w, bal);  } 
	if (w > BOWL_CAPACITY) { 
			printf ("-1"); 
			return 0; 
	};
	while (w > 0) {
		rem = w % 3;
		count += !!rem; //add 1 if rem != 0;
		e = (rem - 3 * (rem >> 1)); // here is a branchless transfer function { 0 -> 0, 1 -> 1, 2 -> -1 }
		w = (w - e) / 3; 
		if (step > MAX_STEP) { 
			printf ("-1"); 
			return 0; 
		};
D 		{ printf("%" PRIu8 ". w=%" PRIu32 "; rem=%" PRIu8 " -> a = %d\n", step, w, rem, e); }
D		{ bal += (uint32_t)pow(3.0,(double)step) * (rem ? (rem > 1 ? -1 : 1) : 0); }
D		{ printf( "weight: %" PRIu32 " -> %s ; balance %" PRId32 "\n", (uint32_t)pow(3.0,(double)step), rem ? (rem < 2 ? "RIGHT" : "LEFT") : "NONE", bal); }    
		step++;
	}
	printf("%" PRIu8, count);
	return 0;
}
