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


/* ***************************************************************** */
/* **************** Dry part - PART 1 ****************************** */
/* ***************************************************************** */




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


Node listCopyOrDestroyAll(Node source, Node dest, Node dest_head)
{
    Node destination_ptr = dest;
    assert(source != NULL);
    while(source) {
        destination_ptr->next = nodeCreateOrDestroyAll(dest_head, source->x);
        if (destination_ptr->next == NULL) {
            return NULL;
        }
        destination_ptr = destination_ptr->next;
        source = source->next;
    }
    return dest;
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node* merged_out) {
    // one of the lists is empty
    if (list1 == NULL || list2 == NULL) {
        return EMPTY_LIST;
    }
    Node merged_ptr = *merged_out;
    // check if lists are sorted
    if(!isListSorted(list1) || !isListSorted(list2)) {
        return UNSORTED_LIST;
    }
    //both lists are sorted and full
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
                else {      // merged list is not empty
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
                else {      // merged list is not empty
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
    Node rest_of_list = NULL;
    // finished with one of the lists
    if (list1 == NULL) {    // finished with list1
        rest_of_list = listCopyOrDestroyAll(list2, merged_ptr, *merged_out);
        if (rest_of_list == NULL) {
            return MEMORY_ERROR;
        }
    }
    else {  // finished with list2
        rest_of_list = listCopyOrDestroyAll(list1, merged_ptr, *merged_out);
        if (rest_of_list == NULL) {
            return MEMORY_ERROR;
        }
    }
    return SUCCESS;
}












































/* ***************************************************************** */
/* **************** Dry part - PART 2 ****************************** */
/* ***************************************************************** */


char *stringDuplicator(char *s, int times) {
    // PROGRAMMING ERROR 1: s should be transferred to the function as const to
    // CONVENTION ERROR 1: s name should be src
    // CONVENTION ERROR 2: func name should be a verb
    assert(!s);
    // PROGRAMMING ERROR 2: should be assert(s)
    assert(times > 0);
    int LEN = strlen(s);
    // CONVENTION ERROR 3: LEN - variable names should be in lower case
    // PROGRAMMING ERROR 3:we should allocate an additional bye for the /0  (strlen() returns the length of the string without the /0 char)
    char *out = malloc(LEN * times);
    assert(out);
    // PROGRAMMING ERROR 4: should be if(!out) { return NULL; }
    for (int i = 0; i < times; i++) {
        // CONVENTION ERROR 4: no indent lines in for loop
        out = out + LEN;
        // PROGRAMMING ERROR 5: this two lines should flip - first copy, than increment pointer
        strcpy(out, s);
    }
    return out;
    // PROGRAMMING ERROR 6: returning a pointer to the end of out string
}


// **********  Fixed: **********


char* stringDuplicate(const char *src, int times) {
    assert(src);
    assert(times > 0);
    char* out = malloc(strlen(src) * (times + 1));
    if(!out) {
        return NULL;
    }
    for (int i = 0; i < times; i++) {
        strcat(out, src);
    }
    return out;
}