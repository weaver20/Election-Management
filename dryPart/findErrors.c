//
// Created by aviram on 14/04/2020.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>


char *stringDuplicator(char *s, int times) {
    // PROGRAMMING ERROR 1: s should be transferred to the function as const to
    // CONVENTION ERROR 1: s should be str
    // CONVENTION ERROR 2: func name should be a verb
    assert(!s);
    // PROGRAMMING ERROR 2: should be assert(s)
    assert(times > 0);
    int LEN = strlen(s);
    // CONVENTION ERROR 3: LEN - variable names should be in lower case
    // PROGRAMMING ERROR 3:we should allocate an additional bye for the /0  (strlen() returns the length of the string without the /0 char)
    char *out = malloc(LEN * times);
    assert(out);
    // PROGRAMMING ERROR 4: should be if(!out) { return NULL; }
    for (int i = 0; i < times; i++) {
    // CONVENTION ERROR 4: no indent lines in for loop
    out = out + LEN;
    // PROGRAMMING ERROR 5: this two lines should flip - first copy, than increment pointer
    strcpy(out, s);
    }
    return out;
    // PROGRAMMING ERROR 6: returning a pointer to the end of out string
}


                    // **********  Fixed: **********


char* stringDuplicate2(const char *str, int times) {
    assert(str);
    assert(times > 0);
    char* out = malloc(strlen(str) * (times+1));
    if(!out) {
        return NULL;
    }
    for (int i = 0; i < times; i++) {
        strcat(out, str);
    }
    return out;
}
