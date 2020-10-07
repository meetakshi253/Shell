// modes: -m and -v
// mkdir returns 0 if successful, -1 failure

#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
 #include <fcntl.h>
 #include <string.h>

int main(int argc, char** argv)   //path, mode, (access mode if -m), files
{
    //char *dirname ="/Users/metset/desktop/emp";
    char** temp = argv;
    temp+=1;
    int c;
    if(strcmp(argv[1], "none")==0 || strcmp(argv[1], "-v")==0)
    {
        temp++;
        while(*(temp)!=NULL)
        {
            c = mkdir(*temp, 0777);
            if(c!=0)
            {
                perror(*temp);
                temp++;
                continue;
            }
            if(strcmp(argv[1], "-v")==0)
            {
                printf("Created directory: '%s'\n", *temp);
            }
            temp++;
        }
        exit(0);
    }

    else if (strcmp(argv[1], "-m")==0)
    {
        temp++;
        char *permissions = argv[2]; //assume that it is an interger represemtable string 
        long permission = strtol(permissions, NULL, 8);
        temp++;
        while(*(temp)!=NULL)
        {
            c = mkdir(*temp, permission);
            if(c<0)
            {
                perror(*temp);
                temp++;
                continue;
            }
            temp++;
        }
        exit(0);

    }
    exit(1);
}
