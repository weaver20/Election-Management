//
// Created by avira on 10/04/2020.
//

#ifndef ME_DRY_H
#define ME_DRY_H

#include <stdio.h>
#include <stdbool.h>



typedef struct node_t* Node;
typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    EMPTY_LIST,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out);



#endif //ME_DRY_H
