#include <stdio.h>

int A = 0;
int B = 0;
int C = 0;

int source_expression(int a, int b, int c) {
	return (!a || b) && !(a || b) && (!a || c);
}

int simple_expression(int a, int b, int c) {	
	return !(a || b);
}

void print_truth_table(int (*func)(int, int, int)) {
	printf("-----------------\n");
	printf("| A | B | C |res|\n");
	printf("-----------------\n");	
	for(int i=0; i<8; i++) {
		A = ((i >> 2) & 1);
		B = ((i >> 1) & 1);
		C = (i & 1);
		printf("| %d | %d | %d | %d |\n", A, B, C, func(A,B,C));
	}
	printf("-----------------\n\n");	
}

int main() {
	printf("Truth table for\nres = (!A->B) && !(A||B) && (!A||C):\n");
	print_truth_table(source_expression);

	printf("Truth table for\nres = !(A || B):\n");
	print_truth_table(simple_expression);

	return 0;
}
