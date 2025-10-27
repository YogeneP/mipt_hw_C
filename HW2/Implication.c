#include <stdio.h>

int A = 0;
int B = 0;

int main() {
	printf("Truth table A->B:\n");
	printf("----------------\n");
	printf("| A | B | A->B |\n");
	printf("| 0 | 0 |  1   |\n");
	printf("| 0 | 1 |  1   |\n");
	printf("| 1 | 0 |  0   |\n");
	printf("| 1 | 1 |  1   |\n");
	printf("----------------\n\n");
	printf("Truth table !A||B:\n");
	printf("-----------------\n");
	printf("| A | B | !A||B |\n");
	printf("-----------------\n");

	for(int i=0; i<4; i++) {
		A = ((i >> 1) & 1);
		B = (i & 1);	
		printf("| %d | %d |   %d   |\n", A, B, !A||B );
	}
	printf("-----------------\n");
	return 0;
}
