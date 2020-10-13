//fork(), exec(), wait()
//internal- cd, echo, history, pwd, exit
//external- ls, cat, date, rm, mkdir


#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h>
#include<readline/readline.h>
#include <readline/history.h>
#include <sysexits.h>
#include<err.h>
#include <sys/stat.h>
#include <wordexp.h>
#define MAXARGS 1024

char PATH[1024];
char* HOME;
int hist_len;
char* prev;
char history_path[1050];


void setpath()
{
getcwd(PATH, sizeof(PATH));
strcat(PATH, "/");
HOME = getenv("HOME");
using_history();
strcpy(history_path, PATH);
strcat(history_path,"/history_file");
int k = read_history(history_path);
if(k!=0)
{
    perror("history_file");
    exit(1);

}
hist_len = history_length;
prev=NULL;
}

//helpers

int check_octal(char* str)
{
    char *temp = str;
    while(*(temp)!=NULL)
    {
        if((int)*temp<48 || (int)*temp>55)
        {
            return -1;
        }
        temp++;
    }
    return 0; //is octal
}

int get_no_of_words(char** str)
{
    char **temp = str;
    int count=0;
    while(*(++temp)!=NULL)
   {
       count++;
   }
   return count;
}

void print_cd_help()
{
    printf("cd: cd [-P] [dir]\n   Change the shell working directory.\n\n   Change the current directory to DIR. The default DIR is the value of the\nHOME shell variable.\n\n   If the directory is not found, then an error message \"Directory not found\" would be displayed. In case of synactical error(s), the error message \"Invalid Syntax\" is displayed.\n\n");
    printf("   Options:\n     -P	   use the physical directory structure without following symbolic links: resolve symbolic links in DIR before processing instances of \"..\".\n");
    printf("      -e	   if the -P option is supplied, and the current working directory cannot be determined successfully, exit with a non-zero status\n\n");
    printf("   The default is to not resolve symbolic links, as if \"-P\" were specified. \"..\" is processed by removing the immediately previous pathname component back to a slash or the beginning of DIR. cd * as is in the bash, is not supported.\n\n");
    printf("   Exit Status:\nExits with exit status 0 if the directory is changed. Exits with status 1 otherwise.\n");
}

void help_for_pwd()
{
    printf("pwd: pwd [-LP]\n   Print the name of the current working directory.\n\n ");  
    printf("    Options:\n    -P	print the physical directory, without any symbolic links\n");
    printf("     --help   to display this page.\n\nBy default, `pwd' behaves as if `-P' were specified.\n\n");
    printf("     Exit Status:\nReturns 0 unless an invalid option is given or the current directory cannot be read.\n");
}

void write_to_history(char *filename)
{
    register HIST_ENTRY **histlist;
    register int i;
    histlist = history_list();
    FILE *fptr;
    fptr= fopen(filename, "a");
    if(fptr==NULL)
        {
            perror(filename);
            return;
        }
           for(i=hist_len; histlist[i]; i++)
            {
                 int num = fprintf(fptr, "%s\n", histlist[i]->line);
                 if(num<0)
                 {
                     perror(filename);

                 }
                //printf("%d: %s\n", i+ history_base, histlist[i]->line);
            } 
            fclose(fptr);
            return;
}

void printHistory(char **parsed_string)
{
    register HIST_ENTRY **histlist;
    register int i;
    histlist = history_list();
    //history_list();
    //if(histlist)
    int n =get_no_of_words(parsed_string);
    if(n==0)
    {

    for(i=0; histlist[i]; i++)
    {
        printf("%d: %s\n", i+ history_base, histlist[i]->line);

    }
    return;
    }

    else if(n==1)
    {
        if(strcmp(parsed_string[1],"-w")==0)
        {
            write_to_history(history_path);
            return;
        }
        if(strcmp(parsed_string[1],"-c")==0)
        {
            register HIST_ENTRY *list;
            for(i=0; histlist[i];)
            {
                list = remove_history(i);
                free(list);
                list = NULL;
            }
            hist_len=0;
            return;

        }

    }
    else if(n==2)
    {
        if(strcmp(parsed_string[1],"-w")==0)
        {
            write_to_history(parsed_string[2]);
            return;

        }

    }
    
    printf("Invalid syntax\n");
    return;
}


