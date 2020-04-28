//
// Created by avira on 28/04/2020.
//

#ifndef EX_1_VOTEMAP_H
#define EX_1_VOTEMAP_H

#include "mtm_map/map.h"

typedef struct vote_t* VoteMap;

typedef enum VoteResult_t {
    VOTE_SUCCESS,
    VOTE_OUT_OF_MEMORY,
    VOTE_NULL_ARGUMENT,
    VOTE_ITEM_DOES_NOT_EXIST,
} VoteResult;



VoteMap voteCreate();

void voteDestrroy(VoteMap toDestroy);

VoteResult voteAdd(VoteMap votes, const char* tribe_id, const char* area_id);

VoteResult voteRemoveTribe(VoteMap votes, const char* tribe_id);

VoteResult voteRemoveArea(VoteMap votes, const char* area_id);

VoteResult voteSet(VoteMap votes, const char* tribe_id, const char* area_id, const char* num_of_votes);

VoteResult  voteGet(VoteMap votes, const char* tribe_id, const char* area_id);




#endif //EX_1_VOTEMAP_H
