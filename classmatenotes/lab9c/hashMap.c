#include "hashMap.h"
#include "dllist.h"
#include <string.h>
#include <stdio.h>

// THE PARAGRAPH BELOW IS TO EXPLAIN THE LOGIC. IF YOU DO NOT NEED THE LOGIC, YOU DO NOT NEED TO READ IT.

// This program implements a basic hash map with a focus on thread safety using locks. The hash map is designed to 
// store key-value pairs, and the implementation employs a polynomial rolling hash function to compute hash values 
// for the keys. The hash map consists of an array of buckets, each protected by its own mutex lock to ensure 
// thread-safe access. The program includes functions for initializing and destroying the hash map, inserting 
// key-value pairs, retrieving values based on keys, and deleting entries. The use of mutex locks prevents race 
// conditions and ensures that multiple threads can safely access and modify the hash map concurrently. 
// The implementation follows a modular structure with separate files for the doubly linked list (DLL) and 
// the hash map. Overall, this program provides a foundation for a concurrent hash map, where the 
// safety of operations is guaranteed in a multi-threaded environment.

// Computes the hash value corresponding to the key using the polynomial rolling hash function
unsigned long long hash(char* key) {
    const int p = 31;
    const int m = 1e9 + 9;
    unsigned long long hash_value = 0;
    long long p_pow = 1;

    // Iterate over each character in the key
    for (int i = 0; i < strlen(key); i++) {
        char c = key[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }

    return hash_value;
}

// Initialize the hash map with a specified number of buckets
void initHashMap(HashMap* hm, unsigned int nb) {
    // Set the numBuckets and numItems fields of hm. Initially, the hash map has 0 items.
    // Allocate memory for nb buckets, allocate memory for a DLList for each bucket, 
    // initialize the list, and initialize the bucket's lock.
    // Initialize the tableLock.
    hm->numBuckets = nb;
    hm->numItems = 0;

    // Allocate memory for the array of buckets
    hm->buckets = (Bucket*)calloc(nb, sizeof(Bucket));

    for (unsigned int i = 0; i < nb; i++) {
        // Initialize the lock for each bucket
        pthread_mutex_init(&hm->buckets[i].lock, NULL);

        // Allocate memory for and initialize a DLList for each bucket
        hm->buckets[i].list = malloc(sizeof(DLList));
        initList(hm->buckets[i].list);
    }

    // Initialize the lock for the entire hash map
    pthread_mutex_init(&hm->tableLock, NULL);
}

// Destroy the hash map, freeing up each bucket and the hash map. Destroy all the mutexes.
void destroyHashMap(HashMap* hm) {
    for (unsigned int i = 0; i < hm->numBuckets; i++) {
        // Destroy the list within each bucket
        destroyList(hm->buckets[i].list);
        free(hm->buckets[i].list);

        // Destroy the lock for each bucket
        pthread_mutex_destroy(&hm->buckets[i].lock);
    }

    // Destroy the lock for the entire hash map
    pthread_mutex_destroy(&hm->tableLock);

    // Free memory allocated for the array of buckets
    free(hm->buckets);

    // Set the numBuckets and numItems fields to zero
    hm->numBuckets = 0;
    hm->numItems = 0;
}

// Insert a key-value pair into the hash map
void insertHashMap(HashMap* hm, char* key, char* value) {
    unsigned int h = hash(key) % hm->numBuckets;

    // Go to bucket h, and insert the (key, value) pair, unless the key is already in the hash map.
    // Note that only one thread may access bucket h at any point in time.
    // If inserted, increment the hash map's numItems field under mutex protection.
    int inserted = 0;
    pthread_mutex_lock(&hm->buckets[h].lock);
    inserted = insertSortedList(hm->buckets[h].list, key, value);
    pthread_mutex_unlock(&hm->buckets[h].lock);

    if (inserted) {
        pthread_mutex_lock(&hm->tableLock);
        hm->numItems++;
        pthread_mutex_unlock(&hm->tableLock);
    }
}

// Retrieve the value corresponding to the key from the hash map
char* retrieveValue(HashMap* hm, char* key) {
    unsigned int h = hash(key) % hm->numBuckets;

    // Find the key in bucket h, under mutex protection.
    // Since the bucket's key-value pairs are stored in a DLList, look at the DLList.h file for an appropriate function to get the value corresponding to the key.
    pthread_mutex_lock(&hm->buckets[h].lock);
    char* val = getValue(hm->buckets[h].list, key);
    pthread_mutex_unlock(&hm->buckets[h].lock);

    return val;
}

// Delete the entry with the specified key from the hash map
void deleteEntry(HashMap* hm, char* key) {
    unsigned int h = hash(key) % hm->numBuckets;

    // Delete the key from bucket h, under mutex protection. If the key isn't in the bucket, don't do anything.
    // If the key was successfully removed from the bucket, decrement the hashmap's numItems field.
    pthread_mutex_lock(&hm->buckets[h].lock);
    int removed = removeFromList(hm->buckets[h].list, key);
    pthread_mutex_unlock(&hm->buckets[h].lock);

    if (removed) {
        pthread_mutex_lock(&hm->tableLock);
        hm->numItems--;
        pthread_mutex_unlock(&hm->tableLock);
    }
}
