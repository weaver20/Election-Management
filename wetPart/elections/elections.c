//
// Created by aviram on 14/04/2020.
//

#define NOT_FOUND -1
#define MAX_LENGTH_OF_ITOA_OUTPUT 11
#define VOTE_SIZE 12
#define VOTE ":0"
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
static ElectionResult addTribeToAreas(Map areas);
static void userItoa(int x, char* buffer);
static char *addVoteSlots(const char *name, int slots);
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

/**
 * adds  ":0" at the end of value strings of all areas
 * @param areas
 * @return
 * ELECTION_SUCCESS if succeeded doing so.
 * ELECTION_OUT_OF_MEMORY if one of the reallocation failed.
 */
static ElectionResult addTribeToAreas(Map areas) {

    MAP_FOREACH(iterator,areas) {
        char* new_value = realloc(iterator, sizeof(*iterator) + VOTE_SIZE);
        if(new_value == NULL) {
            return ELECTION_OUT_OF_MEMORY;
        }
        iterator = new_value;
        strcat(iterator,":0");
    }
    return ELECTION_SUCCESS;
}
//TODO : rewrite itoa.
static void userItoa(int x, char* buffer) {
    itoa(x, buffer,10);
}


static char* addVoteSlots(const char *name, int slots) {

    if(name == NULL || slots < 0) {
        return NULL;
    }

    char* new_name = malloc (sizeof(*name) + (slots * VOTE_SIZE));
    if(new_name == NULL) {
        return NULL;
    }
    //adding Vote Slots
    strcpy(new_name,name);
    for(int i=0;i<slots;i++) {
        strcat(new_name,VOTE);
    }
    return new_name;
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
    // dont need this anymore.
    free(tribe_id_in_string);


   if(addTribeToAreas(election->areas) == ELECTION_OUT_OF_MEMORY) {
       return ELECTION_OUT_OF_MEMORY;
   }

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
    userItoa(area_id,area_id_in_string); // todo: implement userItoa()

    if(area_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if(mapContains(election->areas,area_id_in_string)) {
        free(area_id_in_string);
        return ELECTION_TRIBE_ALREADY_EXIST;
    }

    if(!isValidName(area_name)) {
        free(area_id_in_string);
        return ELECTION_INVALID_NAME;
    }

    // Concatenating vote slots to the end of area name.
    char* area_name_with_votes = addVoteSlots(area_name, mapGetSize(election->tribes));
    if(area_name_with_votes == NULL) {
        free(area_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }

    if(mapPut(election->areas,area_id_in_string, area_name_with_votes) == MAP_OUT_OF_MEMORY) {
        free(area_id_in_string);
        return ELECTION_OUT_OF_MEMORY;
    }
    // dont need this anymore.
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
        return NULL;
    }
    char* copy_of_name = malloc(sizeof(*name_ptr));
    strcpy(copy_of_name,name_ptr);
    free(tribe_id_in_string);

    return copy_of_name;
}

ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name) {

    if(election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    if(tribe_id <0) {
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


//TODO: finish removeTribe
ElectionResult electionRemoveTribe (Election election, int tribe_id) {
    if(election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if(tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    char* tribe_id_in_string = malloc(MAX_LENGTH_OF_ITOA_OUTPUT);
    if(tribe_id_in_string == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    userItoa(tribe_id,tribe_id_in_string);

    int index = getIndex(election->tribes,tribe_id_in_string);
    if(index == NOT_FOUND) {
        free(tribe_id_in_string);
        return ELECTION_TRIBE_NOT_EXIST;
}
    free(tribe_id_in_string);

    MAP_FOREACH(iterator, election->areas) {
        char* old_value = mapGet(election->areas,iterator);
       char* new_value = removeTribeFromArea(old_value,index);
       if (new_value == NULL) {
           return ELECTION_OUT_OF_MEMORY;
       }
       if(mapPut(election->areas,iterator,new_value) == MAP_OUT_OF_MEMORY) {
           return ELECTION_OUT_OF_MEMORY;
       }
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) {
    if(isValidID("43")) {
        return ELECTION_SUCCESS;
    }
    return ELECTION_SUCCESS;
}





