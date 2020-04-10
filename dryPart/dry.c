//
// Created by avira on 10/04/2020.
//
#include "dry.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


struct node_t {
    int x;
    struct node_t *next;
};

static void destroyList(Node ptr);
static Node createOrDestroy(int data);

int getListLength(Node list)
{
  int length = 0;
  while(list !=NULL)
      length++;

  return length;
}


bool isListSorted(Node list) {
    return true;
}

Node createOrDestroy(int data) {
    Node ptr = malloc(sizeof(*ptr));
    if(ptr == NULL) {
        destroyList(ptr);
        return NULL;
    }
    ptr->x = data;
    ptr->next = NULL;
    return ptr;
}

void destroyList(Node ptr) {
    while(ptr) {
        Node toDelete = ptr;
        ptr = ptr->next;
        free(toDelete);
    }
}


ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out) {





}