void printpwd(char pwd[])    //by default mera is -P
{
    char dir[1024];
    char* ptr;
    ptr = getcwd(dir, sizeof(dir));
    if(ptr==NULL)
                {
                    perror("pwd");
                    exit(1);
                }
    strcpy(pwd, dir);
   
}

void pwd_command(char **parsed_string) //default is -P mode
{
    int n = get_no_of_words(parsed_string);
    if(n==0)
    {
        char pwd[1024];
        printpwd(pwd);
        printf("Directory: %s\n", pwd);
        return;
    }
    else if(n==1)
    {
        if(strcmp(parsed_string[1],"-P")==0)
        {
            char pwd[1024];
        printpwd(pwd);
        printf("Directory: %s\n", pwd);
        return;
        }
        else if(strcmp(parsed_string[1],"--help")==0)
        {
            help_for_pwd();
            return;
        }
        else
    {
        printf("Invalid syntax\n");
        return;
    }

    }
    else
    {
        printf("Invalid syntax\n");
        return;
    }
    
}


void naviagte_to_directory(char **parsed_string)
{
    //now we handle the modes
    int tokens ;
    char *nav=NULL;
    tokens = get_no_of_words(parsed_string);
    int ret=-1;
    switch(tokens)
    {
        case 0:
            nav = getenv("HOME");
            //prev = nav;
            ret = chdir(nav);
            break;
        case 1: 
            if (strcmp(parsed_string[1],"--help")==0)
            {
                print_cd_help();
                return;
            }
            if(strcmp(parsed_string[1], "-P")==0)
            {
                nav = getenv("HOME");
                //prev = nav;
                ret = chdir(nav);
               
            }
           else if(strcmp(parsed_string[1], "-Pe")==0)
            {
                char pres[1024];
                char *ptr;
                ptr= getcwd(pres, sizeof(pres));
                if(ptr==NULL)
                {
                    perror("cd");
                    exit(1);
                }
                nav = getenv("HOME");
                //prev = nav;
                ret = chdir(nav);

            }
            else
            {
               nav = parsed_string[1];
                //prev = nav;
                ret = chdir(nav);
            }

            break;
        case 2:
            if(strcmp(parsed_string[1], "-P")==0)
            {
                nav = parsed_string[2];
                prev = nav;
                ret = chdir(nav);
            }
            else if(strcmp(parsed_string[1], "-Pe")==0)
            {
                char pres[1024];
                char *ptr;
                ptr= getcwd(pres, sizeof(pres));
                if(ptr==NULL)
                {
                    perror("cd");
                    exit(1);
                }
                nav = parsed_string[2];
                //prev = nav;
                ret = chdir(nav);
            } 
            else 
            {
                printf("Invalid syntax\n");
                ret = -5;
                break;
            }
            break;
        default:
            printf("Invalid syntax\n");
            return;
    }
    if (ret==-1)
    {
        perror("cd");
    }
}
void print_echo(char **parsed_string)
{
    int extra;
    int n = get_no_of_words(parsed_string);
    if(n==0)
    {
        extra= printf("\n");
        if(extra<0)
        {
            perror("print error");
        }
        return;
    }
    char **str = parsed_string;
    if(strcmp(parsed_string[1],"-n")==0 || strcmp(parsed_string[1],"-E")==0)
        str++;
    while(*(++str)!=NULL)
   {    
       extra= printf("%s ", *str);
       if(extra<0)
        {
            perror("print error");
        }
   }
   if(strcmp(parsed_string[1],"-n")==0)
   {
       return;
   }
   printf("\n");
}

//external commands...

//void create_child_process(char *path, char)

void spawnchild(char** args, char* process_name)
{
    pid_t pid = fork();
    int status;
    if(pid<0)
    {
      
        err(EX_OSERR, "Failed to fork child process");
        exit(1); 
    }
    else if(pid==0)
    {
        execvp(args[0], args);
        perror(process_name);
        exit(1);
    }
    int w = waitpid(pid, &status, 0);
    if(w==-1)
    {
        perror("waitpid");
        return;
    }
    else if(WIFEXITED(status))
    {
        //parent process
        return;
    }
    else
    {
        return;
    }
    
}

