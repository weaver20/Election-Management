#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int main(int argc, char** argv)
{

typedef  (bool (*CmpFunction)(int, int)) CmpFunction ;


    for(int i=0;i<argc;i++)
    {
        printf("\n%d. %s\n",i,argv[i]);
    }   
printf("end\n");
return 0;
}
