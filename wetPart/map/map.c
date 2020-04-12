//
// Created by Noam_ on 09/04/2020.
//
#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define INITIAL_SIZE 2
#define EXPAND_FACTOR 2


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

static int elementIndex(Map map, const char* key){
    for(int i = 0; i < map->size; i++){
        if(strcmp(map->dict[i].key, key) == 0){
            return i;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

static MapResult expand(Map map){
    int newSize = EXPAND_FACTOR * map->maxSize;
    Element newElements = realloc(map->dict, newSize * sizeof(struct element));
    if(newElements == NULL){
        return MAP_OUT_OF_MEMORY;
    }

    map->dict = newElements;
    map->maxSize = newSize;

    return MAP_SUCCESS;
}

static char* copyString(const char* str){
    char* newStr = malloc(strlen(str) + 1);
    if(newStr == NULL){
        return NULL;
    }
    return strcpy(newStr, str);
}

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
    if(map == NULL){
        return -1;
    }
    return map->size;
}

bool mapContains(Map map, const char* key){
    if(map == NULL || key == NULL){
        return false;
    }

    MAP_FOREACH(iterator,map){
        if(strcmp(iterator, key) == 0)
            return true;
    }
    return false;
}

MapResult mapPut(Map map, const char* key, const char* data){
    if( map == NULL || key == NULL || data == NULL){
        return MAP_NULL_ARGUMENT;
    }

    if(mapContains(map, key)){

        if(strcmp(map->dict[elementIndex(map,key)].value, data) == 0){
            return MAP_ITEM_ALREADY_EXISTS;
        }

        else{
            char* new_value = copyString(data);
            if(new_value == NULL){
                return MAP_OUT_OF_MEMORY;
            }
            free(map->dict[elementIndex(map,key)].value);
            map->dict[elementIndex(map,key)].value = new_value;
            return MAP_SUCCESS;
        }
    }

    if(map->size == map->maxSize){
        if(expand(map) == MAP_OUT_OF_MEMORY){
            return MAP_OUT_OF_MEMORY;
        }
    }
    Element new_element = malloc(sizeof(struct element));
    char* new_key = copyString(key);
    char* new_value = copyString(data);
    if(new_value == NULL || new_key == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    new_element->key = new_key;
    new_element->value = new_value;

    map->dict[map->size++] = *new_element;
    return MAP_SUCCESS;
}

char* mapGet(Map map, const char* key){
    if(map == NULL || key == NULL || !mapContains(map, key)){
        return NULL;
    }

    return map->dict[elementIndex(map, key)].value;
}

MapResult mapRemove(Map map, const char* key){
    assert(map != NULL);
    if(key == NULL){
        return MAP_NULL_ARGUMENT;
    }

    int index = elementIndex(map, key);
    if(index == MAP_ITEM_DOES_NOT_EXIST){
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    free(map->dict[index].key);
    free(map->dict[index].value);
    free(&(map->dict[index]));
    map->dict[index] = map->dict[map->size - 1];

    map->size--;
    return MAP_SUCCESS;
}

char* mapGetFirst(Map map){
    if(map == NULL){
        return NULL;
    }

    map->iterator = 0;
    return mapGetNext(map);
}

char* mapGetNext(Map map){
    if(map == NULL){
        return NULL;
    }

    if(map->iterator >= map->size){
        return NULL;
    }

    return map->dict[map->iterator++].key;
}

MapResult mapClear(Map map){
    if(map == NULL){
        return MAP_NULL_ARGUMENT;
    }

    while(mapGetSize(map) > 0){
        mapRemove(map, mapGetFirst(map));
    }

    return MAP_SUCCESS;
}

void mapPrint(Map map){
    printf("The following map contains the following elements in the format of key : value\n ");
    MAP_FOREACH(iterator, map){
        printf("%s : %s ", iterator, map->dict[elementIndex(map, iterator)].value);
    }
}






