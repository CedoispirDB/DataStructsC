#include "../DataStructs/linkedList.h"

int main(void)
{
    DATA_TYPE = "int";
    
    LinkedList *head = add(NULL, &(int){1});
    add(head, &(int){2});
    add(head, &(int){3});
    add(head, &(int){4});
    add(head, &(int){5});


    printf("Values added to list:\n");
    printValues(head);
    printf("\n");

    int index = 1;
    printf("Value inserted at index %i:\n", index);
    insert(head, &(int){10}, index);
    printValues(head);
    printf("\n");

    int rmIndex = 3;
    printf("Value deleted at index %i:\n", rmIndex);
    delete (head, rmIndex);
    printValues(head);

    freeList(head);
    return 0;
}