#include "sem.h"
#include "stdlib.h"
#include <stdio.h>

typedef struct BNDBUF {
    int inp;
    int outp;
    unsigned int count;
    int* buff_data;
    SEM* fill_buff;
    SEM* empt_buff;
} BNDBUF;


BNDBUF *bb_init(unsigned int size){

    //Initialize buffer
    BNDBUF *bb = malloc(sizeof(BNDBUF));
    bb->inp = 0;
    bb->outp = 0;
    bb->count = 0;
    bb->buff_data = malloc(sizeof(int)*size);
    bb->fill_buff = sem_init(0);
    if(bb->fill_buff == 0){
        free(bb->buff_data);
        return NULL;
    }
    bb->empt_buff = sem_init(size);
    if(bb->empt_buff == 0){
        free(bb->buff_data);
        sem_del(bb->empt_buff);
        return NULL;
    }

    return bb;
}

void bb_del(BNDBUF *bb){

    free(bb->buff_data);
    sem_del(bb->fill_buff);
    sem_del(bb->empt_buff);
    return;

}

int  bb_get(BNDBUF *bb){
    int data;
    wait(bb->fill_buff);
    data = bb->buff_data[bb->count-1];
    bb->count--;
    signal(bb->empt_buff);
    return data;
}

int bb_add(BNDBUF *bb, int fd) {
    wait(bb->empt_buff);
    bb->buff_data[bb->count] = fd;
    bb->count ++;
    signal(bb->fill_buff);
    return fd;
}
