#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// THE PARAGRAPH BELOW IS TO EXPLAIN THE LOGIC. IF YOU DO NOT NEED THE LOGIC, YOU DO NOT NEED TO READ IT.

// This program implements a binary search tree with thread safety, where each node in the tree is 
// protected by an individual mutex lock. The tree is designed to store integer values, and the implementation 
// includes functions for creating an empty tree, inserting values into the tree, printing the tree in a 
// parenthesized format, and destroying the tree while releasing all allocated memory. 
// The use of mutex locks ensures that concurrent threads can safely perform operations on 
// the tree without conflicting with each other. The 'insertIntoTreeAux' function is a helper 
// function used for recursively inserting nodes into the tree based on the specified 
// direction (Left or Right). The program employs a modular structure with separate functions 
// for each operation, contributing to a clear and organized implementation. Overall, the 
// program provides a foundation for a concurrent binary search tree, allowing for safe and synchronized 
// operations in a multi-threaded environment.


// Structure definition for a binary tree node
typedef struct TreeNode {
    int val;                // Value of the node
    struct TreeNode* left;  // Pointer to the left child
    struct TreeNode* right; // Pointer to the right child
    pthread_mutex_t lock;   // Mutex lock for synchronization
} TNode;

// Enumeration to represent the direction (Left or Right) in the tree
enum Direction {
    Left,
    Right
};

// Structure definition for a binary tree
typedef struct {
    TNode* root;           // Pointer to the root of the tree
    pthread_mutex_t lock;  // Mutex lock for synchronization
} Tree;

// Function to create an empty binary tree
Tree* makeEmptyTree() {
    Tree* t = malloc(sizeof(Tree));     // Allocate memory for the tree structure
    t->root = NULL;                     // Initialize the root pointer to NULL
    pthread_mutex_init(&t->lock, NULL); // Initialize the tree's mutex lock
    return t;
}

// Helper function to insert a node into the binary tree
void insertIntoTreeAux(TNode* par, enum Direction dir, TNode* nodeToInsert) {
    // Determine the child pointer based on the specified direction (Left or Right)
    TNode** childPointer;
    if (dir == Left) {
        childPointer = &par->left;
    } else {
        childPointer = &par->right;
    }
    // Check if the child pointer is NULL, i.e., there is no existing child in the specified direction
    if (*childPointer == NULL) {
        *childPointer = nodeToInsert;       // Set the child pointer to the new node
        pthread_mutex_unlock(&par->lock);   // Unlock the parent node

    } else {    // If there is an existing child, lock the child's mutex and unlock the parent's mutex
        
        TNode* child = *childPointer;        // Get the child node
        pthread_mutex_lock(&child->lock);    // Lock the child node
        pthread_mutex_unlock(&par->lock);    // Unlock the parent node

        // Determine the new direction based on the value of the node to insert
        enum Direction newDir;
        if (nodeToInsert->val < child->val)       // If the value is less than the current child's value, go to the left
            newDir = Left;
        else if (nodeToInsert->val > child->val)  // If the value is greater than the current child's value, go to the right
            newDir = Right;
        else {
            pthread_mutex_unlock(&child->lock);   // If the value already exists, unlock the child and return
            return;
        }
        insertIntoTreeAux(child, newDir, nodeToInsert); // Recursive call for inserting into the subtree
    }
}

// Function to insert a value into the binary tree
void insertIntoTree(Tree* t, int val) {
    TNode* node = malloc(sizeof(TNode));    // Allocate memory for the new node
    node->val = val;                        // Set the value of the new node
    node->left = NULL;
    node->right = NULL;
    pthread_mutex_init(&node->lock, NULL);  // Initialize the node's mutex lock
    pthread_mutex_lock(&t->lock);           // Lock the tree's mutex
    if (t->root == NULL) {
        t->root = node;                     // If the tree is empty, set the root to the new node
        pthread_mutex_unlock(&t->lock);     // Unlock the tree's mutex
    } else {
        TNode* root = t->root;           // Get the root node
        pthread_mutex_lock(&root->lock); // Lock the root node
        pthread_mutex_unlock(&t->lock);  // Unlock the tree's mutex
        if (val < root->val)
            insertIntoTreeAux(root, Left, node);  // Insert into the left subtree
        else if (val > root->val)
            insertIntoTreeAux(root, Right, node); // Insert into the right subtree
        else {
            free(node);                         // If the value already exists, free the new node
            pthread_mutex_unlock(&root->lock);  // Unlock the root node
        }
    }
}

// Helper function to recursively print the binary tree in parentheses format
void printTreeAux(TNode* root) {
    if (root == NULL)
        return;
    else {
        printf("(");
        printTreeAux(root->left);
        printf(" %d ", root->val);
        printTreeAux(root->right);
        printf(")");
    }
}

// Function to print the binary tree
void printTree(Tree* t) {
    printTreeAux(t->root);
}

// Helper function to recursively destroy the binary tree and release memory
void destroyTreeAux(TNode* root) {
    if (root != NULL) {
        destroyTreeAux(root->left);
        destroyTreeAux(root->right);
        pthread_mutex_destroy(&root->lock); // Destroy the node's mutex lock
        free(root);                         // Free memory allocated for the node
    }
}

// Function to destroy the binary tree and release all associated memory
void destroyTree(Tree* t) {
    destroyTreeAux(t->root);
    pthread_mutex_destroy(&t->lock); // Destroy the tree's mutex lock
    free(t);                         // Free memory allocated for the tree
}
