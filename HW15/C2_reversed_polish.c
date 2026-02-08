#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

typedef struct mystack_t {
	int value;
	struct mystack_t* next;
} mystack_t; 

int push(mystack_t **stack, int value) {
	mystack_t *new = malloc(sizeof(mystack_t));
	if (new == NULL) return -1;
	new->value = value;
	new->next = *stack;
	*stack = new;
	return 1;
}
int pop(mystack_t **stack, int *value) {
	if (*stack == NULL) return -1;
	*value = (*stack)->value;
	mystack_t *temp_stack = *stack;
	*stack = (*stack)->next; 
	free(temp_stack); 
	return 1;
}	

int printstack(mystack_t *stack) {
	int count = 0;
	while (stack != NULL) {
		count++;
		printf("%d. %p: %d\n", count, stack, stack->value);
		stack = stack->next;
	}
	return count;
}

int cleanup(mystack_t **stack) {
	int count = 0;
	while (*stack != NULL) {
		count++;
		mystack_t *temp_stack = *stack;
//		printf("%d. %p: %d\n", count, *stack, (*stack)->value);
		*stack = temp_stack->next;
		free(temp_stack);
	}
	return count;
}

int main(void) {
	int o1;
	int o2;
	char op;

	mystack_t *stack = NULL;
	char c;
	int val = 0;

	c = getchar();
	while (c != '.' && c != '\n' && c != EOF) {
		while (c == ' ') { 
			c = getchar();	
		}

		val = -1;
		while (c <= '9' && c >= '0') {
			if (val < 0) { val = 0; }
			val = val*10 + (c - '0');
			c = getchar();
		}

		if (val >= 0) {
//			printf("%d ",val);
			push(&stack, val); 
			continue;
		}

//		printf("\n");
		if (c != '+' && c != '-' && c != '*' && c != '/') {
			printf("Parsing error\n");
			cleanup(&stack);
			return -1;
		}
				
		op = c;
//		printstack(stack);
		
		if (pop(&stack, &o2) < 0 || pop(&stack, &o1) < 0) {
			printf("Operands mismatch\n");
			cleanup(&stack);
			return -2;
		}

//		printf("Operation: %d %c %d\n", o1, op, o2);
		switch (op) {
			case '+':
				push(&stack, o1 + o2);
				break;
			case '-':
				push(&stack, o1 - o2);
				break;
			case '*':
				push(&stack, o1 * o2);
				break;
			case '/':
				push(&stack, o1 / o2);
				break;
			default: 
				break;
		}
		c = getchar();
	}
	printf("%d\n", stack->value);
	cleanup(&stack);
	return 0;
}
