//
// Created by aviram on 14/04/2020.
//

#define NOT_FOUND -1
#define MAX_LENGTH_OF_INT 11
#define VOTE_SIZE 13 // 11 for itoa output + 2 for ":" and ";"
#define START_OF_NEW_VOTE ";"
#define START_OF_NUMBER_OF_VOTES ":"
#define DEFAULT_NUMBER_OF_VOTES "0"
#define DECIMAL 10
#define ROOM_FOR_NULL_TERMINATOR 1

#include "election.h"
#include "voteMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>



struct election_t {
    Map areas;
    Map tribes;
    VoteMap votes;
};
/* *********************************************************************************************** */
/* **************************************  HELPER FUNCTIONS ************************************** */
/* *********************************************************************************************** */

static bool isValidNameChar(const char toCheck);
static bool isValidName(const char* name);
static bool isValidID(int id);
static char* allocateAndItoa(int x);


static char* allocateAndCopyString(const char *old_value, int new_size) {
    char* new_value2 = malloc(new_size + 1);
    if (new_value2 == NULL) {
        return NULL;
    }
    strcpy(new_value2,old_value);
    return new_value2;
}
/**
 * validate that the char is a-z or space
 * @param toCheck
 * @return
 */
static bool isValidNameChar(const char toCheck) {
    if( (toCheck <= 'z' && toCheck >= 'a')
        || toCheck == ' ' ) {
        return true;
    }
    return false;
}

/**
 * validate that name contains a-z and spaces only!
 * @param name
 * @return
 */
static bool isValidName(const char* name) {
    int name_length = strlen(name);
    for(int i=0;i<name_length;i++)
    {
        if(!isValidNameChar(name[i])) {
            return false;
        }
    }
    return true;
}


static bool isValidID(int id) {
    if(id < 0) {
        return false;
    }
    return true;
}


/**
 * adds ;(tribe_id):0 at the end of area_name.
 * @param area_name
 * @param tribe_id
 * @return
 * pointer to updated area name
 * NULL if failed
 */

static char* allocateAndItoa(int x) {
    if(x < 0) {
        return NULL;
    }
    char* x_in_string = malloc(MAX_LENGTH_OF_INT);
    if(x_in_string == NULL) {
        return NULL;
    }
    sprintf(x_in_string,"%d",x);
    return x_in_string;
}

//TODO: change string FORMAT


/* *********************************************************************************************** */
/* **********************************  END OF HELPER FUNCTIONS *********************************** */
/* *********************************************************************************************** */
/**
 * creates a new election system
 * @return
 *  new Election pointer
 *  NULL if error
 */
Election electionCreate() {
    Election new_election = malloc(sizeof(*new_election));
    if(new_election == NULL) {
        return NULL;
    }
    new_election->areas = mapCreate();
    new_election->tribes = mapCreate();
    new_election->votes = voteCreate();
    return new_election;
}
/**
 * destroys election
 * @param election - to destroy
 */