void ls_command(char **parsed_string) //for executing multiple ls s nahi kia abhi :/
{
    int n = get_no_of_words(parsed_string);
    char mode [10] ;
    char dir[1024], pth[1024];
    printpwd(dir);
    //
    if(n==0)
    {
        strcpy(mode, "none");
    }
    else if(n==1) 
    {
        if(strcasecmp(parsed_string[1], "-A")==0)
            strcpy(mode, parsed_string[1]);
        else
        {
            strcpy(mode, "none");
            strcpy(dir, parsed_string[1]); //anything accept -a and -A will br considered a directiry path
        }     
    }
    else if(n==2)
    {
        if(strcasecmp(parsed_string[1], "-A")==0)
            strcpy(mode, parsed_string[1]);
        else
        {
            printf("Invalid syntax\n");
            return;
        }
        strcpy(dir, parsed_string[2]);
    }
    else
    {
        printf("Invalid syntax\n");
        return;
    }
    strcpy(pth, PATH);
    strcat(pth, "./ls_code");
    char *args[] = {pth, dir, mode, NULL};
    spawnchild(args, "ls");
}

void rm_command(char **parsed_string)
{
    char pth[1024];
   // printf("HERE??");
    char** temp = parsed_string+1;
    int t = get_no_of_words(parsed_string);
    char mode[] = "none";
    char *args[13]; //can ONLY give 10 files at a time as input 
    int i =2;

    if(t==0 || t>=11)
    {
        printf("Invalid syntax\n");
        return;
    }
    if(strcmp(parsed_string[1], "-f")==0 )
    {
        strcpy(mode, "-f");
        temp++;
    }
    else if(strcmp(parsed_string[1], "-d")==0 )
    {
        strcpy(mode, "-d");
        temp++;
    }
    
    strcpy(pth, PATH);
    strcat(pth, "./rm_code");
    //strcpy(*args[0], pth);
    args[0] = pth;
    args[1] = mode;
    while(*(temp)!=NULL)
    {
        args[i]=*temp;
        // printf("%s\n", *temp);
        temp++;
        i++;
       
    }
    //i++;
    for(;i<13;i++)
    {
        args[i]=NULL;
    }
   // printf("HERE??");
    spawnchild(args, "rm");

}

void date_command(char **parsed_string)
{
    int t = get_no_of_words(parsed_string);
    char mode [10];
    char pth[1024];
    if(t==0) //1 token
    {
        strcpy(mode, "none");
    }
    else if(t==1)
    {
        if(strcmp(parsed_string[1], "-R")==0 || strcmp(parsed_string[1], "-u")==0)
            strcpy(mode, parsed_string[1]);
        else
        {
            printf("Invalid syntax\n");
            return;
        }   
    }
    else
    {
        printf("Invalid syntax\n");
        return;
    }
    strcpy(pth, PATH);
    strcat(pth, "./date_code");
    char *args[] = {pth, mode, NULL};
    spawnchild(args, "date");
}

void mkdir_command(char** parsed_string)
{
    char pth[1024];
   // printf("HERE??");
    char** temp = parsed_string+1;
    int t = get_no_of_words(parsed_string);
    char mode[] = "none";
    char *args[13]; //can ONLY give 13 tokens at a time as input 
    int i =2;

    if(t==0 || t>=11)
    {
        printf("Invalid syntax\n");
        return;
    }
    if(strcmp(parsed_string[1], "-v")==0 )
    {
        if(t<2)
        {
            printf("Invalid syntax\n");
            return;
        }
        strcpy(mode, "-v");
        temp++;
    }
    else if(strcmp(parsed_string[1], "-m")==0)
    {
        if(t<3)
        {
            printf("Invalid syntax\n");
            return;
        }
        strcpy(mode, "-m");
        temp++;
        int n = check_octal(parsed_string[2]);

        if (n != 0) 
        {
            printf("Invalid file mode: %s\n", parsed_string[2]);
            return;
            }
    }
    
    strcpy(pth, PATH);
    strcat(pth, "./mkdir_code");
    //strcpy(*args[0], pth);
    args[0] = pth;
    args[1] = mode;
    while(*(temp)!=NULL)
    {
        args[i]=*temp;
        // printf("%s\n", *temp);
        temp++;
        i++;
       
    }
    //i++;
    for(;i<13;i++)
    {
        args[i]=NULL;
    }
   // printf("HERE??");
    spawnchild(args, "mkdir");
}

