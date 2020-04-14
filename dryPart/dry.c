//
// Created by avira on 10/04/2020.
//
#include "dry.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


struct node_t {
    int x;
    struct node_t* next;
};

Node makeListUp(int first, int size) {
    Node list_head = malloc(sizeof(*list_head));
    Node list = list_head;
    if(list == NULL)
        return NULL;
    list->x= first;
    for(int i=1;i<size;i++)
    {
       list->next = nodeCreateOrDestroyAll(list_head,first+i);
        if (list->next == NULL) {
            return NULL;
        }
        list = list->next;
    }
    return list_head;
}
Node makeListDown(int first, int size) {
    Node list_head = malloc(sizeof(*list_head));
    Node list = list_head;
    if(list == NULL)
        return NULL;
    list->x= first;
    for(int i=1;i<size;i++)
    {
        list->next = nodeCreateOrDestroyAll(list_head,first+2*i);
        if (list->next == NULL) {
            return NULL;
        }
        list = list->next;
    }
    return list_head;
}

void listPrint(Node list) {
    while (list != NULL) {
        printf("%d, ",list->x);
        list = list->next;
    }

}
void listDestroy(Node ptr) {
    while (ptr) {
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
}

Node nodeCreateOrDestroyAll(Node head, int data) {
    Node node = malloc(sizeof(*node));
    if (node == NULL) {
        listDestroy(head);
        return NULL;
    }
    node->x = data;
    node->next = NULL;
    return node;
}


Node listCopyOrDestroyAll(Node source,Node destination,Node destination_head)
{
    assert(source != NULL);
    while(source) {
        // destination list is empty
        if(destination_head == NULL) {
            destination_head = nodeCreateOrDestroyAll(destination_head, source->x);
            if(destination_head == NULL){
                return NULL;
            }
            destination = destination_head;
        }
        // destination list is NOT empty
        else {
            destination->next = nodeCreateOrDestroyAll(destination_head, source->x);
            if (destination->next == NULL) {
                return NULL;
            }
            destination = destination->next;
        }
        source = source->next;
    }
    return destination_head;
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node* merged_out) {

    Node merged_ptr = *merged_out;
    // both lists are empty
    if (list1 == NULL && list2 == NULL) {
        return EMPTY_LIST;
    }
    // check if lists are sorted (or empty).
/*    if(!isListSorted((list1)) || !isListSorted((list2))){
        return UNSORTED_LIST;
    }*/
    //both lists are full
    while (list1 != NULL && list2 != NULL) {
        int merge_from = (list1->x <= list2->x) ? 1 : 2;
        switch (merge_from) {
            case 1:
                if(*merged_out == NULL) { // merged list is empty
                    *merged_out = nodeCreateOrDestroyAll(*merged_out, list1->x);
                    if(*merged_out == NULL){
                        return MEMORY_ERROR;
                    }
                    merged_ptr=*merged_out;
                }
                else {
                    merged_ptr->next = nodeCreateOrDestroyAll(*merged_out, list1->x);
                    if (merged_ptr->next == NULL) {
                        return MEMORY_ERROR;
                    }
                    merged_ptr = merged_ptr->next;
                }
                    list1 = list1->next;

                break;
            case 2:
                if(*merged_out == NULL) { // merged list is empty
                    *merged_out = nodeCreateOrDestroyAll(*merged_out, list2->x);
                    if(*merged_out == NULL){
                        return MEMORY_ERROR;
                    }
                    merged_ptr=*merged_out;
                }
                else {
                    merged_ptr->next = nodeCreateOrDestroyAll(*merged_out, list2->x);
                    if (merged_ptr->next == NULL) {
                        return MEMORY_ERROR;
                    }
                    merged_ptr = merged_ptr->next;
                }
                list2 = list2->next;
                break;
        }

    }
    // one of the lists is empty
    if (list1 == NULL) {
        Node tmp = listCopyOrDestroyAll(list2, merged_ptr, *merged_out);
        if ( tmp == NULL) {
            return MEMORY_ERROR;
        }
    } else {  // list2 == NULL
        Node tmp = listCopyOrDestroyAll(list1, merged_ptr, *merged_out);
        if (tmp == NULL) {
            return MEMORY_ERROR;
        }
        *merged_out = tmp;
    }

    return SUCCESS;
}
