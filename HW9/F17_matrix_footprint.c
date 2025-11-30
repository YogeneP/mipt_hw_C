#include <stdio.h>

#define M_SIZE 5

int matrix[M_SIZE][M_SIZE] = {0};

int get_footprint(int s, int m[s][s]);

int main() {
	for (int j = 0; j < M_SIZE; j++) {
		for (int i = 0; i < M_SIZE; i++) {
			scanf("%d", &matrix[j][i]);
		}
	}
	printf("%d ", get_footprint(M_SIZE, matrix));
}

int get_footprint(int s, int m[s][s]) {
	int fp = m[0][0];
	
	for (int i = 1; i < M_SIZE; i++) {
		fp += m[i][i]; 
	}	
	return fp;
}


