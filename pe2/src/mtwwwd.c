#define MAXREQ (4096 * 1024)
#define PORT 6789
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


char buffer[MAXREQ] , body[MAXREQ] , msg[MAXREQ] ;
void error(const char *msg) { perror(msg) ; exit(1); }

int main(void) {

    int sockfd, newsockfd;
    socklen_t clilen;
    struct  sockaddr_in serv_addr, cli_addr;
    
    
}