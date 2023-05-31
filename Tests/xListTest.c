#include "../DataStructs/xList.h"

int main(void)
{
    xList *head = add(NULL, "Zero");
    add(head, "One");
    add(head, "Two");
    add(head, "Three");
    add(head, "Four");
    add(head, "Five");
    add(head, "Six");

    print(head);

    for (int i = 0; i < (int) head->length; ++i)
    {
        char *value = get(head, i);

        if (value == NULL)
        {
            exitOnError("Value is null for some reason", __func__, __LINE__);
        }

        printf("Value at index %i: %s\n", i, value);
    }

    printf("\n");

    for (int i = (int) head->length * -1; i < 0; ++i)
    {
        char *value = get(head, i);

        if (value == NULL)
        {
            exitOnError("Value is null for some reason", __func__, __LINE__);
        }

        printf("Value at index %i: %s\n", i, value);
    }

    return 0;
}