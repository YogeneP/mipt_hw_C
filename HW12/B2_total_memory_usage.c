#include <inttypes.h>

typedef struct list {
	uint64_t address;
	size_t size;
	char comment[64];
	struct list *next;
} list;

size_t totalMemoryUsage(list *head)	{
	size_t sum_size = 0;
	while (head != NULL) {
		sum_size += head->size;
		head = head->next; 
	}
	return sum_size;
}

int main(void) {
	return 0;
}
