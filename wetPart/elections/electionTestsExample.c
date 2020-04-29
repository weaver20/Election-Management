#include <stdlib.h>
#include "election.h"
#include "../test_utilities.h"

/*The number of tests*/
#define NUMBER_TESTS 6
#define NPRINT


bool deleteOnlyFirstArea (int area_id) {
	return area_id == 1;
}
bool deleteNothing(int area_id) {
    return false;
}
bool deleteAll(int area_id) {
    return true;
}
bool deleteEvenNumbers(int area_id) {
    return area_id % 2 == 0;
}


bool testElectionRemoveAreas() {
	Election election = electionCreate();
	ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);

    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "first tribe") == ELECTION_SUCCESS);

	ASSERT_TEST(electionRemoveAreas(election, deleteOnlyFirstArea) == ELECTION_SUCCESS);
	ASSERT_TEST(electionAddVote(election, 1, 1,10) == ELECTION_AREA_NOT_EXIST);

	ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
	ASSERT_TEST(electionAddArea(election, 3, "third area") == ELECTION_SUCCESS);
	ASSERT_TEST(electionRemoveAreas(election, deleteAll) == ELECTION_SUCCESS);

	ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
	ASSERT_TEST(electionAddArea(election, 2, "first area") == ELECTION_SUCCESS);
	ASSERT_TEST(electionAddArea(election, 3, "third area") == ELECTION_SUCCESS);
	ASSERT_TEST(electionAddArea(election, 4, "forth area") == ELECTION_SUCCESS);
#ifndef NPRINT
    printElection(election,"4 areas, 2 tribe");
#endif
	ASSERT_TEST(electionRemoveAreas(election, deleteEvenNumbers) == ELECTION_SUCCESS);
#ifndef NPRINT
    printElection(election,"deleted even areas");
#endif
	ASSERT_TEST(electionAddVote(election, 1, 1,10) == ELECTION_SUCCESS);
	ASSERT_TEST(electionAddVote(election, 2, 1,10) == ELECTION_AREA_NOT_EXIST);
	ASSERT_TEST(electionAddVote(election, 3, 1,10) == ELECTION_SUCCESS);
	electionDestroy(election);
	return true;
}
bool testElectionAddTribe() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(NULL, 2, "elec is null") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddTribe(election, 2, NULL) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddTribe(election, -3, "negative id") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddTribe(election, 2, "already exists") == ELECTION_TRIBE_ALREADY_EXIST);
    ASSERT_TEST(electionAddTribe(election, 3, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 4, "INVAL!D NAME") == ELECTION_INVALID_NAME);

    // test with areas
    ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);

    ASSERT_TEST(electionAddTribe(election, 5, "fifth tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 6, "sixth tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 7, "seventh tribe") == ELECTION_SUCCESS);

    electionDestroy(election);
    return true;
}

bool testElectionAddArea() {
    Election election = electionCreate();
    //add areas with no tribes
    ASSERT_TEST(electionAddArea(election, 1, "first area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 2, "second area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(NULL, 2, "second area") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddArea(election, 2, NULL) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionAddArea(election, -3, "second area") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionAddArea(election, 2, "already exists") == ELECTION_AREA_ALREADY_EXIST);
    ASSERT_TEST(electionAddArea(election, 3, "second area") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 4, "INVAL!D NAME") == ELECTION_INVALID_NAME);

    //area with one tribe
    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 4, "area with one vote slot") == ELECTION_SUCCESS);

    //area with two tribes
    ASSERT_TEST(electionAddTribe(election, 2, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddArea(election, 5, "area with two vote slots") == ELECTION_SUCCESS);

    electionDestroy(election);
    return true;
}

bool testElectionGetTribeName() {
    Election election = electionCreate();
    char* test_ptr;
    ASSERT_TEST((test_ptr = electionGetTribeName(election, 1)) == NULL);
    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST((test_ptr = electionGetTribeName(NULL, 1)) == NULL);
    ASSERT_TEST((test_ptr = electionGetTribeName(election, -2)) == NULL);
    test_ptr = electionGetTribeName(election, 1);
    ASSERT_TEST_WITH_FREE(strcmp(test_ptr ,"first tribe") == 0, free(test_ptr));
    test_ptr = electionGetTribeName(election, 2);
    ASSERT_TEST_WITH_FREE(strcmp(test_ptr ,"first tribe") != 0, free(test_ptr));
    ASSERT_TEST((test_ptr = electionGetTribeName(election, 3)) == NULL);

    electionDestroy(election);
    return true;
}

bool testElectionSetTribeName() {
    Election election = electionCreate();

    ASSERT_TEST(electionSetTribeName(election, 1, "not exist") == ELECTION_TRIBE_NOT_EXIST);

    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, 1, "first tribe new") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 2, "second tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionAddTribe(election, 3, "third tribe") == ELECTION_SUCCESS);
    ASSERT_TEST(electionSetTribeName(election, 4, "first tribe new") == ELECTION_TRIBE_NOT_EXIST);
    ASSERT_TEST(electionSetTribeName(NULL, 4, "elec is null") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionSetTribeName(election, 4, NULL) == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionSetTribeName(election, -3, "negative id") == ELECTION_INVALID_ID);
    ASSERT_TEST(electionSetTribeName(election, 2, "INVAL!D NAME") == ELECTION_INVALID_NAME);
    ASSERT_TEST(electionSetTribeName(NULL, 2, "INVAL!D NAME and NULL") == ELECTION_NULL_ARGUMENT);
    ASSERT_TEST(electionSetTribeName(election, 8, "INVAL!D NAME and not exists") == ELECTION_TRIBE_NOT_EXIST);


    electionDestroy(election);
    return true;
}

bool testElectionRemoveTribe() {
    Election election = electionCreate();
    ASSERT_TEST(electionAddTribe(election, 1, "first tribe") == ELECTION_SUCCESS);




    return true;
}
/*The functions for the tests should be added here*/
bool (*tests[]) (void) = {
                      testElectionRemoveAreas,
                      testElectionAddTribe,
                      testElectionAddArea,
                      testElectionGetTribeName,
                      testElectionSetTribeName,
                      testElectionRemoveTribe
};

/*The names of the test functions should be added here*/
const char* testNames[] = {
                           "testElectionRemoveAreas",
                           "testElectionAddTribe",
                           "testElectionAddArea",
                           "testElectionGetTribeName",
                           "testElectionSetTribeName",
                           "testElectionRemoveTribe"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
              RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
      fprintf(stdout, "Usage: election <test index>\n");
      return 0;
  }

  int test_idx = strtol(argv[1], NULL, 10);
  if (test_idx < 1 || test_idx > NUMBER_TESTS) {
      fprintf(stderr, "Invalid test index %d\n", test_idx);
      return 0;
  }

  RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
  return 0;
}
