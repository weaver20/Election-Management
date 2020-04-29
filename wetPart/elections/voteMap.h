//
// Created by avira on 28/04/2020.
//

#ifndef EX_1_VOTEMAP_H
#define EX_1_VOTEMAP_H
#define AREA "area"
#define TRIBE "tribe"

#include "mtm_map/map.h"

typedef struct vote_t *VoteMap;

typedef enum VoteResult_t {
    VOTE_SUCCESS,
    VOTE_OUT_OF_MEMORY,
    VOTE_NULL_ARGUMENT,
    VOTE_ITEM_DOES_NOT_EXIST,
    VOTE_INVALID_INPUT
} VoteResult;



VoteMap voteCreate(); // Done!t

void voteDestroy(VoteMap toDestroy); // Done!

VoteResult votePut(VoteMap votes, const char* tribe_id, const char* area_id); // Done!

VoteResult voteRemove(VoteMap votes, const char* name_id, const char* to_remove); // Done!

VoteResult voteSet(VoteMap votes, const char* tribe_id, const char* area_id, int updated_votes_number); // Done!

int voteGet(VoteMap votes, const char* area_id, const char* tribe_id); // Done!

#define VOTE_FOREACH(iterator, map) \
    for(char* (iterator) = (char*) mapGetFirst(map) ; \
        (iterator) ;\
        (iterator) = mapGetNext(map))


#endif //EX_1_VOTEMAP_H
