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

char cwd[MAXIMUM_PATH_LENGTH];
char r_d[MAXIMUM_PATH_LENGTH];

// variables
const char SPACE = 0x20;
const char TAB = 0x09;
// This is not actually CTRL-D, but ctrl d shuts down in the wrong way. A simple d will turn off the application.
const char CTRLD = 0x64;

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
            char delimiters[2] = {SPACE, TAB};
            char splittedCommands[MAXIMUM_COMMAND_AMOUNT];
            char *token;
            int toknum = 0;
            token = strtok(&command, delimiters);
            while (token != NULL)
            {
                //Inserting commands into an array
                if(toknum > MAXIMUM_COMMAND_AMOUNT -1){
                    perror("Command too long");
                }

                splittedCommands[toknum] = *token;

                token = strtok(NULL, delimiters);

                toknum++;
            }

            printf("Works");
            //Unable to print array of tokens but i think it is good to go. 
            //Create function that sends commands to executing function. 
        }
    }

    return 1;

}