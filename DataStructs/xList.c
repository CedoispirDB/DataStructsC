#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
typedef Node xList;

struct Node
{
    void *data;
    Node *next;
    Node *prev;
    Node *start;
    Node *end;
    size_t length;
};

void exitOnError(char *message, const char *func, int line)
{
    printf("Error: %s in function \"%s\" at line %d.\n", message, func, line);
    exit(-1);
}

Node *add(Node *head, void *data)
{
    Node *temp = malloc(sizeof(Node));

    if (data == NULL)
    {
        exitOnError("cannot initiate node with null data", __func__, __LINE__);
    }

    if (head == NULL)
    {
        temp->data = data;
        temp->next = NULL;
        temp->prev = NULL;
        temp->start = temp;
        temp->end = temp;
        temp->length = 1;

        return temp;
    }

    Node *current = head;
    Node *prev;

    while (current != NULL)
    {
        current->end = temp;
        current->length = current->length + 1;
        prev = current;
        current = current->next;
    }

    prev->next = temp;

    temp->data = data;
    temp->next = NULL;
    temp->prev = prev;
    temp->start = head;
    temp->end = temp;
    temp->length = prev->length;

    return temp;
}

void *get(Node *head, int index)
{
    if (index >= (int)head->length || abs(index) > (int)head->length)
    {
        exitOnError("index out of bounds", __func__, __LINE__);
    }

    int count = 0;

    if (index < 0)
    {
        Node *temp = head;
        head = temp->end;
        index = abs(index);
        if (index <= (int)(head->length / 2))
        {
            Node *current = head;
            while (current != NULL)
            {
                if (count == (index - 1))
                {
                    return current->data;
                }
                current = current->prev;
                count++;
            }
        }
        else
        {
            Node *current = temp;
            index = current->length - index;
            while (current != NULL)
            {
                if (count == index)
                {
                    return current->data;
                }
                current = current->next;
                count++;
            }
        }
    }
    else
    {
        if (index <= (int)(head->length / 2))
        {
            Node *current = head;
            while (current != NULL)
            {
                if (count == index)
                {
                    return current->data;
                }
                current = current->next;
                count++;
            }
        }
        else
        {
            Node *current = head->end;
            count = head->length - 1;
            while (current != NULL)
            {
                if (count == index)
                {
                    return current->data;
                }
                current = current->prev;
                count--;
            }
        }
    }

    return NULL;
}

void printNode(Node *node)
{
    printf("Current: %p\n", node);
    printf("Data: %s\n", (char *)node->data);
    printf("Next: %p\n", node->next);
    printf("Previous: %p\n", node->prev);
    printf("Start: %p\n", node->start);
    printf("End: %p\n", node->end);
    printf("Count: %zu\n", node->length);
}

void print(Node *head)
{
    Node *current = head;
    size_t count = 0;

    while (current != NULL)
    {
        printf("-------------------------\nindex: %zu\n", count);
        printNode(current);
        current = current->next;
        count++;
    }
    printf("-------------------------\n");
}

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

    for (int i = (int) head->length * -1; i < 0; ++ei)
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