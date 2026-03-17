#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define WORD_SIZE 21

typedef enum {LEFT = 1, RIGHT} direct_t;
typedef int datatype;

typedef struct tree {
    datatype key;        
    struct tree *left;
    struct tree *right;
} tree;

void initCleanup(void);
void cleanup(void);

int addBranch(tree *tree, datatype key, direct_t dir) {
    tree *new_branch = malloc(sizeof(tree));
    switch(dir) {
        case LEFT:
            tree->left = new_branch;
            break;
        case RIGHT:
            tree->right = new_branch;
            break;
        default:
            free(new_branch);
            return -1;
    }
    new_branch->key = key;
    return 0;
}

uint16_t getTreeWidth(tree *tree) {
    
}

void printTree(tree *tree) {
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