void cat_command(char** parsed_string)
{
    char pth[1024];
   // printf("HERE??");
    char** temp = parsed_string+1;
    int t = get_no_of_words(parsed_string);
    char mode[] = "none";
    char *args[13]; //can ONLY give 10 files at a time as input 
    int i =2;

    if(t==0 || t>=11)
    {
        printf("Invalid syntax\n");
        return;
    }
    if(strcmp(parsed_string[1], "-n")==0 )
    {
        strcpy(mode, "-n");
        temp++;
    }
    else if(strcmp(parsed_string[1], "-E")==0 )
    {
        strcpy(mode, "-E");
        temp++;
    }
    
    strcpy(pth, PATH);
    strcat(pth, "./cat_code");
    //strcpy(*args[0], pth);
    args[0] = pth;
    args[1] = mode;
    while(*(temp)!=NULL)
    {
        args[i]=*temp;
        // printf("%s\n", *temp);
        temp++;
        i++;
       
    }
    //i++;
    for(;i<13;i++)
    {
        args[i]=NULL;
    }
   // printf("HERE??");
    spawnchild(args, "cat");
}

int internal_command(char **parsed_string)
{
    if(strcmp(parsed_string[0], "cd")==0)
    {
        naviagte_to_directory(parsed_string);
    }
    else if(strcmp(parsed_string[0], "pwd")==0)
    {
        pwd_command(parsed_string);
        
    }
    else if(strcmp(parsed_string[0], "echo")==0)
    {
        print_echo(parsed_string);
    }
    else if(strcmp(parsed_string[0], "history")==0)
    {
        printHistory(parsed_string);
    }
    else if(strcmp(parsed_string[0], "exit")==0)
    {
        write_to_history(history_path);
        exit(0);
    }
    else
    {
        return 1;
    }
    return 0;
}


int external_command(char **parsed_string)
{
    if(strcmp(parsed_string[0], "ls")==0)
    {
        ls_command(parsed_string);
    }
    else if(strcmp(parsed_string[0], "cat")==0)
    {
        cat_command(parsed_string);
       // printpwd();
    }
    else if(strcmp(parsed_string[0], "date")==0)
    {
        date_command(parsed_string);
    }
    else if(strcmp(parsed_string[0], "rm")==0)
    {
       // printf("STUOD");
        //rm_text(parsed_string);
        rm_command(parsed_string);
    }
    else if(strcmp(parsed_string[0], "mkdir")==0)
    {
        mkdir_command(parsed_string);
    }
    else
    {
        return 1;
    }
    return 0;
}


char** parse_input(char *str, char** parsed_string)
{
    wordexp_t word;
    
    int n = wordexp(str, &word, 0);
    parsed_string = word.we_wordv;
    if(n!=0)
    {
        perror("");
    }
    return parsed_string;
}

int examine_input(char *str, char **final_parse) //str is input string
{
    final_parse = parse_input(str, final_parse);
    int cmd = internal_command(final_parse);
    if(cmd==0)
    {
        return 0;
    }
    else
    {
        int cmd = external_command(final_parse);
        if(cmd==0)
            return 0;
    }
    return 1; 
}

int main()
{
    char *str;
    char *final_str [MAXARGS];
    int execflag;
    setpath();
    printf("%s\n", PATH);
    while(1)
    {
        str = readline(">>> ");
       // parse_input()
        if(strlen(str)!=0)
        {
        add_history(str);
        execflag = examine_input(str, final_str);
        if(execflag == 1)
        {
            printf("Not a command\n");
            continue;
        }
        }
    }
}
