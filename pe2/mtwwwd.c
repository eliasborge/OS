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

//Used lecture 11 in OS as a source

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

//finding the needed destination
void dest(char *buff, char *addr_buff){
    int counter1 = 0;
    int counter2 = 0;
    int h = 0;

    //Loop that runs until broken
    while(1){

        //Increments address buffer if conditional is met.
        if (buff[counter2] == '\n' || buff[counter2] == '\r'){
            addr_buff[counter1+1] = '\0';
            printf(addr_buff);
            break;
        }
        //Increments counter
        if(h == 0 && buff[counter2] == ' '){
            counter2++;
            h = 1;
        }
        //Sets buffer at desired index
        if(h == 1){
            if (buff[counter2]== ' '){
                addr_buff[counter1+1] = '\0';
                printf(addr_buff);
                break;
            }
            addr_buff[counter1] = buff[counter2];
            printf(addr_buff);
            counter1++;
        }
        counter2++;
    }

    return;
}



void arguments(int argc, char*argv[]) {
    if(argc < 5) {
        
        //Finds current directory
        wwwp = getenv("PWD");
        //sets path to folder with html files
        strcat(wwwp, "/pages");

        //port is a bit random, could be anything
        port = 6789;
        if(argc == 2) {
            port = atoi(argv[1]);
        }
        threads = 50;
        buffers = 50;
        int number_of_buffers = [MAXREQ];
    } else {

        //translates values
        wwwp = argv[1];
        port = atoi(argv[2]);
        threads = atoi(argv[3]);
        buffers = atoi(argv[4]);
        int number_of_buffers =buffers;
    }
}

//For å håndtere requests som kommer inn
void *request() {
    char addr_buff[256];
    char tot_addr[256];
    int errorNum = 0;
    int n;
    int newsockfd;


    //create file
    FILE *file;
    while(1) {

        //Get socket descriptor information
        newsockfd = bb_get(bb);
        int socket;
        if(newsockfd < 0){
            error("ERROR on accept");
        }

        //Erase memory area
        bzero(buff, sizeof(buff));
        n = read(newsockfd, buff, sizeof(buff)-1);
        socket = n;
        if(n < 0){
            error("ERROR reading from socket");
        }
        socket=1;
        snprintf(body, sizeof(body),
        "<html>\n<body>\n<h1>Hello web browser</h1>Your request was\n<pre>%s</pre>\n</body>\n</html>\n", buff);

        //https://stackoverflow.com/questions/53417494/checking-if-a-text-file-is-empty
        bzero(addr_buff, sizeof(addr_buff));
        dest(buff, addr_buff);
        
        //Copy and concatenate www-path and address.
        strcpy(tot_addr, wwwp);
        strcat(tot_addr, addr_buff);

        char * f_buff = 0;
        long l;
        
        //Generate file path
        FILE * filepath = fopen (tot_addr, "r+");
        if (filepath == NULL ){
            strcpy(tot_addr, wwwp);
            strcat(tot_addr, "/error.html");
            errorNum= 1;
            filepath = fopen (tot_addr, "rb");
            errorNum = 1;
        }
        //reading a fil; https://stackoverflow.com/questions/70767909/reading-a-file-and-print-the-content-in-c
        char * rbuff;
        errorNum = 0;
        fseek (filepath, 0, SEEK_END);
        l = ftell (filepath);
        fseek (filepath, 0, SEEK_SET);
        //Allocated memory to the given filesize
        f_buff = malloc (l);
        if (f_buff)
            {
                fread (f_buff, 1, l, filepath);
            }
        fclose (filepath);
        int closed = -1;
        bzero(tot_addr, sizeof(tot_addr));
        if(errorNum == 0) {
            snprintf(message, sizeof(message),
            "HTTP/1.0 200 OK\n Content-Type: text/html\n Content-Length: %lu\n\n%s", strlen(f_buff), f_buff);
        } else {
            snprintf(message, sizeof(message),
            "HTTP/1.0 404 Not found\n Content-Type: text/html\n Content-Length: %lu\n\n%s", strlen(f_buff), f_buff);
        }
        //Prints done
        //Check write 
        n = write(newsockfd, message, strlen(message));
        

        if (n<0){
            error("ERROR writing to socket");
        }
        //CLOSE
        close(newsockfd);
        
    }       
}

// https://www.geeksforgeeks.org/multithreaded-servers-in-java/#:~:text=Multithreaded%20Server%3A%20A%20server%20having,clients%20at%20the%20same%20time.
// https://www.educative.io/edpresso/how-to-implement-tcp-sockets-in-c

int main(int argc, char *argv[]){
    
    //What does this do? Found on stack overflow.
    arguments(argc, argv);

    //create threads
    pthread_t t[threads];
    int temp_arg[threads];
    int j = 0;
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
        j++;
    }
    listen(sockfd, 5);
    if(j<50){
        int sock = 2;
    }
    while(1){
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        bb_add(bb, newsockfd);
    }
}
