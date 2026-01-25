#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h> 
#include <string.h>

typedef struct list {
	uint64_t address;
	size_t size;
	char comment[64];
	struct list *next;
} list;

uint64_t findMaxBlock(list *head) {
	uint8_t max_size = 0;
	uint64_t max_addr = 0; 
	printf("\n");
	while (head != NULL) {
		printf("%s: ", head->comment);
		if (head->size > max_size) {
			max_addr = head->address;
			max_size = head->size;
			printf(" - max (%" PRIu8 ") size, addr: %" PRIu64 "\n", max_size, max_addr);
		}
		head = head->next; 
	}
	return max_addr;
}

size_t totalMemoryUsage(list *head)	{
	size_t sum_size = 0;
	while (head != NULL) {
		sum_size += head->size;
		head = head->next; 
	}
	return sum_size;
}
/*
uint64_t findMaxBlock(list *head) {
	uint8_t max_size = 0;
	uint64_t max_addr = 0; 
	while (head != NULL) {
		if (head->size > max_size) {
			max_addr = head->address;
			max_size = head->size;
		}
		head = head->next; 
	}
	return max_addr;
}
*/


uint8_t push(list ** head, const char * comm) {
	list * new = malloc(sizeof(list));
	if (new == NULL) return 1;
	new->address = (uint64_t)new;
	strcpy(new->comment, comm);
	new->size = strlen(comm);
	new->next = *head; 
	*head = new;
	return 0;
 }

void fill_list(list ** head) {
	push(head, "f");
	push(head, "fo");
	push(head, "foo");
	push(head, "fooo");
	push(head, "foooo");
	push(head, "fooo");
	push(head, "foo");
	push(head, "fo");
	push(head, "f");		
}

uint8_t delete(list ** head, uint32_t n) {
	if(*head == NULL) return 1;
	list *curr = *head;

	if(n == 0) {
		*head = curr->next;
		free(curr);
		return 0;
	}
	
	for(int i = 0; i < n-1; i++) {
		if (curr->next == NULL) return 1;
		curr = curr->next;
	}
	
	list *tmp;
	tmp = curr->next;
	curr->next = (curr->next)->next;
	free(tmp);
	return 0;
}

void printList(list *head) {
	while (head) {
		printf("%s %"PRIu64"[%"PRIu8"]\n", head->comment, head->address, (uint8_t)(head->size));
		head = head->next;
	}
}

int main(void) {
	list* l = NULL;
	fill_list(&l);
	printList(l);
	printf("\n");
	delete(&l,3);
	printList(l); 
	printf("\n");
	list* max_block = (list*)findMaxBlock(l);
	printf("max block content: %s", max_block->comment);
	size_t total_size = totalMemoryUsage(l);
	printf("total blocks size: %zu", total_size);
	return 0;
}
