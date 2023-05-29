#include <stdint.h>
#include <inttypes.h>
#include "linkedList.h"

#define MAX_TABLE_SIZE 500
#define MEMORY_CAP 6400

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

// ulong createHashCode(char *key)
// {
//     unsigned long hash = 5381;
//     size_t keyCount = 0;

//     while (keyCount < strlen(key))
//     {
//         int c = (int)key[keyCount];
//         hash = ((hash << 5) + hash) + c;
//         keyCount++;
//     }

//     return hash;
// }

// size_t getIndexFromKey(HashTable *table, char *key)
// {
//     ulong hash = createHashCode(key);

//     // int index;

//     // if (table->uniqueKeys == 0)
//     // {

//     //     index = hash % 1;
//     // }
//     // else
//     // {
//     //     index = hash % table->uniqueKeys;
//     // }
//     printf("%zu %% %zu =  %i \n", createHashCode(key), table->tableSize, hash % table->tableSize);

//     return hash % table->tableSize;
// }

 uint32_t murmur_32_scramble(uint32_t k)
{
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}
uint32_t murmur3_32(const char *key, size_t len, uint32_t seed)
{
    printf("Key: %s, len: %zu, seed: %u\n", key, len, seed);
    uint32_t h = seed;
    uint32_t k;
    /* Read in groups of 4. */
    for (size_t i = len >> 2; i; i--)
    {
        // Here is a source of differing results across endiannesses.
        // A swap here has no effects on hash properties though.
        memcpy(&k, key, sizeof(uint32_t));
        key += sizeof(uint32_t);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    /* Read the rest. */
    k = 0;
    for (size_t i = len & 3; i; i--)
    {
        k <<= 8;
        k |= key[i - 1];
    }
    // A swap is *not* necessary here because the preceding loop already
    // places the low bytes in the low places according to whatever endianness
    // we use. Swaps only apply when the memory is copied in a chunk.
    h ^= murmur_32_scramble(k);
    /* Finalize. */
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

size_t hashFunc(char *key)
{
    size_t keyCount = 0;
    int sum = 0;
    while (keyCount < strlen(key))
    {
        int c = (int)key[keyCount];
        keyCount++;
        sum += c * keyCount;
    }

    return sum % 2069;
}

void set(HashTable *table, char *key, void *value)
{

    table->keyCount++;
    table->uniqueKeys++;
    size_t index = hashFunc(key);

    // printf("Key: %s\n", key);
    // printf("%zu %% %zu =  %zu - ", createHashCode(key), table->tableSize, index);
    // printf("index: %i\n", index);
    // printf("keys: %zu, unique keys: %zu\n", table->keyCount, table->uniqueKeys);

    // keyCount -> how many in the array index >  0; index < keyCount
    if (table->keyCount != 0 && table->buckets[index].content != NULL)
    {
        // Index already taken
        if (table->uniqueKeys > 0)
        {
            table->uniqueKeys--;
        }

        Bucket tempBucket = table->buckets[index];
        KeyValuePair *tempPair = malloc(sizeof(KeyValuePair));
        tempPair->value = value;
        tempPair->key = key;

        // Link values with the same hash code
        add(tempBucket.content, tempPair);
        printf("REPEATED: For key: %s, value: %i added at index: %zu\n", key, *(int *)value, index);

        return;
    }

    KeyValuePair *kvp = malloc(sizeof(KeyValuePair));
    kvp->value = value;
    kvp->key = key;

    LinkedList *list = add(NULL, kvp);

    Bucket bucket;
    bucket.content = list;

    table->buckets[index] = bucket;
    printf("UNIQUE: For key: %s, value: %i added at index: %zu\n", key, *(int *)value, index);
}

void *getByKey(HashTable *table, char *key)
{
    size_t index = hashFunc(key);

    if (table->buckets[index].content == NULL)
    {
        printf("Key not found error\n");
    }

    Bucket tempBucket = table->buckets[index];

    LinkedList *list = tempBucket.content;
    size_t length = (size_t)len(list);

    // KeyValuePair *kvp = (KeyValuePair *)list->value;
    // printf("%s\n", kvp->key);

    // printf("Size of key %zu\n", strlen(kvp->key));
    // printf("Key: %s\n", key);
    // printf("Pointer to key: %p\n", kvp);

    if (length > 1)
    {
        // Different value with same hash
        // O(len(content))

        LinkedList *current = list;
        while (current != NULL)
        {
            KeyValuePair kvp = *(KeyValuePair *)current->value;

            if (kvp.key == key)
            {
                return kvp.value;
            }

            current = current->next;
        }
    }
    else if (length == 1)
    {
        // Only one value
        KeyValuePair kvp = *(KeyValuePair *)list->value;

        if (kvp.key == key)
        {
            return kvp.value;
        }
    }
    else
    {
        printf("List is empty\n");
        exit(1);
    }

    printf("Key does not match any values\n");

    return NULL;
}

void freeHashTable(HashTable *table)
{
    for (int i = 0; i < MAX_TABLE_SIZE; i++)
    {
        LinkedList *bucket = table->buckets[i].content;
        freeList(bucket);
    }
    free(table->buckets);
    free(table);
}

int main2(void)
{
    HashTable *hashTable = malloc(sizeof(HashTable));
    if (hashTable != NULL)
    {
        for (int i = 0; i < MAX_TABLE_SIZE; ++i)
        {
            hashTable->buckets[i].content = NULL;
        }
    }
    hashTable->keyCount = 0;
    hashTable->uniqueKeys = 0;

    // ulong hash = createHashCode("Lol");
    // printf("Hash code: %lu, index: %i\n", hash, hash % 3);
    // printf("Hash code: %lu, index: %i\n", hash, hash % 4);
    // printf("Hash code: %lu, index: %i\n", hash, hash % 5);
    // printf("Hash code: %lu, index: %i\n", hash, hash % 5);

    // printf("Index: %lu\n", hash % 4);

    char **strings;
    size_t stringLen = 5;
    strings = (char **)malloc(stringLen * sizeof(char *));
    // Allocate memory for each string dynamically
    strings[0] = createStr("Marco");
    strings[1] = createStr("Bia");
    strings[2] = createStr("Bruno");
    strings[3] = createStr("Augie");
    strings[4] = createStr("Bianca");

    // set(&hashTable, "1", strings[0]);
    // set(&hashTable, "2", strings[1]);
    // set(&hashTable, "3", strings[2]);
    // set(&hashTable, "4", strings[3]);
    // set(&hashTable, "5", strings[4]);

    // set(hashTable, "1", &(int){1});
    // set(hashTable, "2", &(int){2});
    // set(hashTable, "3", &(int){3});
    // set(hashTable, "4", &(int){4});
    // set(hashTable, "5", &(int){5});
    // set(hashTable, "5", &(int){5});

    char *keys[5] = {"1", "2", "3", "4", "10"};
    int n[5] = {1, 2, 3, 4, 10};
    (void)n;

    for (int i = 0; i < 5; ++i)
    {
        set(hashTable, keys[i], &n[i]);
    }

    // set(hashTable, keys[1], &n[0]);

    // printf("%i\n", *(int *)((KeyValuePair *)((hashTable->buckets[50].content->next)->value))->value);

    printf("\n----------------------------------------\n\n");

    // printf("Key: %s\n", ((KeyValuePair *)(hashTable->buckets[49].content->value))->key);
    // printf("Key: %s\n", ((KeyValuePair *)(hashTable->buckets[50].content->value))->key);
    // printf("Key: %s\n", ((KeyValuePair *)(hashTable->buckets[51].content->value))->key);
    // printf("Key: %s\n", ((KeyValuePair *)(hashTable->buckets[52].content->value))->key);
    // printf("Key: %s\n", ((KeyValuePair *)(hashTable->buckets[53].content->value))->key);
    // printf("value: %i\n", *(int *)((KeyValuePair *)(hashTable->buckets[49].content->value))->value);
    // printf("value: %i\n", *(int *)((KeyValuePair *)(hashTable->buckets[50].content->value))->value);
    // printf("value: %i\n", *(int *)((KeyValuePair *)(hashTable->buckets[51].content->value))->value);
    // printf("value: %i\n", *(int *)((KeyValuePair *)(hashTable->buckets[52].content->value))->value);
    // printf("value: %i\n", *(int *)((KeyValuePair *)(hashTable->buckets[53].content->value))->value);
    // printf("List loc: %p\n", hashTable->buckets[49].content);
    // printf("List loc: %p\n", hashTable->buckets[50].content);
    // printf("List loc: %p\n", hashTable->buckets[51].content);
    // printf("List loc: %p\n", hashTable->buckets[52].content);
    // printf("List loc: %p\n", hashTable->buckets[53].content);

    for (int i = 0; i < 5; ++i)
    {
        void *value = getByKey(hashTable, keys[i]);

        if (value == NULL)
        {
            return 1;
        }

        int intValue = *(int *)value;
        printf("Value: %i\n", intValue);
    }

    printf("Number of keys: %zu\n", hashTable->keyCount);
    printf("Number of unique keys: %zu\n", hashTable->uniqueKeys);
    // printf("%i\n", len(hashTable.buckets[2].content));

    return 0;
}

uint64_t murmurhash64A(const void *key, int len, uint64_t seed) {
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t *data = (const uint64_t *)key;
    const uint64_t *end = data + (len / 8);

    while (data != end) {
        uint64_t k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
    }

    const uint8_t *data2 = (const uint8_t *)data;

    switch (len & 7) {
        case 7: h ^= ((uint64_t)data2[6]) << 48;
        case 6: h ^= ((uint64_t)data2[5]) << 40;
        case 5: h ^= ((uint64_t)data2[4]) << 32;
        case 4: h ^= ((uint64_t)data2[3]) << 24;
        case 3: h ^= ((uint64_t)data2[2]) << 16;
        case 2: h ^= ((uint64_t)data2[1]) << 8;
        case 1: h ^= ((uint64_t)data2[0]);
                h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}


int main()
{
    // uint32_t seed = 10;
    // char *key = "Marco";
    // printf("%zu\n", strlen(key));
    // printf("%zu\n", sizeof(uint32_t));

    // uint32_t hash = murmur3_32(key, strlen(key), seed);

    // printf("Hash: %i\n", hash);
    char *key = "marco";
    int len = strlen(key);
    uint64_t seed = 10;

    uint64_t hash = murmurhash64A(key, len, seed);
    printf("Hash: %" PRIu64 "\n", hash);

    return 1;
}

int main1()
{
    HashTable *hashTable = malloc(sizeof(HashTable));
    if (hashTable != NULL)
    {
        for (int i = 0; i < MAX_TABLE_SIZE; ++i)
        {

            hashTable->buckets[i].content = NULL;
        }
    }
    else
    {
        printf("It broke\n");
    }
    hashTable->keyCount = 0;
    hashTable->uniqueKeys = 0;

    int N = 10;

    int *numbers = (int *)malloc(N * sizeof(int));

    if (numbers == NULL)
    {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        numbers[i] = i + 1;
    }

    char **strings = (char **)malloc(N * sizeof(char *));

    if (strings == NULL)
    {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        strings[i] = (char *)malloc(2 * sizeof(char));
        if (strings[i] == NULL)
        {
            printf("Failed to allocate memory for string.\n");

            for (int j = 0; j < i; j++)
            {
                free(strings[j]);
            }
            free(strings);
            return 1;
        }

        sprintf(strings[i], "%d", i + 1);
    }

    for (int i = 0; i < N; ++i)
    {
        // printf("At %i: \"%s\" and \"%i\"\n", i, strings[i], numbers[i]);
        set(hashTable, strings[i], &numbers[i]);
    }

    for (int i = 0; i < N; ++i)
    {
        void *value = getByKey(hashTable, strings[i]);

        if (value == NULL)
        {
            return 1;
        }

        int intValue = *(int *)value;
        printf("Value for key %s: %i\n", strings[i], intValue);
    }

    printf("Number of keys: %zu\n", hashTable->keyCount);
    printf("Number of unique keys: %zu\n", hashTable->uniqueKeys);

    // Free all the allocated memory
    for (int i = 0; i < N; i++)
    {
        free(strings[i]);
    }

    free(strings);
    free(numbers);
    freeHashTable(hashTable);

    return 0;
}