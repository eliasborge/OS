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


//Connection functions

void conn_clear(conn_t *){};


void conn_close(conn_t *){};

int conn_open(conn_t *, long, short){};

int conn_accept(server *, conn_t *){};

void conn_clear_all(conn_t *, const int){};

void conn_close_all(conn_t *, const int){};

int conn_add(conn_t *, const int, conn_t *){};

int conn_close_del(conn_t *, const int, conn_t *){};

int conn_wait_to_read(conn_t *, struct timeval *){};

int conn_write(conn_t *, const void *, int){};

int conn_read(conn_t *, void *, int){};

int main(int argc, char *argv[]){
    

    return 0;
};