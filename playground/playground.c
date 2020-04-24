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
#include <stdbool.h>
#include <string.h>

//TODO: valgrind disturbing errors - not memory leaks.

static char* removeTribeFromArea(char* value, int index) {
    char* spacer = ":";
    char* tmp = malloc(strlen(value)+1);
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
        // debug
#ifndef NDEBUG
        printf("iteration #%d: %s\n",i, tmp);
#endif

    }
    // debug
#ifndef NDEBUG
    printf("end of loop: %s\n", tmp);
#endif
    int new_size = strlen(tmp) + 1;
    char* final = realloc(tmp, new_size);
    if(final == NULL) {
        return NULL;
    }
    return final;
}

int main()
{
    printf("\nTesting removeTribesFromArea \n");
    char* string = malloc(22);
    strcpy(string, "lod:0:100:200:300:400") ;
    char* final_ptr =removeTribeFromArea(string,5);
    puts(final_ptr);
    free(string);
    free(final_ptr);



}
