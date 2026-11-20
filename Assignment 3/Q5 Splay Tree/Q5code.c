#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node* createNode(int key) {
    Node *n = (Node*)malloc(sizeof(Node));

    n->key = key;
    n->left = n->right = NULL;

    return n;
}

Node* rightRotate(Node *x) {
    Node *y = x->left;

    x->left = y->right;
    y->right = x;

    return y;
}

Node* leftRotate(Node *x) {
    Node *y = x->right;

    x->right = y->left;
    y->left = x;

    return y;
}

Node* splay(Node *root, int key) {
    if (root == NULL || root->key == key)
        return root;

    // Zig-Zig Left
    if (key < root->key && root->left) {

        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }

        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);

            if (root->left->right)
                root->left = leftRotate(root->left);
        }

        return root->left ? rightRotate(root) : root;
    }

    // Zig-Zig Right
    if (key > root->key && root->right) {

        if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        else if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);

            if (root->right->left)
                root->right = rightRotate(root->right);
        }

        return root->right ? leftRotate(root) : root;
    }

    return root;
}

Node* insert(Node *root, int key) {
    if (root == NULL)
        return createNode(key);

    root = splay(root, key);

    if (root->key == key)
        return root;

    Node *newNode = createNode(key);

    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    }
    else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    return newNode;
}

void preorder(Node *root) {
    if (root) {
        printf("%d ", root->key);
        preorder(root->left);
        preorder(root->right);
    }
}

int main() {
    int keys[] = {10, 5, 20, 3, 9, 15};

    Node *root = NULL;

    for (int i = 0; i < 6; i++)
        root = insert(root, keys[i]);

    printf("Before access:\n");
    preorder(root);

    root = splay(root, 3);

    printf("\n\nAfter accessing 3:\n");
    preorder(root);

    root = splay(root, 9);

    printf("\n\nAfter accessing 9:\n");
    preorder(root);

    printf("\n");

    return 0;
}