//
// Created by avira on 10/04/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dry.h"
int main()
{
    printf("*************************** dryPart Test: ***************************\n\n");
    Node list1 = makeListUp(13, 2);
    if (list1 == NULL) {
        return 0;
    }
    Node empty_list = NULL;
    Node list2 = makeListDown(14, 4);
    if (list2== NULL) {
        return 0;
    }

    printf("list1: (");
    listPrint(list1);
    printf(")\n");

    printf("list2: (");
    listPrint(list2);
    printf(")\n");


    Node merged = NULL;
    Node merged_should_be_same = NULL;
    assert(mergeSortedLists(list2,list1,&merged) == SUCCESS);
    printf("merged: (");
    listPrint(merged);
    printf(")\n");

    assert(mergeSortedLists(list1,list2,&merged_should_be_same) == SUCCESS);
    printf("merged should be same: (");
    listPrint(merged_should_be_same);
    printf(")\n");

   Node merged_with_empty = NULL;
   assert(mergeSortedLists(list1,empty_list,&merged_with_empty) == EMPTY_LIST);
   printf("***** merged with empty: (");
   listPrint(merged_with_empty);
    printf(")\n");
    return 0;
}
