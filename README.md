----Description:----

This code is an attempt at designing a simple shell that can handle five internal and five external commands viz. ‘cd’, ‘pwd’, ‘echo’ , ‘history’ and ‘exit’ ‘ls’, ‘cat’, ‘date’ , ‘rm’ and ‘mkdir’ 
Two command line options for each command have been implemented (except exit). 
There are certain assumptions that have been taken while implementing some commands and they will be listed down in their respective command’s sections. 

Some important information/pointers and assumptions-

The home directory for the shell is the actual home directory of your computer. 

‘~’ expands to your home folder. For example, /home/metset in my case.

Commands like rm and mkdir can take multiple file names as arguments however, the maximum number of files that the requisite command can take at a time is specified in the corresponding command section of this writeup.

Double quotes are treated as is in the Linux bash: everything inside double quotes is considered a single entity. For example “folder name” is a single token. 

The size of the paths to a file cannot exceed more than 1024 characters. It would lead to a fault in the implementation. I have assumed that the size of the path would always be less than 1024 characters.

. (dot) represents the current directory and ..(dot dot) represents the parent directory.

Operators in combination with commands have not been implemented. For example, rm , cd -, cat >filename etc would just result in the treatment of “” and “-“, “>filename” as directory/file name only.

There is a separate history file that records history across all sessions. At the start of every session, the history list reads from the history file and writes to it at the end of every session.

User can only choose one option at a time. All invalid options will be considered as file/directory name or will be reported for error depending on the command.
Error handling is mostly done via perror().




----------------------------------------------------------------Commands:---------------------------------------------------------------------

echo 

Format: echo [-option] [string]... 

Options: 
• -n : do not output the trailing newline. 
• -E : disable interpretation of backslash/escape characters. 
If no option is provided, -E option is the default assumption. 

Assumptions: none 

Error handling and corner cases: Unless a write error occurs, echo will be success.

*******

history 

Format: history [-c] or history -w [filename] 

Options: 
• -c : clear the history list- delete all its entries. 
• -w : write the current history to the file. If no filename is provided, history is written to the history_file. If no option is provided, “history” command would display the history list for that session. 

Assumptions: The history -w [file] command will always append the current session’s history to the history_file or the file provided. In case the file does not exist, the command will create a file and copy the history in it. The history will encompass the implemented shell’s history, not the history of the bash. 

Error handling and corner cases: • If the history -w option is executed and a file name is provided, if the file doesn’t not allow write access, “Permission denied” error message would be displayed. • In case the path specifying the filename in history -w has any one directory which does not allow edit or search permissions, “Permission denied” error message would be displayed. • If there is any error opening or writing to or reading from the history file, an error would be displayed and the program would terminate. • In case of any syntactical ambiguity on user’s side that doesn’t adhere to the specified format of the command, an “Invalid syntax” error will be shown.

*******

date 

Format: date [option] 

Options: • -u : print the Coordinated Universal Time (UTC). • -R : output the date/time in RFC 5322 format. If no option is provided, “date” command would display current date and time in the default format. 

Assumptions: The user does not enter any date format. In case any other syntax than the one specified here is followed, a syntax error message is displayed. 

Error handling and corner cases: • In case localtime() and gmtime() functions fail, the requisite error message will be shown. • In case any flag other than the ones specified are used, “Invalid syntax” message is displayed. • In case time points outside the available memory space, the EFAULT error is handled. • In case of any syntactical ambiguity on user’s side that doesn’t adhere to the specified format of the command, an “Invalid syntax” error will be shown.

*******

cat 

Format: cat [option] [file]... 

Options: • -n : number all output lines. • -E : display $ at the end of each line. If no option is provided, cat simply concatenates the file(s) to the stdout. 

Assumptions: The user should not input any more than 10 files at a time in case of default or no option specification and 9 files at a time in case of -n and -E specification, else an “Invalid syntax” error will be thrown. If any option except -n or -E is provided, it will be treated as a file name. 

Error handling and corner cases:
• When the file is in a path in which even a single directory does not have search permission, cat would not be able to open the file and a permission denied message would be displayed. • When the file does not have read permission, cat would not be able to open the file and a permission denied message would be displayed. • When the file does not exist in the specified directory, a file does not exist message will be displayed. • In case there are more than 10 files provided at a time to cat or in case of any syntactical ambiguity that doesn’t adhere to the specified format, an Invalid syntax error will be shown.

*******

ls 

Format: ls [option] [file] 

Options: • -a : do not ignore entries starting with . (dot) • -A : do not list implied . And .. If no option is provided, all entries starting with . (dot) are ignored. This is the default mode. 

Assumptions: The user only enters at most single filename after ls [option] or else a syntax error message will be shown. If the user enters any option except -a and -A, it will be treated as a directory name. 

Error handling/corner cases: • If the read permission is denied for the directory and/ or search permission is denied for any directory in the path, an error message “Permission denied” will be displayed. • If there is no existing directory as per the path/directory name input, then the error message “No such file or directory” will be displayed. • If there is a even a single component in the pathname (if it is the input) that is not a directory, the error message “No such file or directory” will be displayed. • In case of any syntactical ambiguity on user’s side that doesn’t adhere to the specified format, an “Invalid syntax” error will be shown.

