#ifndef CUSTOM_LINKED_LIST_H
#define CUSTOM_LINKED_LIST_H

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

Node *createNode();
Node *add(Node *head, void *value, char *dataType);
int len(Node *head);
void exitIfOutOfBounds(Node *head, int index, char *loc);
void *get(Node *head, int index);
Node *getNode(Node *head, int index);
Node *insert(Node *head, void *value, int index, char *dataType);
void delete(Node *head, int index);
void freeMemory(Node *head);
Node *convertArray(void *array, size_t len, char *dataType);
void printValues(Node *head);
char *createStr(char *src);
void ShowBasicCreation();
void ShowConvertFromArray();

#endif  // CUSTOM_LIST_H
