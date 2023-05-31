#include "hashTable.h"

uint32_t rotl32(uint32_t x, int r)
{
    return (x << r) | (x >> (32 - r));
}

uint32_t MurmurHashAligned2(const void *key, int32_t len, uint32_t seed)
{
    const uint32_t m = 0x5BD1E995;
    const int32_t r = 24;
    const uint8_t *data = (const uint8_t *)key;
    uint32_t h = seed ^ len;

    // Mix 4 bytes at a time in 32-bit blocks
    while (len >= 4)
    {
        uint32_t k;
        memcpy(&k, data, sizeof(uint32_t));
        k *= m;
        k ^= rotl32(k, r);
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    // Handle the last few bytes
    switch (len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    // Finalize the hash
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

void set(HashTable *table, char *key, void *value)
{

    table->keyCount++;
    table->uniqueKeys++;
    size_t index = MurmurHashAligned2(key, strlen(key), HASH_SEED) % MAX_TABLE_SIZE;

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
        printf("REPEATED: For key: %s, value: %i added at index: %zu\n", key, (char *)value, index);

        return;
    }
    KeyValuePair *kvp = malloc(sizeof(KeyValuePair));
    kvp->value = value;
    kvp->key = key;


    LinkedList *list = add(NULL, kvp);

    Bucket bucket;
    bucket.content = list;

    table->buckets[index] = bucket;
    printf("UNIQUE: For key: %s, value: %i added at index: %zu\n", key, (char *)value, index);
}

void *getByKey(HashTable *table, char *key)
{
    size_t index = MurmurHashAligned2(key, strlen(key), HASH_SEED) % MAX_TABLE_SIZE;

    if (table->buckets[index].content == NULL)
    {
        printf("Key not found error\n");
    }

    Bucket tempBucket = table->buckets[index];

    LinkedList *list = tempBucket.content;
    size_t length = (size_t)len(list);

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
}

// int main()
// {
//     HashTable *hashTable = malloc(sizeof(HashTable));

//     if (hashTable != NULL)
//     {
//         for (int i = 0; i < MAX_TABLE_SIZE; ++i)
//         {

//             hashTable->buckets[i].content = NULL;
//         }
//     }
//     else
//     {
//         printf("It broke\n");
//     }
//     hashTable->keyCount = 0;
//     hashTable->uniqueKeys = 0;

//     int N = 400;

//     int *numbers = (int *)malloc(N * sizeof(int));

//     if (numbers == NULL)
//     {
//         printf("Failed to allocate memory.\n");
//         return 1;
//     }

//     for (int i = 0; i < N; i++)
//     {
//         numbers[i] = i + 1;
//     }

//     char **strings = (char **)malloc(N * sizeof(char *));

//     if (strings == NULL)
//     {
//         printf("Failed to allocate memory.\n");
//         return 1;
//     }

//     for (int i = 0; i < N; i++)
//     {
//         strings[i] = (char *)malloc(2 * sizeof(char));
//         if (strings[i] == NULL)
//         {
//             printf("Failed to allocate memory for string.\n");

//             for (int j = 0; j < i; j++)
//             {
//                 free(strings[j]);
//             }
//             free(strings);
//             return 1;
//         }

//         sprintf(strings[i], "%d", i + 1);
//     }

//     for (int i = 0; i < N; ++i)
//     {
//         // printf("At %i: \"%s\" and \"%i\"\n", i, strings[i], numbers[i]);
//         set(hashTable, strings[i], &numbers[i]);
//     }

//     for (int i = 0; i < N; ++i)
//     {
//         void *value = getByKey(hashTable, strings[i]);

//         if (value == NULL)
//         {
//             return 1;
//         }

//         int intValue = *(int *)value;
//         printf("Value for key %s: %i\n", strings[i], intValue);
//     }

//     printf("Number of keys: %zu\n", hashTable->keyCount);
//     printf("Number of unique keys: %zu\n", hashTable->uniqueKeys);

//     // Free all the allocated memory
//     for (int i = 0; i < N; i++)
//     {
//         free(strings[i]);
//     }

//     free(strings);
//     free(numbers);
//     freeHashTable(hashTable);
//     free(hashTable);

//     return 0;
// }