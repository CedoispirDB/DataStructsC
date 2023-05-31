#ifndef XLIST_H
#define XLIST_H

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

void exitOnError(char *message, const char *func, int line);
Node *add(Node *head, void *data);
void *get(Node *head, int index);
void printNode(Node *node);
void print(Node *head);

#endif /* XLIST_H */
