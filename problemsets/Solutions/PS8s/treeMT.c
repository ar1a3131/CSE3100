#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

Tree* makeEmptyTree()
{
    Tree* t = malloc(sizeof(Tree));
    t->root = NULL;
    pthread_mutex_init(&t->lock, NULL);
    return t;
}

void insertIntoTreeAux(TNode* par, enum Direction dir, TNode* nodeToInsert)
{
    TNode** childPointer;
    if(dir == Left)
        childPointer = &par->left;
    else
        childPointer = &par->right;

    if(*childPointer == NULL){
        *childPointer = nodeToInsert;    
        pthread_mutex_unlock(&par->lock);
    } else{
        TNode* child = *childPointer;
        pthread_mutex_lock(&child->lock);
        pthread_mutex_unlock(&par->lock);
        enum Direction newDir;
        if(nodeToInsert->val < child->val)
            newDir = Left;
        else if(nodeToInsert->val > child->val)
            newDir = Right;
        else{
            pthread_mutex_unlock(&child->lock);
            return;
        }
        insertIntoTreeAux(child, newDir, nodeToInsert);    
    }
}

void insertIntoTree(Tree* t, int val)
{
    TNode* node = malloc(sizeof(TNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    pthread_mutex_init(&node->lock, NULL);
    pthread_mutex_lock(&t->lock);
    if(t->root == NULL){
        t->root = node;
        pthread_mutex_unlock(&t->lock);
    } else{
        TNode* root = t->root;
        pthread_mutex_lock(&root->lock);
        pthread_mutex_unlock(&t->lock);
        if(val < root->val)
            insertIntoTreeAux(root, Left,  node);
        else if(val > root->val)
            insertIntoTreeAux(root, Right, node);
        else{
            free(node);
            pthread_mutex_unlock(&root->lock);
        }
    }
}

void printTreeAux(TNode* root)
{
    if (root == NULL)
        return;
    else {
        printf("(");
        printTreeAux(root->left);
        printf(" %d ",root->val);
        printTreeAux(root->right);
        printf(")");      
    }
}

void printTree(Tree* t)
{
    printTreeAux(t->root);
}

void destroyTreeAux(TNode* root)
{
    if(root != NULL){
        destroyTreeAux(root->left);
        destroyTreeAux(root->right);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
}

void destroyTree(Tree* t)
{
    destroyTreeAux(t->root);
    pthread_mutex_destroy(&t->lock);
    free(t);
}


