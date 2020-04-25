//
// Created by aviram on 14/04/2020.
//

#define NOT_FOUND -1
#define MAX_LENGTH_OF_ITOA_OUTPUT 11
#define VOTE_SIZE 13 // 11 for itoa output + 2 for ":" and ";"
#define START_OF_NEW_VOTE ";"
#define START_OF_NUMBER_OF_VOTES ":"
#define DEFAULT_NUMBER_OF_VOTES "0"

#include "election.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// TODO: tip from forum: use strtok()

struct election_t {
    Map areas;
    Map tribes;


};
/* *********************************************************************************************** */
/* **************************************  HELPER FUNCTIONS ************************************** */
/* *********************************************************************************************** */

/*  *************  FIXED ************* */
static char* removeTribeFromArea(const char *old_value, const char *tribe_id);
static char *addVoteSlots(const char *area_name, Map tribes);
static char* addTribeToArea(const char* area_name, const char *tribe_id);
static bool isValidNameChar(const char toCheck);
static bool isValidIDChar(const char toCheck);
static bool isValidName(const char* name);
static bool isValidID(const char* name);
static void userItoa(int x, char* buffer);


static char * allocateAndCopyString(const char *old_value, int new_size) {
    char* new_value2 = malloc(new_size);
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
 * validate that the char is a-z or space
 * @param toCheck
 * @return
 */
static bool isValidIDChar(const char toCheck) {
    if(toCheck <= 'z' && toCheck >= 'a') {
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


static bool isValidID(const char* id) {
    int id_length = strlen(id);
    for(int i=0;i<id_length;i++)
    {
        if(!isValidIDChar(id[i])) {
            return false;
        }
    }
    return true;
}




/**
 * returns a copy of old_value string and reducts ";<tribe id>:<tribe votes>"
 * @param old_value
 * @param tribe_id
 * @return
 * pointer to a COPY of the new value if succeeded <br>
 * NULL in case of memory fail.
 */
// TODO: valgrind ends with disturbing errors (not memory leaks) - mini tester in playground

static char* removeTribeFromArea(const char *old_value, const char *tribe_id) {

    // copy the old value
    char* temp = allocateAndCopyString(old_value, strlen(old_value) + 1);
    if(temp == NULL) {
        return NULL;
    }
    // create delimeter: "tribeID:"
    char* delimeter = allocateAndCopyString(tribe_id, strlen(tribe_id) + 2);
    if(delimeter == NULL) {
        return NULL;
    }
    strcat(delimeter, START_OF_NUMBER_OF_VOTES);

    // find tribe votes in temp and skip them
    strtok(temp, delimeter);
    char* rest_of_value = strtok(NULL,START_OF_NEW_VOTE); // this will be NULL if tribeID is in the end

    // create the new value and copy from old everything before tribe_id votes
    int size_of_removed = strlen(tribe_id) + VOTE_SIZE;
    int new_size = strlen(old_value) +1 - size_of_removed;

    char* new_value = allocateAndCopyString(temp, new_size);
    if(new_value == NULL) {
        free(delimeter);
        return NULL;
    }
    // adds everything after tribe_id votes.
    strcat(new_value, rest_of_value);
    free(rest_of_value);
    return new_value;
}

/**
 * adds ;(tribe_id):0 at the end of area_name.
 * @param area_name
 * @param tribe_id
 * @return
 * pointer to updated area name
 * NULL if failed
 */
/*  *************  FIXED ************* */
static char* addTribeToArea(const char* area_name, const char *tribe_id) {
    assert(area_name != NULL && tribe_id != NULL);
    int new_size = strlen(area_name) + VOTE_SIZE + strlen(tribe_id);

    char* updated_area_name = allocateAndCopyString(area_name, new_size);
    if(updated_area_name == NULL) {
        return NULL;
    }
    strcat(updated_area_name,START_OF_NEW_VOTE);
    strcat(updated_area_name, tribe_id);
    strcat(updated_area_name,START_OF_NUMBER_OF_VOTES);
    strcat(updated_area_name,DEFAULT_NUMBER_OF_VOTES);

    return updated_area_name;
}
//TODO : rewrite itoa.
static void userItoa(int x, char* buffer) {
    itoa(x, buffer,10);
}

//TODO: change string FORMAT


/**
 * change "area_name" to "area_name;tribeID:0;......;tribeID:0"
 * @param area_name
 * @param tribes
 * @return
 * pointer to the updated area_name if succeeded. <br>
 * NULL if one of the parameters is NULL or allocation failed.
 */
static char* addVoteSlots(const char *area_name, Map tribes) {

    if (area_name == NULL || tribes == NULL) {
        return NULL;
    }

    char* updated_area_name = allocateAndCopyString(area_name, strlen(area_name) + 1);
    if(updated_area_name == NULL) {
        return NULL;
    }


    MAP_FOREACH(iterator, tribes) {
        char *tmp = addTribeToArea(updated_area_name, iterator);
        if (tmp == NULL) {
            return NULL;
        }
        free(updated_area_name);
        updated_area_name = allocateAndCopyString(tmp, strlen(tmp) + 1);
        if(updated_area_name == NULL) {
            free(tmp);
            return NULL;
        }
        free(tmp);
    }
    return updated_area_name;
}

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
    free(election);
}

// todo: can`t use itoa(tribe_id, buffer) - will not compile in csl3 - implement userItoa()

ElectionResult electionAddTribe (Election election, int tribe_id, const char* tribe_name) {

    if(tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if(tribe_name == NULL || election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    char* tribe_id_in_string = malloc(MAX_LENGTH_OF_ITOA_OUTPUT);
    userItoa(tribe_id,tribe_id_in_string); // todo: implement userItoa()

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

    // todo: should we check if mapPut... == MAP_NULL_ARG?????
    if(mapPut(election->tribes,tribe_id_in_string,tribe_name) == MAP_OUT_OF_MEMORY) {
        free(tribe_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    MAP_FOREACH(iterator,election->areas) {

        char* updated_area_name = addTribeToArea(mapGet(election->areas,iterator),tribe_id_in_string);
        if(updated_area_name == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        if(mapPut(election->areas,iterator,updated_area_name) == MAP_OUT_OF_MEMORY) {
            free(updated_area_name);
            return ELECTION_OUT_OF_MEMORY;
        }
        free(updated_area_name);
    }
    free(tribe_id_in_string);
    return ELECTION_SUCCESS;
}


ElectionResult electionAddArea(Election election, int area_id, const char* area_name) {

    if(area_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if(area_name == NULL || election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    char* area_id_in_string = malloc(MAX_LENGTH_OF_ITOA_OUTPUT);
    if(area_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    userItoa(area_id,area_id_in_string); // todo: implement userItoa()


    if(mapContains(election->areas,area_id_in_string)) {
        free(area_id_in_string);
        return ELECTION_AREA_ALREADY_EXIST;
    }

    if(!isValidName(area_name)) {
        free(area_id_in_string);
        return ELECTION_INVALID_NAME;
    }

    // Concatenating vote slots to the end of area name.
    char* area_name_with_votes = addVoteSlots(area_name,election->tribes);
    if(area_name_with_votes == NULL) {
        free(area_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }
    // updating the new area to the map
    if(mapPut(election->areas,area_id_in_string, area_name_with_votes) == MAP_OUT_OF_MEMORY) {
        free(area_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }


    free(area_id_in_string);
    free( area_name_with_votes);
    return ELECTION_SUCCESS;
}

char* electionGetTribeName (Election election, int tribe_id) {
    if(election == NULL || tribe_id <0) {
        return NULL;
    }

    char* tribe_id_in_string = malloc(MAX_LENGTH_OF_ITOA_OUTPUT);
    if(tribe_id_in_string == NULL) {
        return NULL;
    }
    userItoa(tribe_id,tribe_id_in_string);

    char* name_ptr = mapGet(election->tribes,tribe_id_in_string);
    if(name_ptr == NULL) {
        free(tribe_id_in_string);
        return NULL;
    }
    char* copy_of_name = allocateAndCopyString(name_ptr, strlen(name_ptr) + 1);
    if(copy_of_name == NULL) {
        free(tribe_id_in_string);
        return NULL;
    }

    free(tribe_id_in_string);
    return copy_of_name;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) {

    if(election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    if(tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    //TODO: how about a helper function to do all that?
    char* tribe_id_in_string = malloc(MAX_LENGTH_OF_ITOA_OUTPUT);
    if(tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    userItoa(tribe_id,tribe_id_in_string);
    //END OF HELPER FUNCTION?

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
    if(tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    // converting to string
    char* tribe_id_in_string = malloc(MAX_LENGTH_OF_ITOA_OUTPUT);
    if(tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    userItoa(tribe_id,tribe_id_in_string);

    if(mapRemove(election->tribes,tribe_id_in_string) == MAP_ITEM_DOES_NOT_EXIST) {
        free(tribe_id_in_string);
        return ELECTION_TRIBE_NOT_EXIST;
    }

    MAP_FOREACH(iterator, election->areas) {
        char* old_value = mapGet(election->areas,iterator);
        char* new_value = removeTribeFromArea(old_value,tribe_id_in_string);
        if (new_value == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        if(mapPut(election->areas,iterator,new_value) == MAP_OUT_OF_MEMORY) {
            free(new_value);
            return ELECTION_OUT_OF_MEMORY;
        }
        free(new_value);
    }
    free(tribe_id_in_string);
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) {
    if(isValidID("43")) {
        return ELECTION_SUCCESS;
    }
    return ELECTION_SUCCESS;
}





