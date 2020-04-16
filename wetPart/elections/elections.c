//
// Created by aviram on 14/04/2020.
//

#include "election.h"
#include <stdio.h>

// TODO: tip from forum: use strtok()

struct election_t {
    Map areas;
    Map tribes;
    // todo: do we really need those?
    int num_of_tribes;
    int num_of_areas;

};

/**
 * validate that name contains a-z and spaces only!
 * @param name
 * @return
 */
static ElectionResult validateName(const char* name);

/**
 * validate that the char is a-z or space
 * @param toCheck
 * @return
 */
static bool isValidChar(char toCheck);
/**
 * returns the index of the key in the map
 * @param map
 * @param key
 * @return
 *  -1 if not found
 */
static int getIndex(Map map, const char* key);
/**
 * removes ":num_of_votes_of_tribe[index]" from value string
 * @param value
 * @param index
 * @return
 */
static ElectionResult removeVotesFromString(char* value, int index);
