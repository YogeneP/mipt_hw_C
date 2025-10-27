#include <stdio.h>

int A = 0;
int B = 0;

int equality(int a, int b) {
	return a==b;
}

int ab_expression(int a, int b) {	
	return ((a && b) || (!a && !b));
}

void print_truth_table(int (*func)(int, int)) {
	printf("-------------\n");
	printf("| A | B | C |\n");
	printf("-------------\n");	
	for(int i=0; i<4; i++) {
		A = ((i >> 1) & 1);
		B = (i & 1);	
		printf("| %d | %d | %d |\n", A, B, func(A,B));
	}
	printf("-------------\n\n");	
}

int main() {
	printf("Truth table C = A<->B:\n");
	print_truth_table(equality);

	printf("Truth table C = (A && B) || (!A && !B):\n");
	print_truth_table(ab_expression);

	return 0;
}
