//
// Created by Noam_ on 09/04/2020.
//
#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define INITIAL_SIZE 2


typedef struct element{
    char* key;
    char* value;
}*Element;

struct Map_t{
    int size;
    int maxSize;
    int iterator;
    Element dict;
};

Map mapCreate(){
    Map map = malloc(sizeof(*map));
    if(map == NULL){
        return NULL;
    }

    map->dict = malloc(INITIAL_SIZE * sizeof(struct element));
    if(map->dict == NULL){
        free(map);
        return NULL;
    }

    map->size = 0;
    map ->iterator = 0;
    map->maxSize = INITIAL_SIZE;
    return map;
}

void mapDestroy(Map map){
    if(map == NULL){
        return;
    }

    while(mapGetSize(map) > 0){
        mapRemove(map, mapGetFirst(map));
    }

    free(map->dict);
    free(map);
}

static MapResult addOrDestroy(Map map, const struct element index){
    MapResult result = mapPut(map, index.key, index.value);
    if(result == MAP_OUT_OF_MEMORY){
        mapDestroy(map);
    }
    return result;
}

static MapResult addAllOrDestroy(Map map, Map targetMap){
    for(int i = 0; i < targetMap->size; i++){
        if(addOrDestroy(map, targetMap->dict[i]) == MAP_OUT_OF_MEMORY){
            return MAP_OUT_OF_MEMORY;
        }
    }
    return MAP_SUCCESS;
}

Map mapCopy(Map map){
    if(map == NULL){
        return NULL;
    }

    Map newMap = mapCreate();
    if(newMap == NULL){
        return  NULL;
    }

    if(addAllOrDestroy(map, newMap) == MAP_OUT_OF_MEMORY){
        return NULL;
    }

    newMap->iterator = map->iterator;
    return newMap;
}

int mapGetSize(Map map){
    assert(map != NULL);
    int count = 0;








}










