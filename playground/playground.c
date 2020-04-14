//
// Created by avira on 10/04/2020.
//

//
// Created by aviram on 14/04/2020.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

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


char *stringDuplicator(char *s, int times) { // CONVENTION ERROR 1: s should be str
    assert(!s);                   // PROGRAMMING ERROR 1: should be assert(s)
    assert(times > 0);
    int LEN = strlen(s);             // CONVENTION ERROR 2: LEN - variable names should be in lower case
    char *out = malloc(LEN * times);
    assert(out);
    char* tmp = out;// PROGRAMMING ERROR 2: should be if(!out) { return NULL; }
    for (int i = 0; i < times; i++) {
        // this line increment the pointer. -
        strcpy(out, s);                 // PROGRAMMING ERROR 4: should use strcat().
        out = out + LEN;                // PROGRAMMING ERROR 3: out is a pointer to char -
    }
    puts(tmp);
    puts("TEST");
    return out;
}

int main()
{
    puts(stringDuplicate("Hey",3));
}
