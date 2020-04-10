#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int main(int argc, char** argv)
{


    for(int i=0;i<argc;i++)
    {
        printf("\n%d. %s\n",i,argv[i]);
    }   
printf("end\n");
return 0;
}