*******

cd 

Format: cd [-P] [dir] or cd [-Pe] [dir] 

Options:
• -P : use the physical directory structure with out following symbolic links • -e : used with -P. If the current working directory cannot be determined successfully, exit with a non-zero status. (-Pe) • —help : display help. If no option is provided, -P option is the default assumption. 

Assumptions: The user would enter the mode -e as -Pe only and not as -P -e else there would be a syntactical error message shown. The default cd option is -P. The ‘current working directory’ refers to the current directory after resolution of symbolic links. 

Error handling/corner cases: • If permission to read or search even a single directory out of the of the provided path file is denied, an error message “Permission denied” will be displayed. • If the directory specified in the path/ or named doesn’t exist, “A no such file or directory” message is displayed. • If the user tries to enter a file, “not s directory” error message is displayed. • If the path is a bad address, “bad address” error message will be displayed. • In case of any syntactical ambiguity on user’s side that doesn’t adhere to the specified format, an “Invalid syntax” error will be shown.

*******

pwd 

Format: pwd [option] 

Options: • -P : avoid symlinks. • —help : display help. If no option is provided, -P option is the default assumption. 

Assumptions: The default pwd option is -P. 

Error handling/corner cases: • In case there is a non allowance of read or search permission for any component of the pathname, an appropriate error message is displayed. • If the control is in a directory already and its permissions are changed afterwards, pwd would still be able to display the path. • Any error occurring in the getcwd() function is being handled. The adequate error message will be displayed as well.
• In case of any syntactical ambiguity on user’s side that doesn’t adhere to the specified format, an “Invalid syntax” error will be shown.

******

mkdir 

Format: mkdir [-v] [-m mode] directory... 

Options: • -m : set file mode, not a=rwx, accepts octal modes only. • -v : print a message for each created directory. If no option is provided, the permission 0777 is set for each created directory, no message is printed after successful creation. 

Assumptions: The user should not input any more than 10 files at a time in case of default, not more than 9 files for -v options and not more than 8 files at a time for the -m option, else an “Invalid syntax” error will be thrown. If any option except -n or -E is provided, it will be treated as a file name. Permission modes would only be input as an octal number. a=rwx or other similar format would not be considered. 

Error handling and corner cases: • If the parent directory does not allow write permission, or even a single one of the directories in the path does not allow permission, then an error message “permission denied” will be displayed. • If the directory already exists in the same location, the error message “File exists” will be displayed. • If even a single directory in the path name is not a directory, an error message “no such file or directory” will be displayed. • In case there are more files than specified for a particular mode, or in case of any syntactical ambiguity on user’s side that doesn’t adhere to the specified format, an “Invalid syntax” error will be shown.

*******

rm 

Format: rm [option] [file]... 

Options: • -f : ignore nonexistent files and arguments, never prompt. • -d : remove empty directories. By default, rm does not remove directories.
If no option is provided, the default mode would not remove directories and will prompt the user in case of errors.

Assumptions: In case there is a file that does not allow write permissions, rm would not be able to delete it, with any or no option whatsoever. f any option except -f or -d is provided, it will be treated as a file name If any option other than -f or -d is used, it would be considered as the normal file/ directory name/path. The user should not input any more than 10 files at a time, in case of default, and 9 in case of -f and -d modes else an “Invalid syntax” error will be thrown. If a file is provided in the rm -d option, it should have both read and write permissions to be removed, just like a directory. 

Error handling and corner cases: • When removing a directory, if even a single one of the directories in the path does not allow search permission, or the directory doesn’t allow read and write permissions, then an error message “permission denied” will be displayed. • The -f mode is able to delete any file irrespective of the permissions without any prompt. • If the directory is not empty and there is an attempt to remove it, a “Directory not empty” error will be displayed. • When removing a file in the default mode, if the file doesn’t allow write permissions or its path does not allow search permissions, then an error message “permission denied” will be displayed. • In case there are more files than specified for a particular mode, or in case of any syntactical ambiguity on user’s side that doesn’t adhere to the specified format, an “Invalid syntax” error will be shown.

*******

exit 

Format: exit 

Assumptions; NA 

Error handling and corner cases: NA





----------------------------------------------------------Test cases:----------------------------------------------------------------------

The following are some sample input test cases:

1. cd ~/Desktop

2. pwd

3. ls-a..

4. mkdir -m 0222 ~/Desktop/d1 ~/Desktop/“d 2”

5. rm -d ~/Desktop/“d 2”

6. date -u

7. history -w hist_file

8. echo -n “The sun is shining”


Corresponding outputs/actions:

1. <navigates to desktop>

2. In my case, /home/metset

3. <lists all files in the home directory, even the ones that start with . (dot)>

4. <will make two directories d1 and d 2 in the specified path>

5. <will remove d 2>

6. In my case, Wed Sep 30 18:12:17 UTC 2020

7. <will write the current session’s history to hist_file>

8. The sun is shining
