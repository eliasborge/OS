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

    int socketfd, newsocketfd;
    socklen_t clilen;
    struct  sockaddr_in server_address, client_address;
    int n;
    socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        error("Error opening socket");
    };
    bzero((char *) &server_address , sizeof(server_address));
    

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if(bind(socketfd, (struct sockaddr *) &server_address,
    sizeof(server_address)) < 0) {
        error("ERROR on binding");
    }
    listen(socketfd ,5);

    while(1) {
        clilen = sizeof(client_address);
        newsocketfd = accept  (socketfd, (struct sockaddr *) &client_address, &clilen);
        if (newsocketfd < 0) error("Error on accept");
        bzero(buffer, sizeof(buffer));
        n = read (newsocketfd,buffer,sizeof(buffer) - 1);
        if (n < 0) error("ERROR reading from socket");
    
    
        snprintf (body, sizeof (body),
        "<html>\n<bodt>\n"
        "<h1>Welcome to this web browser</h1> \n<h2>Have fun</h2>\n"
        "\nYour request was \n"
        "<pre>%s</pre>\n"
        "</body>\n</html>\n",buffer);
        
        snprintf(msg, sizeof(msg), 
        "HTTP/1.0 200 OK\n"
        "Content-Type: text/html\n"
        "Content-Length: %lu\n\n%s", strlen (body), body);
        n = write (newsocketfd,msg,strlen(msg));
        if(n<0) error("Error writing to socket");
        close (newsocketfd);
    
    }
}