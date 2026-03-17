#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>


#define WORD_SIZE 21

typedef struct list_t {
char word[WORD_SIZE];
struct list_t *next;
} list_t;

void initCleanup(void);
void cleanup(void);

int addNode(list_t **list, char word[]) 
{
    if (strlen(word) > (WORD_SIZE - 1)) return -1;
    list_t *new_node = malloc(sizeof(list_t));
    if (!new_node) return -2;
    new_node->next = *list;
    *list = new_node;
    strcpy((*list)->word, word);
    return 0;
}

void swapNodes(list_t *node1, list_t *node2) { // swaps content only
    if(node1 == node2) { return; }
    char temp[WORD_SIZE];
    strcpy(temp,node2->word);
    strcpy(node2->word,node1->word);
    strcpy(node1->word,temp);
} 

int compareNodes(list_t *node1, list_t *node2) {
    return strcmp(node1->word,node2->word); 
}

void sortList(list_t *list) {
    list_t *node_min;
    list_t *node = list;
    list_t *cur_node;
    while (node->next) {
        cur_node = node;
        node_min = node;
        while(cur_node = cur_node->next) {
//            printf("comparing min: \"%s\" and \"%s\": ", node_min->word, cur_node->word);
            if(compareNodes(cur_node,node_min) < 0) {
                node_min = cur_node;
//                printf("new min: \"%s\"\n", node_min->word);
                continue;
            }
//            printf("keeping old min \"%s\"\n", node_min->word);
        }
//        printf("swapping: \"%s\" and \"%s\"\n", node_min->word, node->word);
        swapNodes(node,node_min);
        node = node->next;
    }
}

void printList(list_t *list) {
    list_t *node = list;
    while (node) {
        printf ("%s ", node->word);
        node = node->next;
    }
}

list_t* words=NULL;

int main (void) {
    initCleanup();
    char* line = NULL;
    size_t line_len = 0;
    char word[WORD_SIZE] = {0};
//   printf("zeroing next ref... \n");
    uint8_t i = 0;
    char c;
//    printf("getting string: \n");
    do {
        c = getchar();
        if (c == ' ' || c == '.' || c == '\n' || c == EOF) {
            if(i > 0) {
                word[i] = '\0';
                addNode(&words, word);
                word[0] = '\0';
                i = 0;
            }            
            continue;
        }
        word[i] = c;
        i++;
    } while (c != '.' && c != '\n' && c != EOF);
    sortList(words);
    printList(words);
    exit(0);
}

void initCleanup() {
    if(atexit(cleanup) != 0) {
        printf("Memory management error!");
        exit(-1);
    }
}

void cleanup() {
    list_t* next;
    do {
        next = words->next;
        free(words);
        words = next;
    } while(words);
}