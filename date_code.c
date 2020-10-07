//-u to display in UTC format strptime(3).
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <err.h>


int main(int argc, char** argv)
{
    time_t rawtime;
    struct tm *ftime;
    char buffer[1024];
    rawtime = time(NULL);
    if(rawtime==-1)
    {
        printf("time() function failed");
        exit(1);
    }

    if(strcmp(argv[1],"none")==0 || strcmp(argv[1],"-R")==0)  //no mode specified or RFC 5322
    {
        ftime = localtime(&rawtime);
        if(rawtime==-1)
        {   perror("date\n");
            exit(1);
        }
        if(strcmp(argv[1],"none")==0)
            strftime(buffer, sizeof(buffer), "%a %b %d %T %Z %Y", ftime);
        else
            strftime(buffer, sizeof(buffer), "%a, %b %d %Y %T %z", ftime);
        printf("%s\n", buffer);
        exit(0);
    }

    //date -U
    else if(strcmp(argv[1],"-u")==0)
    {
        ftime = gmtime(&rawtime);
        if(rawtime==-1)
        {
            perror("date\n");
            exit(1);
        }
        strftime(buffer, sizeof(buffer), "%a %b %d %T UTC %Y", ftime);
        printf("%s\n", buffer);
        exit(0);
    }
    exit(1);
}