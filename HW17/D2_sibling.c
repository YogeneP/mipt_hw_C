typedef int datatype;

typedef struct tree {
datatype key;
struct tree *left, *right;
struct tree *parent; //ссылка на родителя
} tree;

tree * findBrother(tree *root, int key) {
    tree * sibling;
    if (!root) {
        return 0;
    }
    if (key < root->key) { 
        sibling = findBrother(root->left, key);
        return sibling;
    }
    if (key > root->key) {
        sibling = findBrother(root->right, key);
        return sibling;
    }
    if(key == root->key) {
        if(!root->parent) {
            return 0;
        }
        sibling = (root->parent->key > root->key) ? root->parent->right : root->parent->left;
        return sibling;
    }
}

int main(void) {
    return 0;
}