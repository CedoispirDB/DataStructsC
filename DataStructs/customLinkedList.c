#include "customLinkedList.h"

Node *createNode()
{
    return (Node *)malloc(sizeof(Node));
}

Node *add(Node *head, void *value, char *dataType)
{
    Node *temp = createNode();
    temp->value = value;
    temp->next = NULL;
    temp->dataType = dataType;

    if (head == NULL)
    {
        return temp;
    }

    Node *current = head;
    Node *last;

    while (current != NULL)
    {
        last = current;
        current = current->next;
    }
    last->next = temp;

    return temp;
}

int len(Node *head)
{
    int size = 0;

    Node *current = head;

    while (current != NULL)
    {
        size += 1;
        current = current->next;
    }

    return size;
}

void exitIfOutOfBounds(Node *head, int index, char *loc)
{
    if (index < 0 || index >= len(head))
    {
        printf("Index %i out of bounds in %s\n", index, loc);
        exit(1);
    }
}

void *get(Node *head, int index)
{
    exitIfOutOfBounds(head, index, "get");

    Node *current = head;
    int count = 0;

    while (current != NULL)
    {
        if (count == index)
            return current->value;

        current = current->next;
        count++;
    }

    return NULL;
}

Node *getNode(Node *head, int index)
{
    exitIfOutOfBounds(head, index, "getNode");

    Node *current = head;
    int count = 0;

    while (current != NULL)
    {
        if (count == index)
            return current;

        current = current->next;
        count++;
    }

    return NULL;
}

Node *insert(Node *head, void *value, int index, char *dataType)
{
    exitIfOutOfBounds(head, index, "insert");

    Node *newNode = createNode();

    if (index == 0)
    {
        newNode->value = head->value;
        newNode->next = head->next;
        newNode->dataType = dataType;
        head->value = value;
        head->next = newNode;
        return newNode;
    }
    else if (index == len(head) - 1)
    {
        add(head, value, dataType);
        return newNode;
    }

    newNode->value = value;
    newNode->dataType = dataType;

    Node *prevNode = getNode(head, index - 1);
    Node *nextNode = getNode(head, index);

    // printf("prevNode - ptr: %p value: %i, next: %p", (void *) prevNode, *(int*)(prevNode->value), (void *)(prevNode->next));
    // printf("nextNode - ptr: %p value: %i, next: %p", (void *) nextNode, *(int*)(nextNode->value), (void *)(nextNode->next));

    prevNode->next = newNode;
    newNode->next = nextNode;

    return newNode;
}

void delete(Node *head, int index)
{
    exitIfOutOfBounds(head, index, "delete");

    Node *remove = getNode(head, index);

    if (index == 0)
    {
        Node *nextNode = head->next;
        head->value = nextNode->value;
        head->next = nextNode->next;
        free(nextNode);
        return;
    }

    Node *prevNode = getNode(head, index - 1);

    prevNode->next = remove->next;

    free(remove);
}

void freeMemory(Node *head)
{
    Node *current = head;
    Node *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;

        free(temp);
    }
}

Node *convertArray(void *array, size_t len, char *dataType)
{
    Node *head;

    if (strcmp(dataType, "int") == 0)
    {
        int *intArray = (int *)array;

        for (size_t i = 0; i < len; ++i)
        {
            if (i == 0)
            {
                head = add(NULL, (void *)&intArray[i], dataType);
                continue;
            }
            add(head, (void *)&intArray[i], dataType);
        }
    }
    else if (strcmp(dataType, "str") == 0)
    {
        char **strArray = (char **)array;

        for (size_t i = 0; i < len; ++i)
        {
            if (i == 0)
            {
                head = add(NULL, (void *)strArray[i], dataType);
                continue;
            }
            add(head, (void *)strArray[i], dataType);
        }
    }
    else if (strcmp(dataType, "char") == 0)
    {
        char *charArray = (char *)array;

        for (size_t i = 0; i < len; ++i)
        {
            if (i == 0)
            {
                head = add(NULL, &charArray[i], dataType);
                continue;
            }
            add(head, &charArray[i], dataType);
        }
    }
    else
    {
        printf("Data type \"%s\" not implemented\n", dataType);
    }

    return head;
}

void printValues(Node *head)
{
    printf("Size: %i\n", len(head));
    for (int i = 0; i < len(head); ++i)
    {
        Node *current = getNode(head, i);
        char *dataType = (char *)current->dataType;

        if (strcmp(dataType, "int") == 0)
        {
            printf("Value at index %i: %i (%s)\n", i, *(int *)current->value, dataType);
        }
        else if (strcmp(dataType, "str") == 0)
        {
            printf("Value at index %i: %s (%s)\n", i, (char *)current->value, dataType);
        }
        else if (strcmp(dataType, "char") == 0)
        {
            printf("Value at index %i: %c (%s)\n", i, *(char *)current->value, dataType);
        }
        else
        {
            printf("Data type \"%s\" not implemented\n", dataType);
        }
    }
}

char *createStr(char *src)
{
    size_t len = strlen(src) + 1;
    char *des = malloc(len * sizeof(char));
    if (des != NULL)
    {
        memcpy(des, src, len);
    }

    return des;
}

void ShowBasicCreation()
{
    Node *head = add(NULL, &(int){1}, "int");

    char *str = "Cedoispir";
    add(head, &(int){2}, "int");
    add(head, str, "str");
    add(head, &(int){3}, "int");
    add(head, &(int){4}, "int");
    add(head, &(int){5}, "int");

    printf("Values added to list:\n");
    printValues(head);
    printf("\n");
    
    int index = 1;
    printf("Value inserted at index %i:\n", index);
    insert(head, &(int){10}, index, "int");
    printValues(head);
    printf("\n");

    int rmIndex = 3;
    printf("Value deleted at index %i:\n", rmIndex);
    delete (head, rmIndex);
    printValues(head);

    freeMemory(head);
}

void ShowConvertFromArray(void)
{
    // Create integer array
    int integers[] = {50, 40, 30, 20, 10, 9, 8, 7, 6};
    size_t integerLen = sizeof(integers) / sizeof(integers[0]);

    // Create string array
    char **strings;
    size_t stringLen = 5;
    strings = (char **)malloc(stringLen * sizeof(char *));
    // Allocate memory for each string dynamically
    strings[0] = createStr("Marco");
    strings[1] = createStr("Bia");
    strings[2] = createStr("Bruno");
    strings[3] = createStr("Augie");
    strings[4] = createStr("Bianca");

    // Create char array
    char charArray[6] = {'H', 'e', 'l', 'l', 'o', '!'};
    size_t charLen = sizeof(charArray) / sizeof(charArray[0]);

    Node *integerHead = convertArray(integers, integerLen, "int");
    Node *stringHead = convertArray(strings, stringLen, "str");
    Node *charHead = convertArray(charArray, charLen, "char");

    printf("Converting from int array:\n");
    printValues(integerHead);

    printf("\nConverting from string array:\n");
    printValues(stringHead);

    printf("\nConverting from char array:\n");
    printValues(charHead);

    // Free allocated memory
    freeMemory(integerHead);
    freeMemory(stringHead);
    freeMemory(charHead);
}