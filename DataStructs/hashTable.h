#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "linkedList.h"

#define MAX_TABLE_SIZE 500
#define MEMORY_CAP 6400
#define HASH_SEED 9973

typedef unsigned long ulong;

typedef struct KeyValuePair
{
    void *value;
    char *key;
} KeyValuePair;

typedef struct Bucket
{
    LinkedList *content;
} Bucket;

typedef struct HashTable
{
    Bucket buckets[MAX_TABLE_SIZE];
    size_t keyCount;
    size_t uniqueKeys;
} HashTable;

uint32_t rotl32(uint32_t x, int r);
uint32_t MurmurHashAligned2(const void *key, int32_t len, uint32_t seed);
void set(HashTable *table, char *key, void *value);
void *getByKey(HashTable *table, char *key);
void freeHashTable(HashTable *table);

#endif /* HASHTABLE_H */
