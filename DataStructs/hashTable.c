#include <stdio.h>
#include "linkedList.h  "

typedef struct ValueKeyPair
{
    int key;
    void *value
} ValueKeyPair;

typedef struct HashTable
{
    Node *buckets;
} HashTable;

char *createHashCode(char *key)
{
    return key % 30;
}

int main(void)
{
   char *hash = createHashCode("marco");

    printf("Hash code: %s\n", hash);

    return 0;
}