#include <inttypes.h>

typedef struct list {
	uint64_t address;
	size_t size;
	char comment[64];
	struct list *next;
} list;

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

int main(void) {
	return 0;
}
