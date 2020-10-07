//can implement cat -E, cat -n and default
//open files, read them line by line, and unke aage numbers daldo, ya peeche $ daldo

#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv)
{
    FILE *fptr;
    char** temp = argv;
    temp++; temp++;
    char line[2048];
    int line_count=1;
    //check the read access of files yes. read permissions honi chahiye
    while(*(temp)!=NULL)
    {

        fptr = fopen(*temp, "r");
        if(fptr==NULL)
        {
            perror(*temp);
            temp++;
            continue;
        }
        while(fgets(line, 2048, fptr)!=NULL)
        {
            if(strcmp(argv[1], "none")==0)
                printf("%s", line);
            else if(strcmp(argv[1], "-n")==0)
            {

                printf("%d  %s", line_count, line);
                if(line[strlen(line)-1]=='\n')
                    line_count++;
            }
                
            else if(strcmp(argv[1], "-E")==0)
            {
                if(line[strlen(line)-1]=='\n')
                {
                    line[strlen(line)-1]='$';
                    printf("%s\n", line);
                }
                else
                    printf("%s", line);

            }    
           
        }
        fclose(fptr);
        temp++  ; 
    }
   // printf("\n");
    exit(0);
}