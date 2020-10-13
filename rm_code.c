#include <dirent.h>
#include <stdio.h>
#include <err.h>
#include <sysexits.h>
#include <stdlib.h>
#include<sys/stat.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char** argv) 
{
    int c, c1;
    
    char** temp = argv;
    temp+=1;

    //printf("%s %d", *(temp), argc);
    //printf("%s %s %s\n", argv[2], argv[3], argv[4]);

    //rm -i
    if(strcmp(argv[1], "-f")==0)
    {
        //printf("HERE");
        temp++;
        while(*(temp)!=NULL)
        {
            char ch;
       
            c= access(*temp, F_OK);
            if(c<0)
            {
                temp++;
                continue;
            }
            c1 = unlink(*temp);
            if(c1<0)
            {
                temp++;
                continue;
            }
            temp++;
        }
        exit(0);
    }

    //rm -d
    else if(strcmp(argv[1], "-d")==0)
    {
        temp++;
        while(*(temp)!=NULL)
        {
       
            c= access(*temp, R_OK|W_OK);
           
            if(c<0)
            {
                perror(*temp);
                temp++;
                continue;
            }
            c1 = remove(*temp);
            if(c1<0)
            {
                perror(*temp);
                temp++;
                continue;
            }
            temp++;
        }
        exit(0);
    }
    else if( strcmp(argv[1], "none")==0)//default
    {
       temp++;
        while(*(temp)!=NULL)
        {
       
            c= access(*temp, W_OK);
            if(c<0)
            {
                perror(*temp);
                temp++;
                continue;
            }
            c1 = unlink(*temp);
            if(c1<0)
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
    return 0;
}
