#include "../DataStructs/hashTable.h"


int main()
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

    int N = 400;

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
    free(hashTable);

    return 0;
}