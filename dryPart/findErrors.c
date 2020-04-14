//
// Created by aviram on 14/04/2020.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>


char *stringDuplicator(char *s, int times) {
    // CONVENTION ERROR 1: s should be str
    // CONVENTION ERROR 2: func name should be a verb
    assert(!s);
    // PROGRAMMING ERROR 1: should be assert(s)
    assert(times > 0);
    int LEN = strlen(s);
    // CONVENTION ERROR 3: LEN - variable names should be in lower case
    char *out = malloc(LEN * times);
    assert(out);
    // PROGRAMMING ERROR 2: should be if(!out) { return NULL; }
    for (int i = 0; i < times; i++) {
    // CONVENTION ERROR 4: no indent lines in for loop
    out = out + LEN;
    // PROGRAMMING ERROR 3: this two lines should flip - first copy, than increment pointer
    strcpy(out, s);
    }
    return out;
    // PROGRAMMING ERROR 4: returning a pointer to the end of out string
}


                    // **********  Fixed: **********

#include <stdlib.h>
#include <string.h>
#include <assert.h>


char* stringDuplicate(char *str, int times) {
    assert(str);
    assert(times > 0);
    char* out = malloc(strlen(str) * times);
    if(!out) {
        return NULL;
    }
    for (int i = 0; i < times; i++) {
        strcat(out, str);
    }
    return out;
}
