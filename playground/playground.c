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

char *stringDuplicator(char *s, int times) { // PROGRAMMING ERROR: should be char* stringDuplicator(char *s, int times)
    assert(!s);
    assert(times > 0);
    int LEN = strlen(s); // CONVENTION ERROR: LEN - variable names should be in lower case
    char *out = malloc(LEN * times); // PROGRAMMING ERROR: should be: malloc(sizeof(LEN) * times)
    assert(out);                           // PROGRAMMING ERROR: should be if(!out) { **end with error**}
    for (int i = 0; i < times; i++) {
        strcat(out, s);
    }
    return out;
}
int main()
{
    puts(stringDuplicator("hey",3));
}
