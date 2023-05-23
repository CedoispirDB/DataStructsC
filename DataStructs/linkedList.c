#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node Node;

struct Node
{
    void* value;
    Node* next;
    size_t size;
};

Node* add(Node* head, void* value)
{
    Node* temp = (Node *)malloc(sizeof(Node));
    temp->value = value;
    temp->next = NULL;

    if(head == NULL) {
        return temp;
    }

    Node* current = head;
    Node* last;

    while(current != NULL) {
        last = current;
        current = current->next;
    }
    last->next = temp;

    return temp;
}

int len(Node* head) 
{
    int size = 0;

    Node* current = head;

    while(current != NULL) 
    {
        size += 1;
        current = current->next;
    }

    return size;
}

void* get(Node* head, int index) 
{
    if(index < 0)
    {
        printf("Index %i out of bounds\n", index);
        exit(1);
    } 

    Node* current = head;
    int count = 0;

    while(current != NULL) 
    {
        if(count == index) return current->value;
        
        current = current->next;
        count++;
    }

    printf("Index %i out of bounds\n", index);

    return NULL;

}

void printValues(Node* head, int index) {
    void* value = get(head, index);

    size_t size = sizeof(*value);

    if(size == 1) {
        // Char
        printf("here\n");
        printf("Value at %i: %s\n", index, (char*)value);
    } else if(size == 4) {
        printf("here 2\n");

        printf("Value at %i: %i\n", index, *(int*)value);
    }
}


int main()
{
    Node* head = add(NULL, &(int){1});
    add(head, &(int){2});
    add(head, &(int){3});
    add(head, &(int){4});
    add(head, &(int){5});

    printf("Size: %i\n", len(head));

    for(int i = 0; i < len(head); ++i)
    {
        printf("Value at %i: %i\n", i, *(int*)get(head, i));
    } 

    free(head);

    return 0;
}