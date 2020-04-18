//
// Created by aviram on 14/04/2020.
//

#define NOT_FOUND -1
#define MAX_LENGTH_OF_ITOA_OUTPUT 11
#define  DECIMAL 10
#include "election.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// TODO: tip from forum: use strtok()

struct election_t {
    Map areas;
    Map tribes;


};
/* *********************************************************************************************** */
/* **************************************  HELPER FUNCTIONS ************************************** */
/* *********************************************************************************************** */

static bool isValidNameChar(const char toCheck);
static bool isValidIDChar(const char toCheck);
static bool isValidName(const char* name);
static bool isValidID(const char* name);
static int getIndex(Map map, const char* key);
static char* removeTribeFromArea(char* value, int index);
static ElectionResult addTribeToAreas(Map areas, char *key);

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
 * returns the index of the key in the map
 * @param map
 * @param key
 * @return
 *  -1 if not found
 */
static int getIndex(Map map, const char* key) {
    char* id_ptr = mapGetFirst(map);
    int i=0;
    while(id_ptr) {
        if(strcmp(key, id_ptr) == 0) {
            return i;
        }
        id_ptr = mapGetNext(map);
        i++;
    }
    return NOT_FOUND;
}
/**
 * returns a copy of value string and reducts ":num_of_votes_of_tribe[index]"
 * @param value
 * @param index
 * @return
 */
 // TODO: valgrind ends with disturbing errors (not memory leaks) - mini tester in playground
static char* removeTribeFromArea(char* value, int index) {
    char* spacer = ":";
    char* tmp = malloc(sizeof(value)+1);
    if(tmp == NULL) {
        return NULL;
    }
    char* token;
    strcpy(tmp,strtok(value, spacer));        // skipping the area name
    token = strtok(NULL, spacer);     // getting first token
    for(int i=0; token ; i++ , token = strtok(NULL, spacer)) {
        if(i == index) {
            continue;
        }
        strcat(tmp,spacer);
        strncat(tmp,token,strlen(token));
        //     printf("iteration #%d: %s\n",i, tmp);
    }
    //  printf("end of loop: %s\n", tmp);
    int new_size = strlen(tmp) + 1;
    char* final = realloc(tmp, new_size);
    if(final == NULL) {
        return NULL;
    }
    return final;
}


static ElectionResult addTribeToAreas(Map areas, char *key) {

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
    new_election->num_of_areas=0;
    new_election->num_of_tribes=0;
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
    itoa(tribe_id,tribe_id_in_string, DECIMAL); // todo: change to userItoa()

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

   addTribeToAreas(election->areas,tribe_id_in_string);
    //TODO: check return values of this function

}













