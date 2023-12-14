#include "dllist.h"
#include <stdio.h>
#include <string.h>

// THE PARAGRAPH BELOW IS TO EXPLAIN THE LOGIC. IF YOU DO NOT NEED THE LOGIC, YOU DO NOT NEED TO READ IT.

// This program defines a doubly linked list (DLL) and provides functions for initializing, destroying, 
// searching, inserting, and removing elements in a sorted manner. The DLL is designed to store key-value pairs 
// in a sorted order based on the keys. The program employs a structure (DLNode) to represent each node in the DLL, 
// with pointers to the next and previous nodes. The main functions include initList for initializing an empty list, 
// destroyList for freeing the allocated memory, getValue to retrieve the value corresponding to a given key, 
// insertSortedList to insert a key-value pair in sorted order, removeFromList to remove a key-value pair, and 
// printListForward to print the key-value pairs in ascending order. The insertion and removal functions maintain the 
// sorted order of the DLL. Memory management is crucial, ensuring proper allocation and deallocation of memory for keys,
// values, and nodes.

// Initialize the doubly linked list
void initList(DLList* list) {
    list->head = list->tail = NULL; // Set both head and tail pointers to NULL as the list is empty
}

// Destroy the doubly linked list. Recursively free all nodes from head to tail.
void destroyList(DLList* list) {
    DLNode* cur = list->head;
    while (cur) {
        DLNode* next = cur->next;   // Save the reference to the next node
        free(cur->key);             // Free memory allocated for the key
        free(cur->value);           // Free memory allocated for the value
        free(cur);                  // Free memory allocated for the current node
        cur = next;                 // Move to the next node
    }
    list->head = list->tail = NULL; // Set both head and tail pointers to NULL as the list is empty
}

// Return the value corresponding to the input key from the list. If the key doesn't exist, return NULL.
char* getValue(DLList* list, char* key) {
    DLNode* cur = list->head;
    // Traverse the list until the key is found or until we reach a key greater than the target key
    while (cur && strcmp(cur->key, key) < 0)
        cur = cur->next;
    if (cur && strcmp(cur->key, key) == 0)
        return cur->value; // Return the value if the key is found
    else
        return NULL;       // Return NULL if the key is not found
}

// Insert the key-value pair in a sorted manner
int insertSortedList(DLList* list, char* key, char* val) {
    DLNode* cur = list->head;
    // Traverse the list until the key is found or until we reach a key greater than the target key
    while (cur && strcmp(cur->key, key) < 0)
        cur = cur->next;
    if (cur && strcmp(cur->key, key) == 0)
        return 0; // Key already exists, return 0 indicating failure to insert

    // Allocate memory for the new node and its key and value
    DLNode* nn = malloc(sizeof(DLNode));

    nn->value = (char*)malloc(sizeof(char) * (1 + strlen(val)));
    nn->key = (char*)malloc(sizeof(char) * (1 + strlen(key)));

    strcpy(nn->value, val); // Copy the value to the new node
    strcpy(nn->key, key);   // Copy the key to the new node

    if (cur) {
        // We found the spot: before `cur`
        DLNode* prev = cur->prv; // Save the reference to the previous node
        DLNode* nxt = cur;       // Save the reference to the next node

        nn->prv = prev;   // Set the previous pointer of the new node
        nn->next = cur;   // Set the next pointer of the new node to the current node

        if (prev) prev->next = nn; // Update the next pointer of the previous node to the new node
        else
            list->head = nn;  // If there is no previous node, the new node becomes the head
        nxt->prv = nn;        // Update the previous pointer of the current node to the new node
    } else {
        // The list is empty or the new node goes at the end

        nn->next = NULL;         // The next pointer of the new node is NULL as it is the last node
        nn->prv = list->tail;    // The previous pointer of the new node is set to the current tail

        if (list->tail) list->tail->next = nn; // If there is a tail, update its next pointer to the new node
        else
            list->head = nn;  // If there is no tail, the new node becomes the head
        list->tail = nn;      // The new node becomes the tail
    }
    return 1; // Successful insertion, return 1
}

// Remove the key-value pair from the list
int removeFromList(DLList* list, char* key) {
    DLNode* cur = list->head;
    // Traverse the list until the key is found or until we reach a key greater than the target key
    while (cur && strcmp(cur->key, key) < 0)
        cur = cur->next;
    if (cur) {
        // Found the node with the key
        if (cur->prv)
            cur->prv->next = cur->next; // Update the next pointer of the previous node
        else
            list->head = cur->next; // If there is no previous node, update the head pointer
        if (cur->next)
            cur->next->prv = cur->prv; // Update the previous pointer of the next node
        else
            list->tail = cur->prv;  // If there is no next node, update the tail pointer
        free(cur->key);             // Free memory allocated for the key
        free(cur->value);           // Free memory allocated for the value
        free(cur);                  // Free memory allocated for the current node
        return 1;                   // Successful removal, return 1
    }
    return 0; // Key not found, return 0 indicating failure to remove
}

// Print the key-value pairs in sorted order
void printListForward(DLList* list) {
    DLNode* cur = list->head;
    while (cur) {
        printf("(%s, %s) ", cur->key, cur->value); // Print the key-value pair
        cur = cur->next; // Move to the next node
    }
    printf("\n");
}
