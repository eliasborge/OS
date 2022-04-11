#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXIMUM_PATH_LENGTH 4096;

char cwd[MAXIMUM_PATH_LENGTH];

//variables
const char SPACE = 0x20;
const char TAB = 0x09;
const char CTRLD= 0x04;


void take_arguments(){
    //Implement method for reading the input string. Splitting on SPACE or TAB and then call method for
    // creating the fork process and exectuing the args. 
}


void print_current_working_directory(){
    cwd = getcwd();

    if(cwd == NULL){
        perror("Error while getting current working directory");
    }
    else{
        printf("%s:",cwd);
    }

    take_arguments();
    
}