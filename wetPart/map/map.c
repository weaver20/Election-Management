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
    Element dictionary;
};

static int elementIndex(Map map, const char* key){
    for(int i = 0; i < map->size; i++){
        if(strcmp(map->dictionary[i].key, key) == 0){
            return i;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

static MapResult expand(Map map){
    int newSize = EXPAND_FACTOR * map->maxSize;
    Element newElements = realloc(map->dictionary, newSize * sizeof(struct element));
    if(newElements == NULL){
        return MAP_OUT_OF_MEMORY;
    }

    map->dictionary = newElements;
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

static MapResult removeElement(Map map, int index){

    free(map->dictionary[index].key);
    free(map->dictionary[index].value);
    for(int i = index; i < map->size - 1 ; i++){
        map->dictionary[i] = map->dictionary[i + 1];
    }
/*

    //TODO: this frees dictionary[size-2] also!!! (dictionary is an array of pointers)
    if(index != map->size - 1){
        free(map->dictionary[map->size - 1].key);
        free(map->dictionary[map->size - 1].value);
    }
*/

    Element tmp = realloc(map->dictionary, (map->maxSize - 1) * sizeof(*tmp));
    if(tmp == NULL){
        return MAP_OUT_OF_MEMORY;
    }

    map->size--;
    map->dictionary = tmp;
    return MAP_SUCCESS;
}

Map mapCreate(){
    Map map = malloc(sizeof(*map));
    if(map == NULL){
        return NULL;
    }
// TODO: why not sizeof(*(map->dictionary))? (line 60)
    map->dictionary = malloc(INITIAL_SIZE * sizeof(*(map->dictionary)));
    if(map->dictionary == NULL){
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
    // TODO: Free key and value
    free(map->dictionary);
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
        if(addOrDestroy(map, targetMap->dictionary[i]) == MAP_OUT_OF_MEMORY){
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

    //check if key exists in map
    if(mapContains(map, key)){
        if(strcmp(map->dictionary[elementIndex(map,key)].value, data) == 0){
            return MAP_ITEM_ALREADY_EXISTS;
        }

        else{
            char* new_value = copyString(data);
            if(new_value == NULL){
                return MAP_OUT_OF_MEMORY;
            }
            map->dictionary[elementIndex(map,key)].value = new_value;
            return MAP_SUCCESS;
        }
    }
    // expands map if necessary
    if(map->size == map->maxSize){
        if(expand(map) == MAP_OUT_OF_MEMORY){
            return MAP_OUT_OF_MEMORY;
        }
    }
    //TODO: why create new_element? there already is an element in dictionary waiting for key and value!
    char* new_key = copyString(key);
    char* new_value = copyString(data);
    if(new_value == NULL || new_key == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    map->dictionary[map->size].key = new_key;
    map->dictionary[map->size++].value = new_value;
/*
    // add element
    Element new_element = malloc(sizeof(*new_element));
    char* new_key = copyString(key);
    char* new_value = copyString(data);
    if(new_element == NULL ||new_value == NULL || new_key == NULL){
        return MAP_OUT_OF_MEMORY;
    }
    new_element->key = new_key;
    new_element->value = new_value;
    //update map size
    map->dictionary[map->size++] = *new_element;
    free(new_element);*/
    return MAP_SUCCESS;
}

char* mapGet(Map map, const char* key){
    if(map == NULL || key == NULL || !mapContains(map, key)){
        return NULL;
    }

    return map->dictionary[elementIndex(map, key)].value;
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

    return removeElement(map, index);
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

    return map->dictionary[map->iterator++].key;
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
        printf("%s : %s ", iterator, map->dictionary[elementIndex(map, iterator)].value);
    }
}






