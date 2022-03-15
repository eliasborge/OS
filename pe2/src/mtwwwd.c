#include <winsock2.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>



typedef struct server {
    int socket;
    struct sockaddr_in address;
}
server;
typedef struct conn_t {
    int socket;
    struct sockaddr_in address;
    struct sockaddr_in local;
}
conn_t;

//Server functions
void server_clear(server * server){
    
};

void server_clear_all (server * server, int num) {

};
void server_close(server *){};

void server_close_all(server *, const int) {};

int server_open(server *, long, short) {};




int main(int argc, char *argv[]){
    

    return 0;
};