#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node Node;

struct Node
{
    void *value;
    Node *next;
    const char *dataType;
};

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
    Node *nextNode = getNode(head, index + 1);

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
        char *strArray = (char *)array;

        for (size_t i = 0; i < len; ++i)
        {
            if (i == 0)
            {
                head = add(NULL, (void *)&strArray[i], dataType);
                continue;
            }
            add(head, (void *)&strArray[i], dataType);
        }
    }
    else
    {
        printf("Data type \"%s\" not implemented\n", dataType);
    }

    return head;
}

// void printValues(Node* head, int index) {
//     void* value = get(head, index);

//     size_t size = sizeof(*value);

//     if(size == 1) {
//         // Char
//         printf("here\n");
//         printf("Value at %i: %s\n", index, (char*)value);
//     } else if(size == 4) {
//         printf("here 2\n");

//         printf("Value at %i: %i\n", index, *(int*)value);
//     }
// }

void printValues(Node *head)
{
    printf("Size: %i\n", len(head));
    for (int i = 0; i < len(head); ++i)
    {
        Node *current = getNode(head, i);
        char *dataType = (char *)current->dataType;

        if (strcmp(dataType, "int") == 0)
        {
            printf("Value at %i: %i (%s)\n", i, *(int *)current->value, dataType);
        }
        else if (strcmp(dataType, "str") == 0)
        {
            printf("Value at %i: %s (%s)\n", i, (char *)current->value, dataType);
        }
        else
        {
            printf("Data type \"%s\" not implemented\n", dataType);
        }
    }
}

int main2(void)
{
    Node *head = add(NULL, &(int){1}, "int");

    char *str = "Cedoispir";
    add(head, &(int){2}, "int");
    add(head, str, "str");
    add(head, &(int){4}, "lol");
    add(head, &(int){5}, "int");

    printValues(head);
    printf("\n");
    insert(head, &(int){9}, 0, "int");

    printValues(head);
    printf("\n");
    delete (head, 0);

    printValues(head);

    freeMemory(head);

    return 0;
}

int main(void)
{
    // int number[] = {50, 40, 30, 20, 10, 9, 8, 7, 6};
    // size_t len = sizeof(number) / sizeof(number[0]);

    char words[][10] = {"Marco", "Bia", "Bruno", "Augie"};
    size_t len = sizeof(words) / sizeof(words[0]);

    char **strings;
    int numStrings = 5;

    strings = (char **)malloc(numStrings * sizeof(char *));

    // Allocate memory for each string dynamically
    strings[0] = strdup("Marco");
    strings[1] = strdup("Bia");
    strings[2] = strdup("Bruno");
    strings[3] = strdup("Augie");
    strings[4] = strdup("Bianca");

    Node *head = convertArray(strings, len, "str");

    printValues(head);

    freeMemory(head);

    return 0;
}