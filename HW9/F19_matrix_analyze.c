#include <stdio.h>

#define M_SIZE 5

int matrix[M_SIZE][M_SIZE];

int get_footprint_average(int s, int m[s][s]);
int analyze_matrix(int s, int m[s][s], int aver);

int main() {
	for (int i = 0; i < M_SIZE; i++) {
		for (int j = 0; j < M_SIZE; j++) {
			scanf("%d", &matrix[i][j]);
		}
	}
	printf("%d ", analyze_matrix(M_SIZE, matrix, get_footprint_average(M_SIZE, matrix)));
}

int get_footprint_average(int s, int m[s][s]) {
	int fp = m[0][0];
	
	for (int i = 1; i < s; i++) {
		fp += m[i][i]; 
	}	
	return fp/s;
}

int analyze_matrix(int s, int m[s][s], int aver) {
	int n = 0;
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			if (matrix[i][j] > aver) n++;
		}
	}
	return n;
}
