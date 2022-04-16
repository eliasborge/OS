#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAXIMUM_PATH_LENGTH 4096
#define MAXIMUM_COMMAND_AMOUNT 64
#define MAX_BUFFER_LENGTH 700

char cwd[MAXIMUM_PATH_LENGTH];
char r_d[MAXIMUM_PATH_LENGTH];

// variables // vet ikke om vi trenger disse
const char SPACE = 0x20;
const char TAB = 0x09;
// This is not actually CTRL-D, but ctrl d shuts down in the wrong way. A simple d will turn off the application.
const char CTRLD = 0x64;

char *splittedCommands[MAX_BUFFER_LENGTH];
char *mainCommand;

void take_arguments()
{
    // Implement method for reading the input string. Splitting on SPACE or TAB and then call method for
    //  creating the fork process and exectuing the args.
}

void print_current_working_directory()
{

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Error while getting current working directory");
    }
    else
    {
        printf("\n");
        printf("%s:", cwd);
    }
}

int main()
{

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Error while getting current working directory");
    }

    memcpy(r_d, cwd, sizeof(cwd));
    while (1)
    {
        print_current_working_directory();
        char command;
        scanf(" %s", &command);

        int ascii = command;
        if (ascii == CTRLD)
        {
            printf("Shutting Down\n");
            break;
        }

        else
        {
            // Split string on SPACE and TAB and send to different method.
            // src: https://riptutorial.com/c/example/2557/tokenisation--strtok----strtok-r---and-strtok-s--
            // Had to concat Tab onto space because i didnt know any other way.
            char delimiters[3] = {' ', '\t', '\0'};
            int toknum = 0;
            char *token = strtok(&command, delimiters);

            //Lagrer hovedkommandoen i en egen variabel, samler resten i splittedCommands
            mainCommand = token;

            while(token != NULL) {

                if(toknum > MAXIMUM_COMMAND_AMOUNT -1){
                    perror("Command too long");
                }

                token = strtok(NULL, delimiters);
                splittedCommands[toknum] = token;
                toknum++;
            }
            printf("Works");

            //Sender deretter denne til take_arguments som utfører kommandoene
            




        }
    }

    return 1;

}