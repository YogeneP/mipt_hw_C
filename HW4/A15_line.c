#include <stdio.h>
#include <inttypes.h>

int32_t x_1 = 0, y_1 = 0;
int32_t x_2 = 0, y_2 = 0;

int main() {
float k = 0, b = 0;
scanf("%" SCNd32 "%" SCNd32 "%" SCNd32 "%" SCNd32, &x_1, &y_1, &x_2, &y_2);
k = ((float)(y_2 - y_1))/(x_2 - x_1);
b = y_1 - k * x_1;
printf("%.2f %.2f", k, b);
	
}
