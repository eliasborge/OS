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

//Lagret disse globalt sånn at man lettere kunne nå de gjennom forskjellige funksjone
char *splittedCommands[MAX_BUFFER_LENGTH];
char *mainCommand;

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

void take_arguments()
{
    // Implement method for reading the input string. Splitting on SPACE or TAB and then call method for
    //  creating the fork process and exectuing the args.

    //Check what the main command is, https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/
    if(strcmp(mainCommand, "cd") == 0){

        //Sjekker at den det første som ligger i splittedCommands eksisterer
        if(splittedCommands[0] != NULL) {
  
            if(chdir(splittedCommands[0]) == -1) {
                printf("No such file or directory");
            }
        } else {
            printf("The path does not exist");
        }
    }


}

void execute_wait_operation() {

    //https://www.delftstack.com/howto/c/execvp-in-c/#:~:text=Use%20execvp%20Function%20to%20Replace%20a%20Process%20Image%20in%20C,-In%20Unix%2Dbased&text=These%20functions%20take%20a%20file,arguments%20as%20the%20second%20argument
    // Må få til exit status 

    
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

        //Bruker fgets istedenfor scanf fordi scanf ikke er "buffersafe" (Se under)
        //https://stackoverflow.com/questions/22065675/get-text-from-user-input-using-c
        char command[MAX_BUFFER_LENGTH];
        fgets(&command, MAX_BUFFER_LENGTH, stdin);
        fflush(stdin);

        //Fjerner mellomrommet, ellers virker ikke cd// Dette er kok så vi må gjøre dette på en annen måte
        command[strlen(command) - 1] = '\0';
        
        //Er noen warnings her som jeg ikke vet hvor kommer fra
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

            //Sender deretter denne til take_arguments som utfører alle kommandoene
            take_arguments();



        }
    }

    return 1;

}