void electionDestroy(Election election) {
    if(election == NULL) {
        return;
    }
    mapDestroy(election->tribes);
    mapDestroy(election->areas);
    voteDestroy(election->votes);
    free(election);
}



ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name) {

    if(isValidID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }
    if(tribe_name == NULL || election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    char* tribe_id_in_string = allocateAndItoa(tribe_id);
    if(tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(mapContains(election->tribes,tribe_id_in_string)) {
        free(tribe_id_in_string);
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if(!isValidName(tribe_name)) {
        free(tribe_id_in_string);
        return ELECTION_INVALID_NAME;
    }

    if(mapPut(election->tribes,tribe_id_in_string,tribe_name) == MAP_OUT_OF_MEMORY) {
        free(tribe_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    MAP_FOREACH(iterator,election->areas) {

        if(votePut(election->votes, tribe_id_in_string, iterator)
           == VOTE_OUT_OF_MEMORY) {
            free(tribe_id_in_string);
            return ELECTION_OUT_OF_MEMORY;
        }
    }
    free(tribe_id_in_string);
    return ELECTION_SUCCESS;
}


ElectionResult electionAddArea(Election election, int area_id, const char* area_name) {

    if(isValidID(area_id)) {
        return ELECTION_INVALID_ID;
    }
    if(area_name == NULL || election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    char* area_id_in_string = allocateAndItoa(area_id);
    if(area_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    if(mapContains(election->areas,area_id_in_string)) {
        free(area_id_in_string);
        return ELECTION_AREA_ALREADY_EXIST;
    }

    if(!isValidName(area_name)) {
        free(area_id_in_string);
        return ELECTION_INVALID_NAME;
    }

    MAP_FOREACH(iterator,election->tribes) {
        if(votePut(election->votes,iterator,area_id_in_string)
           == VOTE_OUT_OF_MEMORY) {
            free(area_id_in_string);
            return ELECTION_OUT_OF_MEMORY;
        }
    }
    free(area_id_in_string);
    return ELECTION_SUCCESS;
}


char* electionGetTribeName (Election election, int tribe_id) {

    if(election == NULL || tribe_id <0) {
        return NULL;
    }

    char* tribe_id_in_string = allocateAndItoa(tribe_id);
    if(tribe_id_in_string == NULL) {
        return NULL;
    }
    char* name = mapGet(election->tribes, tribe_id_in_string);
    if(name == NULL) {
        free(tribe_id_in_string);
        return NULL;
    }
    char* copy_of_name = allocateAndCopyString(name, strlen(name) + 1);
    if(copy_of_name == NULL) {
        free(tribe_id_in_string);
        return NULL;
    }

    free(tribe_id_in_string);
    return copy_of_name;
}

ElectionResult electionAddVote (Election election, int area_id, int tribe_id, int num_of_votes) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if(isValidID(tribe_id) || isValidID(area_id)) {
        return ELECTION_INVALID_ID;
    }
    if(num_of_votes <= 0 ) {
        return ELECTION_INVALID_VOTES;
    }
    char* area_id_in_string = allocateAndItoa(area_id);
    char* tribe_id_in_string = allocateAndItoa(tribe_id);
    if(area_id_in_string == NULL ||
       tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(!mapContains(election->areas,area_id_in_string)) {
        free(area_id_in_string);
        free(tribe_id_in_string);
        return ELECTION_AREA_NOT_EXIST;
    }
    if(!mapContains(election->tribes,tribe_id_in_string)) {
        free(area_id_in_string);
        free(tribe_id_in_string);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    int current_num_of_votes = voteGet(election->votes,tribe_id_in_string,area_id_in_string);
    //if(current_num_of_votes == -1) { ERROR??} ??

    int updated_votes = current_num_of_votes + num_of_votes;
    if(voteSet(election->votes,tribe_id_in_string,area_id_in_string,updated_votes)
       == VOTE_OUT_OF_MEMORY) {
        free(area_id_in_string);
        free(tribe_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    free(area_id_in_string);
    free(tribe_id_in_string);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if(isValidID(tribe_id)|| isValidID(area_id)) {
        return ELECTION_INVALID_ID;
    }
    if(num_of_votes <= 0 ) {
        return ELECTION_INVALID_VOTES;
    }
    char* area_id_in_string = allocateAndItoa(area_id);
    char* tribe_id_in_string = allocateAndItoa(tribe_id);
    if(area_id_in_string == NULL ||
       tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(!mapContains(election->areas,area_id_in_string)) {
        free(area_id_in_string);
        free(tribe_id_in_string);
        return ELECTION_AREA_NOT_EXIST;
    }
    if(!mapContains(election->tribes,tribe_id_in_string)) {
        free(area_id_in_string);
        free(tribe_id_in_string);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    int current_votes = voteGet(election->votes,tribe_id_in_string,area_id_in_string);
    int new_votes = current_votes - num_of_votes;
    if(new_votes < 0) {
        new_votes = 0;
    }

    if(voteSet(election->votes,tribe_id_in_string,area_id_in_string,new_votes)
       == VOTE_OUT_OF_MEMORY) {
        free(area_id_in_string);
        free(tribe_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    free(area_id_in_string);
    free(tribe_id_in_string);
    return ELECTION_SUCCESS;

}


ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) {

    if(election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    if(isValidID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }

    char* tribe_id_in_string = allocateAndItoa(tribe_id);
    if(tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    if(!mapContains(election->tribes,tribe_id_in_string)) {
        free(tribe_id_in_string);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    if(!isValidName(tribe_name)) {
        free(tribe_id_in_string);
        return ELECTION_INVALID_NAME;
    }
    if(mapPut(election->tribes,tribe_id_in_string,tribe_name) == MAP_OUT_OF_MEMORY) {
        free(tribe_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    free(tribe_id_in_string);
    return ELECTION_SUCCESS;
}


/**
 * removes tribe and all votes data of that tribe  from the system
 * @param election
 * @param tribe_id
 * @return
 *
 */
ElectionResult electionRemoveTribe (Election election, int tribe_id) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if(isValidID(tribe_id)) {
        return ELECTION_INVALID_ID;
    }
    char* tribe_id_in_string = allocateAndItoa(tribe_id);
    if(tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }

    if(mapRemove(election->tribes,tribe_id_in_string) == MAP_ITEM_DOES_NOT_EXIST) {
        free(tribe_id_in_string);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    MAP_FOREACH(iterator, election->areas) {
#ifndef NDEBUG
        VoteResult result =
#endif
        voteRemove(election->votes,iterator,tribe_id_in_string);
        assert(result == VOTE_SUCCESS);
    }
    free(tribe_id_in_string);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) {

    if(election == NULL || should_delete_area == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    bool removed_an_area = false;
    do {
        MAP_FOREACH(iterator, election->areas) {
            removed_an_area = false;
            int int_id = atoi(iterator);
            if(should_delete_area(int_id)) {
                mapRemove(election->areas, iterator);
                removed_an_area = true;
                break;
            }

        }
    }while(removed_an_area);


    return ELECTION_SUCCESS;
}





