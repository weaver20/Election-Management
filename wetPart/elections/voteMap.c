//
// Created by noam on 28/04/2020.
//

#include "voteMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"


#define INITIAL_VOTES 0
#define DELIMETER ":"
#define MAX_VOTE_SIZE 13
#define INITIAL_FACTOR 10



struct vote_t{
    Map votes;
    int total_num_of_votes;
};

static char* copyString(const char* str){
    char* new_str = malloc(strlen(str) + 1);
    if(new_str == NULL){
        return NULL;
    }
    return strcpy(new_str, str);
}

static int toInteger(char* str){
    assert(strlen(str) > 0);
    int factor = pow((double) INITIAL_FACTOR, (double) strlen(str) - 1);
    int number_of_votes = 0;

    char* iterator = str;
    while(*iterator != '\0'){
        number_of_votes += factor * (*iterator - '0');
        factor--;
        iterator++;
    }
    return number_of_votes;

}

static char* createVoteKeyElement(const char* area_id, const char* tribe_id){
    char* key_to_assign = malloc(strlen(tribe_id) + strlen(area_id) + 2);
    if(key_to_assign == NULL){
        return NULL;
    }
    char* delimeter = DELIMETER;
    char* area_copy = copyString(area_id);
    char* tribe_copy = copyString(tribe_id);
    strcat(key_to_assign, area_copy);
    strcat(key_to_assign, delimeter);
    strcat(key_to_assign, tribe_copy);

    free(area_copy);
    free(tribe_copy);
    return key_to_assign;
}

static char* createVoteValueElement(int number_of_votes){
    char* value_to_assign = malloc(MAX_VOTE_SIZE);
    if(value_to_assign == NULL){
        return NULL;
    }

    sprintf(value_to_assign, "%d", number_of_votes);
    return value_to_assign;
}

VoteMap voteCreate(){
    VoteMap votes = malloc(sizeof(*votes));
    if(votes == NULL){
        return NULL;
    }
    votes->votes = mapCreate();
    votes->total_num_of_votes = 0;
    return votes;
}

void voteDestroy(VoteMap votes){
     if(votes == NULL){
         return;
     }

     mapDestroy(votes->votes);
     free(votes);
}

VoteResult votePut(VoteMap votes, const char* tribe_id, const char* area_id){
    if(votes == NULL || tribe_id == NULL || area_id == NULL){
        return VOTE_NULL_ARGUMENT;
    }

    char* key_to_assign = createVoteKeyElement(area_id, tribe_id);
    char* value_to_assign = createVoteValueElement(INITIAL_VOTES);
    if(key_to_assign == NULL || value_to_assign == NULL){
        return VOTE_OUT_OF_MEMORY;
    }

#ifndef NDEBUG
    VoteResult result =
#endif
    mapPut(votes->votes, key_to_assign, value_to_assign);
    assert(result == MAP_SUCCESS);
    free(key_to_assign);
    return VOTE_SUCCESS;
}

VoteResult voteRemove(VoteMap votes, const char* name_id, const char* to_remove){
    if(votes == NULL || name_id == NULL){
        return VOTE_NULL_ARGUMENT;
    }

    if(strcmp(to_remove, TRIBE) !=0 || strcmp(to_remove, AREA) != 0){
        return VOTE_INVALID_INPUT;
    }

    char* delimeter = DELIMETER;
    int delete_count = 0;

    VOTE_FOREACH(iterator, votes->votes){
        char* key_copy = copyString(iterator);
        char* token = strtok(key_copy, delimeter);

        //in case a tribe ID supplied
        if(strcmp(to_remove, TRIBE) == 0){
            token = strtok(NULL, delimeter);
            if(strcmp(name_id, token) == 0){
                mapRemove(votes->votes, iterator);
                delete_count++; // if we've reached this phase then the item necessarily exists in the map
            }
        }

        //in case an area ID supplied
        if(strcmp(name_id, token) == 0){
            mapRemove(votes->votes, iterator);
            delete_count++; // if we've reached this phase then the item necessarily exists in the map
        }
        free(key_copy);
    }

    if(delete_count > 0){
        return VOTE_SUCCESS;
    }
    return VOTE_ITEM_DOES_NOT_EXIST;
}

VoteResult voteSet(VoteMap votes, const char* area_id, const char* tribe_id, int updated_votes_number){
    if(votes == NULL || tribe_id == NULL || area_id == NULL){
        return VOTE_NULL_ARGUMENT;
    }

    char* key = createVoteKeyElement(area_id, tribe_id);
    char* value = createVoteValueElement(updated_votes_number);

    if(key == NULL || value == NULL){
        return VOTE_OUT_OF_MEMORY;
    }

    if(mapPut(votes->votes, key, value) == MAP_OUT_OF_MEMORY){
        return VOTE_OUT_OF_MEMORY;
    }

    free(key);
    free(value);
    votes->total_num_of_votes = updated_votes_number;
    return VOTE_SUCCESS;
}

int voteGet(VoteMap votes, const char* area_id, const char* tribe_id){
    if(votes == NULL || area_id == NULL || tribe_id == NULL){
        return VOTE_NULL_ARGUMENT;
    }
    char* key = createVoteKeyElement(area_id, tribe_id);
    char* votes_number = mapGet(votes->votes, key); // We call this function when it's certain that the key element
                                                    // is in the map
    return toInteger(votes_number);
}






