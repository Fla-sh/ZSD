#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct AVL{
    struct AVL * up;
    struct AVL * left;
    struct AVL * right;
    int key;
    int balance;
};

struct AVL * createNode(int key){
    struct AVL * node = malloc(sizeof(struct AVL));
    node->up = NULL;
    node->left = NULL;
    node->right = NULL;
    node->key = key;
    node->balance = 0;
    return node;
}

void RR_rotate(struct AVL * root, struct AVL * A){
    struct AVL * B = A->right;
    struct AVL * A_parent = A->up;

    A->right = B->left;
    if(A->right != NULL) A->right->up = A;
    B->left = A;
    B->up = A_parent;
    A->up = B;
    if(A_parent != NULL){
        if(A_parent->left == A) A_parent->left = B;
        else A_parent->right = B;
    }
    else root = B;
    if(B->balance == -1){
        A->balance = 0;
        B->balance = 0;
    }
    else{
        A->balance = -1;
        B->balance = 1;
    }
}

void LL_rotate(struct AVL * root, struct AVL * A){
    struct AVL * B = A->left;
    struct AVL * A_parent = A->up;
    A->left = B->right;
    if(A->left != NULL) A->left->up = A;
    B->right = A;
    B->up = A_parent;
    A->up = B;
    if(A_parent != NULL){
        if(A_parent->left == A) A_parent->left = B;
        else A_parent->right = B;
    }
    else root = B;
    if(B->balance == 1){
        A->balance = 0;
        B->balance = 0;
    }
    else{
        A->balance = 1;
        B->balance = -1;
    }
}

void RL_rotate(struct AVL * root, struct AVL * A){
    struct AVL * B = A->right;
    struct AVL * C = B->left;
    struct AVL * p = A->up;
    B->left = C->right;
    if(B->left != NULL) B->left->up = B;
    A->right = C->left;
    if(A->right != NULL) A->right->up = A;
    C->left = A;
    C->right = B;
    A->up = C;
    B->up = C;
    C->up = p;
    if(p != NULL){
        if(p->left == A) p->left = C;
        else p->right = C;
    }
    else root = C;
    if(C->balance == -1) A->balance = 1;
    else A->balance = 0;
    if(C->balance == 1) B->balance = -1;
    else B->balance = 0;
    C->balance = 0;

}

void LR_rotation(struct AVL * root, struct AVL * A){
    struct AVL * B = A->left;
    struct AVL * C = B->right;
    struct AVL * p = A->up;
    B->right = C->left;
    if(B->right != NULL) B->right->up = B;
    A->left = C->right;
    if(A->left != NULL) A->left->up = A;
    C->right = A;
    C->left = B;
    A->up = C;
    B->up = C;
    C->up = p;
    if(p != NULL){
        if(p->left == A) p->left = C;
        else p->right = C;
    }
    else root = C;
    if(C->balance == 1) A->balance = -1;
    else A->balance = 0;
    if(C->balance == -1) B->balance = 1;
    else B->balance = 0;
    C->balance = 0;
}

int main() {
    struct AVL * tree;
    int key;
    scanf("%d", &key);
    tree = createNode(key);

    return 0;
}