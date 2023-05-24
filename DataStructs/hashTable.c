#include "linkedList.h"

#define MAX_SIZE 1024

typedef struct ValueKeyPair
{
    int key;
    void *value;
} ValueKeyPair;

typedef struct Bucket
{
    Node *content;
    ValueKeyPair valueKeyPair; 
} Bucket;

typedef struct HashTable
{
    Bucket buckets[MAX_SIZE];
    size_t tableSize;
    size_t numOfElements;
} HashTable;

unsigned long createHashCode(char *key)
{
    unsigned long hash = 5381;
    int c = (int) key;
    size_t count = 0;

    while (count < strlen(key))
    {
        c =  (int) key[count];
        printf("Char: %c. ASCII value: %d\n", key[count], c);
        hash = ((hash << 5) + hash) + c;
        count++;
    }

    return hash % MAX_SIZE;
}

int main(void)
{
    unsigned long hash = createHashCode("bia");

    printf("Hash code: %lu\n", hash);
    printf("Index: %lu\n", hash % 4);

    return 0;
}