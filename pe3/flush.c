#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define MAXIMUM_PATH_LENGTH 4096
#define MAXIMUM_COMMAND_AMOUNT 64
#define MAX_BUFFER_LENGTH 700
#define MAX_BACKGROUND_TASKS 32

char cwd[MAXIMUM_PATH_LENGTH];
char r_d[MAXIMUM_PATH_LENGTH];

bool backgroundTask = false;
int ampersandIndex;

struct ChildProcess
{
    pid_t pid;
    char main[20];
    bool isActive;
    char extraCommands[MAX_BUFFER_LENGTH];
};

struct ChildProcess background_processes[MAX_BACKGROUND_TASKS];

// variables
const char SPACE = 0x20;
const char TAB = 0x09;
// This is not actually CTRL-D, but ctrl d shuts down in the wrong way. A simple d will turn off the application.
const char CTRLD = 0x04;

// Lagret disse globalt sånn at man lettere kunne nå de gjennom forskjellige funksjone
char *splittedCommands[MAX_BUFFER_LENGTH];
char *mainCommand[20];

void print_current_working_directory()
{
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Error while getting current working directory");
    }
    else
    {
        printf("\n");
        printf("%s: ", cwd);
    }
}

void change_dir()
{

    // Check what the main command is, https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/

    // Sjekker at den det første som ligger i splittedCommands eksisterer
    if (splittedCommands[1] != NULL)
    {
        chdir(splittedCommands[1]);
    }
    else
    {
        printf("The path does not exist");
    }
}

void jobs()
{

    for (int i = 0; i < MAX_BACKGROUND_TASKS; i++)
    {
        struct ChildProcess cp = background_processes[i];

        // If I is at the final possible process and it is not active.
        if (i == 0 && !cp.isActive)
        {
            printf("\nThere are no active background tasks");
            return;
        }
        // If process is inactive, continue to next.
        if (!cp.isActive)
        {
            
            continue;
        }
            
            printf("\n %d: %s", cp.pid, cp.main);
        
    }
}

void execute_cd_or_jobs()
{

    if (strcmp(mainCommand, "cd") == 0)
    {
        change_dir();
    }

    else if (strcmp(mainCommand, "jobs") == 0)
    {
        jobs();
    }
}

void execute_wait_operation()
{

    // Source: https://www.youtube.com/watch?v=DiNmwwQWl0g
    // Source: https://www.delftstack.com/howto/c/execvp-in-c/#:~:text=Use%20execvp%20Function%20to%20Replace%20a%20Process%20Image%20in%20C,-In%20Unix%2Dbased&text=These%20functions%20take%20a%20file,arguments%20as%20the%20second%20argument

    pid_t child_pid = fork();

    int wait_status;

    // Lagrer informasjon om prosessen.
    struct ChildProcess currentProcess;
    currentProcess.pid = child_pid;
    strcpy(currentProcess.main,mainCommand);
    currentProcess.isActive = true;
    // måtte lage loop. Klarte ikke sette hele arrayet på en linje.
    for (int i = 0; i < sizeof(currentProcess.extraCommands); i++)
    {
        currentProcess.extraCommands[i] = splittedCommands[i];
    }

    // plasserer structen i arrayet med bakgrunnsprosesser.
    for (int i = 0; i < MAX_BACKGROUND_TASKS; i++)
    {
        if (background_processes[i].isActive == false)
        {
            background_processes[i] = currentProcess;
            break;
        }

        if (i == MAX_BACKGROUND_TASKS - 1 && background_processes[MAX_BACKGROUND_TASKS - 1].isActive)
        {
            printf("Background task array full");
        }
    }

    if (child_pid == -1)
    {
        printf("Wrong!!!");
        exit(EXIT_FAILURE);
    }

    // In the childprocess
    else if (child_pid == 0)
    {

        int exec;

        exec = execvp(mainCommand, splittedCommands);
        if (exec == -1)
        {
            printf("error with execvp");
        }
        exit(0);
    }
    else if (child_pid > 0)
    {
        // Parent process from YT video
        // wait(wait_status);

        if (waitpid(child_pid, &wait_status, WUNTRACED) == -1)
        {
            printf("Perror: waitpid.\n");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(wait_status))
        {
            int statusCode = WEXITSTATUS(wait_status);
            printf("\nExit status [%s %s] = %d", mainCommand, splittedCommands[1], statusCode);
            //Catch the zombies. After process is finished, remove its boolean value and it wont appear for user. 
            for (int i = 0; i < 32; i++)
            {
                if(background_processes[i].pid == child_pid){
                    background_processes[i].isActive = false;
                }
            }
            
        }
    }
}

int main()
{

    for (int i = 0; i < MAX_BACKGROUND_TASKS; i++)
    {
        background_processes[i].isActive = false;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Error while getting current working directory");
    }

    memcpy(r_d, cwd, sizeof(cwd));
    while (1)
    {
        print_current_working_directory();

        // Bruker fgets istedenfor scanf fordi scanf ikke er "buffersafe" (Se under)
        // https://stackoverflow.com/questions/22065675/get-text-from-user-input-using-c
        char command[MAX_BUFFER_LENGTH];
        fgets(&command, MAX_BUFFER_LENGTH, stdin);
        fflush(stdin);

        // Fjerner mellomrommet, ellers virker ikke cd// Dette er kok så vi må gjøre dette på en annen måte
        command[strlen(command) - 1] = '\0';

        // Er noen warnings her som jeg ikke vet hvor kommer fra
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

            // Lagrer hovedkommandoen i en egen variabel, samler resten i splittedCommands
            strcpy(mainCommand,token);
            // Måtte legge til den første og ellers funker ikke execvp
            splittedCommands[toknum] = mainCommand;

            while (token != NULL)
            {

                if (toknum > MAXIMUM_COMMAND_AMOUNT - 1)
                {
                    perror("Command too long");
                }
                token = strtok(NULL, delimiters);

                splittedCommands[toknum + 1] = token;
                toknum++;
            }
            // Må sjekke om det skal være en background process.
            if (splittedCommands[strlen(splittedCommands) - 1] == '&')
            {

                backgroundTask = true;

                for (int i = 0; i < MAX_BUFFER_LENGTH; i++)
                {
                    char *ret = strstr(splittedCommands[i], "&");
                    if (ret)
                    {
                        splittedCommands[i] = "";
                    }
                }

                printf("BG");
            }

            // Hvis det ikke er noen kommandoer i det hele tatt
            // Betyr at man i teorien kan skrive enter så litt feil
            if (mainCommand == NULL || strstr(mainCommand, "0"))
            {
                printf("exiting flush\n");
                return 0;
            }
            else
            {

                if (!strcmp(mainCommand, "cd") || !strcmp(mainCommand, "jobs"))
                {
                    execute_cd_or_jobs();
                }

                else
                {

                    execute_wait_operation();
                }
            }

            // reset
            memset(splittedCommands, 0, MAX_BUFFER_LENGTH * sizeof(splittedCommands[0]));
            //mainCommand = "\0";
        }
    }

    return 1;
}