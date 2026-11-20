#include <stdio.h>
#include <stdlib.h>

#define T 2

typedef struct BTreeNode {
    int keys[2 * T - 1];
    struct BTreeNode *child[2 * T];
    int n;
    int leaf;
} BTreeNode;

BTreeNode* createNode(int leaf) {
    BTreeNode *node = (BTreeNode*)malloc(sizeof(BTreeNode));

    node->leaf = leaf;
    node->n = 0;

    for (int i = 0; i < 2 * T; i++)
        node->child[i] = NULL;

    return node;
}

void traverse(BTreeNode *root) {
    if (root == NULL)
        return;

    int i;

    for (i = 0; i < root->n; i++) {
        if (!root->leaf)
            traverse(root->child[i]);

        printf("%d ", root->keys[i]);
    }

    if (!root->leaf)
        traverse(root->child[i]);
}

void search(BTreeNode *root, int key) {
    if (root == NULL) {
        printf("%d not found\n", key);
        return;
    }

    int i = 0;

    while (i < root->n && key > root->keys[i])
        i++;

    if (i < root->n && key == root->keys[i]) {
        printf("%d found\n", key);
        return;
    }

    if (root->leaf) {
        printf("%d not found\n", key);
        return;
    }

    search(root->child[i], key);
}

void splitChild(BTreeNode *parent, int i, BTreeNode *full) {
    BTreeNode *newNode = createNode(full->leaf);

    newNode->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        newNode->keys[j] = full->keys[j + T];

    if (!full->leaf) {
        for (int j = 0; j < T; j++)
            newNode->child[j] = full->child[j + T];
    }

    full->n = T - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->child[j + 1] = parent->child[j];

    parent->child[i + 1] = newNode;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = full->keys[T - 1];
    parent->n++;
}

void insertNonFull(BTreeNode *node, int key) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->n++;
    }
    else {
        while (i >= 0 && key < node->keys[i])
            i--;

        i++;

        if (node->child[i]->n == 2 * T - 1) {
            splitChild(node, i, node->child[i]);

            if (key > node->keys[i])
                i++;
        }

        insertNonFull(node->child[i], key);
    }
}

void insert(BTreeNode **root, int key) {
    if (*root == NULL) {
        *root = createNode(1);
        (*root)->keys[0] = key;
        (*root)->n = 1;
        return;
    }

    BTreeNode *r = *root;

    if (r->n == 2 * T - 1) {
        BTreeNode *newRoot = createNode(0);

        newRoot->child[0] = r;

        splitChild(newRoot, 0, r);

        int i = 0;

        if (key > newRoot->keys[0])
            i++;

        insertNonFull(newRoot->child[i], key);

        *root = newRoot;
    }
    else {
        insertNonFull(r, key);
    }
}

int main() {
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};

    BTreeNode *root = NULL;

    for (int i = 0; i < 8; i++)
        insert(&root, keys[i]);

    printf("B-Tree traversal: ");
    traverse(root);

    printf("\n");

    search(root, 12);
    search(root, 25);

    return 0;
}