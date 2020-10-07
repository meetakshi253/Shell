#include <dirent.h>
#include <stdio.h>
#include <err.h>
#include <sysexits.h>
#include <stdlib.h>
#include<sys/stat.h>
#include <unistd.h>
#include <string.h>


// -A mode lists hidden files too. except . and .. default is not -A -a lists all. default is not -a:>. 

int main(int argc, char **argv) //number of args passed and the args
{
    struct dirent *destruct;
    DIR *dir = opendir(argv[1]);
    if(dir==NULL)
    {
        perror(argv[1]);
       // err(EX_OSERR, "Could not open the current directory\n");
        exit(1);
    }

    while((destruct = readdir(dir))!=NULL)
    {
       if(destruct->d_name[0]=='.')
       {
            if(strcmp(argv[2], "none")==0) //none
                continue;
            else if(strcmp(argv[2], "-a")==0) //-a
                printf("%s\n", destruct->d_name);
            else if(strcmp(argv[2], "-A")==0 && (strcmp(destruct->d_name,".." )==0 || strcmp(destruct->d_name,".")==0) )//-A
                continue;
       }
       else
            printf("%s\n", destruct->d_name);
    }
    closedir(dir);
    exit(0);
}