//
// Created by avira on 10/04/2020.
//
#include "dry.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0
#define DEMI_VALUE -9999

struct node_t {
    int x;
    struct node_t* next;
};

static void destroyList(Node ptr);
static Node createOrDestroy(int data);

int getListLength(Node list) {
    int length = 0;
    while (list != NULL)
        length++;






    return length - 1;  // first Node is a demi.
}

// TODO:
bool isListSorted(Node list) {
    return true;
}
//TODO:  destroyAll part
Node nodeCreateOrDestroyAll(Node head, int data) {
    Node ptr = malloc(sizeof(*ptr));
    if (ptr == NULL) {
        return NULL;
    }
    ptr->x = data;
    ptr->next = NULL;
    return ptr;
}

void destroyList(Node ptr) {
    while (ptr) {
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
}

ErrorCode listCopyOrDestroyAll(Node list1,Node merged_ptr,Node merged_head)
{


}

ErrorCode mergeSortedLists(Node list1, Node list2, Node* merged_out) {
    // making sure merged_out is not NULL
    Node node1 = list1, node2 = list2, merged_ptr = *merged_out;
    merged_ptr = nodeCreateOrDestroyAll(*merged_out, DEMI_VALUE);
    if (merged_ptr == NULL) {
        return MEMORY_ERROR;
    }
    int list1_length = getListLength(list1);
    int list2_length = getListLength(list2);

    if (list1_length == EMPTY && list2_length == EMPTY) {
        return EMPTY_LIST;  // what do we do with merged_out?
    }

    //if we made it so far both of the lists are not empty
    while (list1 != NULL && list2 != NULL) {
        int merge_from = (list1->x <= list2->x) ? 1 : 2;
        switch (merge_from) {
            case 1:
                //..
                merged_ptr->next = nodeCreateOrDestroyAll(*merged_out, list1->x);
                if (merged_ptr->next == NULL) {
                    return MEMORY_ERROR;
                }
                merged_ptr = merged_ptr->next;
                list1 = list1->next;
                break;
            case 2:
                //..
                merged_ptr->next = nodeCreateOrDestroyAll(*merged_out, list2->x);
                if (merged_ptr->next == NULL) {
                    return MEMORY_ERROR;
                }
                merged_ptr = merged_ptr->next;
                list2 = list2->next;
                break;
        }

    }
        // TODO: maybe do a recursive call instead??
    if (list1 == NULL) {
        if (listCopyOrDestroyAll(list2, merged_ptr, *merged_out) == MEMORY_ERROR) {
            return MEMORY_ERROR;
        }
    } else if (list2 == NULL) {
        if (listCopyOrDestroyAll(list1, merged_ptr, *merged_out) == MEMORY_ERROR) {
            return MEMORY_ERROR;
        }
    }
    return SUCCESS;
}
