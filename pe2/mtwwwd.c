#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include "bbuffer.h"
#include <pthread.h>


#define MAXREQ (4096*1024)

char buff[MAXREQ], body[MAXREQ], message[MAXREQ];
char *wwwp;
int port;
int threads;
int buffers;


BNDBUF *bb;

void error(const char* msg){
    perror(msg);
    exit(1);
}

void dest(char *buff, char *addr_buff){
    int i = 0;
    int condition = 0;
    int j = 0;
    while(1){
        if (buff[j] == '\n' || buff[j] == '\r'){
            addr_buff[i+1] = '\0';
            break;
        }

        if(condition == 0 && buff[j] == ' '){
            j++;
            condition = 1;
        }
        if(condition == 1){
            if (buff[j]== ' '){
                addr_buff[i+1] = '\0';
                break;
            }
            addr_buff[i] = buff[j];
            i++;
        }
        j++;
    }

    return;
}

void arguments(int argc, char*argv[]) {
    if(argc < 5) {
        
        wwwp = getenv("PWD");
        strcat(wwwp, "/pages");
        port = 6789;
        if(argc == 2) {
            port = atoi(argv[1]);
        }
        threads = 50;
        buffers = 50;
    } else {
        wwwp = argv[1];
        port = atoi(argv[2]);
        threads = atoi(argv[3]);
        buffers = atoi(argv[4]);
    }
}

//For å håndtere requests som kommer
void *request() {
    char addr_buff[256];
    char tot_addr[256];
    int errorNum = 0;
    int n;
    int newsockfd;

    FILE *file;
    while(1) {
        newsockfd = bb_get(bb);
        if(newsockfd < 0){
            error("ERROR on accept");
        }
        bzero(buff, sizeof(buff));
        n = read(newsockfd, buff, sizeof(buff)-1);
        
        if(n < 0){
            error("ERROR reading from socket");
        }

        snprintf(body, sizeof(body),
        "<html>\n<body>\n<h1>Hello web browser</h1>Your request was\n<pre>%s</pre>\n</body>\n</html>\n", buff);

        bzero(addr_buff, sizeof(addr_buff));
        dest(buff, addr_buff);
        
        strcpy(tot_addr, wwwp);
        strcat(tot_addr, addr_buff);
    
        char * f_buff = 0;
        long l;
        
        
        FILE * filepath = fopen (tot_addr, "r+");
        if (filepath == NULL ){
            strcpy(tot_addr, wwwp);
            strcat(tot_addr, "/error.html");
            printf("Response adress: %s \n", tot_addr);
            filepath = fopen (tot_addr, "rb");
            errorNum = 1;
        }
        errorNum = 0;
        fseek (filepath, 0, SEEK_END);
        l = ftell (filepath);
        fseek (filepath, 0, SEEK_SET);
        f_buff = malloc (l);
        if (f_buff)
            {
                fread (f_buff, 1, l, filepath);
            }
        fclose (filepath);

        bzero(tot_addr, sizeof(tot_addr));
        if(errorNum == 0) {
            snprintf(message, sizeof(message),
            "HTTP/1.0 200 OK\n Content-Type: text/html\n Content-Length: %lu\n\n%s", strlen(f_buff), f_buff);
        } else {
            snprintf(message, sizeof(message),
            "HTTP/1.0 404 Not found\n Content-Type: text/html\n Content-Length: %lu\n\n%s", strlen(f_buff), f_buff);
        }

        n = write(newsockfd, message, strlen(message));
        

        if (n<0){
            error("ERROR writing to socket");
        }

        close(newsockfd);
        
    }       
}

int main(int argc, char *argv[]){
    
    arguments(argc, argv);

    pthread_t t[threads];
    int temp_arg[threads];
    bb = bb_init(buffers);

    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockfd<0) error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding");
    }
    //Loop that creates threads
    int i = 0;
    while(i<threads){
        temp_arg[i] = i;
        int result = pthread_create(&t[i], NULL, &request, &temp_arg[i]);
        i++;
    }
    listen(sockfd, 5);
    
    while(1){
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        bb_add(bb, newsockfd);
    }
}
