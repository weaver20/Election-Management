//
// Created by avira on 10/04/2020.
//
#include <stdio.h>
#include <stdlib.h>
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
    mergeSortedLists(list1,list2,&merged);

    printf("merged: (");
    listPrint(merged);
    printf(")\n");

    Node merged_with_empty = NULL;
    mergeSortedLists(list1,empty_list,&merged_with_empty);
    printf("merged with empty: (");
    listPrint(merged_with_empty);
    printf(")\n");
    return 0;
}
