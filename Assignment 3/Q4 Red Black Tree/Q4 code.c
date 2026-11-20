#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct Node {
    int key;
    int color;
    struct Node *left, *right, *parent;
} Node;

Node *root = NULL;

Node* createNode(int key) {
    Node *n = (Node*)malloc(sizeof(Node));

    n->key = key;
    n->color = RED;
    n->left = n->right = n->parent = NULL;

    return n;
}

void leftRotate(Node *x) {
    Node *y = x->right;

    x->right = y->left;

    if (y->left)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(Node *y) {
    Node *x = y->left;

    y->left = x->right;

    if (x->right)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void fixInsert(Node *z) {
    while (z != root && z->parent->color == RED) {

        Node *p = z->parent;
        Node *g = p->parent;

        if (p == g->left) {
            Node *u = g->right;

            if (u && u->color == RED) {
                p->color = BLACK;
                u->color = BLACK;
                g->color = RED;
                z = g;
            }
            else {
                if (z == p->right) {
                    z = p;
                    leftRotate(z);
                }

                p->color = BLACK;
                g->color = RED;
                rightRotate(g);
            }
        }
        else {
            Node *u = g->left;

            if (u && u->color == RED) {
                p->color = BLACK;
                u->color = BLACK;
                g->color = RED;
                z = g;
            }
            else {
                if (z == p->left) {
                    z = p;
                    rightRotate(z);
                }

                p->color = BLACK;
                g->color = RED;
                leftRotate(g);
            }
        }
    }

    root->color = BLACK;
}

void insert(int key) {
    Node *z = createNode(key);
    Node *y = NULL;
    Node *x = root;

    while (x != NULL) {
        y = x;

        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == NULL)
        root = z;
    else if (key < y->key)
        y->left = z;
    else
        y->right = z;

    fixInsert(z);
}

void inorder(Node *root) {
    if (root) {
        inorder(root->left);

        printf("%d(%s) ",
               root->key,
               root->color == RED ? "R" : "B");

        inorder(root->right);
    }
}

int main() {
    int keys[] = {10, 20, 30, 15, 25, 5};

    for (int i = 0; i < 6; i++)
        insert(keys[i]);

    printf("Red-Black Tree Inorder:\n");
    inorder(root);

    printf("\n");

    return 0;
}