#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_BUF 200
//https://www.delftstack.com/howto/c/get-current-directory-in-c/#:~:text=Use%20the%20getcwd%20Function%20to%20Get%20Current%20Working%20Directory,-The%20getcwd%20function&text=The%20function%20stores%20the%20name,absolute%20pathname%20of%20the%20directory. 
char * getCurrentDirectory() {

    //buffer where the pathname is stored and the number of bytes allocated in the given buffer. 
    char buffer[MAX_BUF];
    //returns the absolute pathname 
    char *path = getcwd(buffer, MAX_BUF);

    return path;
}

int 

int main() {

    //print out the current user directory and starts a new line
    printf('%s: \n', getCurrentDirectory())

    /*
    Gets user input
    Source: https://stackoverflow.com/questions/22065675/get-text-from-user-input-using-c 
    Using fgets bc scanf isn't buffer safe
    */
   char user_input[MAX_BUF];
   //Should the limit be higher? 
   fgets(input, 200, stdin);

}