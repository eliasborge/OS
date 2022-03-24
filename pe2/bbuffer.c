#include "sem.h"
#include "stdlib.h"
#include <stdio.h>

/*sources often used: https://www.elis.uni-erlangen.de/Lehre/SS11/V_SP2/Uebung/doc/xmt-httpd/jbuffer_8h.shtml
The text over the functions are taken from this site for instance
*/

/*
Opaque type of a Bounded Buffer.
*/
typedef struct BNDBUF {
    int inp;
    int outp;
    unsigned int count;
    int* buff_data;
    SEM* fill_buff;
    SEM* empt_buff;
} BNDBUF;

/*
Creates a new Bounded Buffer.

This function creates a new bounded buffer and all the helper data structures required by the buffer, including semaphores for synchronization. If an error occurs during the initialization the implementation shall free all resources already allocated by then.

Parameters:
size 	The number of integers that can be stored in the bounded buffer.
Returns:
handle for the created bounded buffer, or NULL if an error occured.
*/
BNDBUF *bb_init(unsigned int size){

    //Initialize bufferhttps://stackoverflow.com/questions/53417494/checking-if-a-text-file-is-empty
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
/*
Destrous a bounded buffer
This function adds an element to the bounded buffer. If the bounded buffer is full, the function blocks until an element is removed from the buffer.

Parameters:
bb 	Handle of the bounded buffer.
*/
void bb_del(BNDBUF *bb){

    free(bb->buff_data);
    sem_del(bb->fill_buff);
    sem_del(bb->empt_buff);
    return;

}
/*
Retrieve an element from the bounded buffer.

This function removes an element from the bounded buffer. If the bounded buffer is empty, the function blocks until an element is added to the buffer.

Parameters:
bb 	Handle of the bounded buffer.
Returns:
the int element
*/
int  bb_get(BNDBUF *bb){
    int data;
    wait(bb->fill_buff);
    data = bb->buff_data[bb->count-1];
    bb->count--;
    signal(bb->empt_buff);
    return data;
}

/*
Add an element to the bounded buffer.

This function adds an element to the bounded buffer. If the bounded buffer is full, the function blocks until an element is removed from the buffer.

Parameters:
bb 	Handle of the bounded buffer.
fd 	Value that shall be added to the buffer.
Returns:
the int element
*/
int bb_add(BNDBUF *bb, int fd) {
    //attempts to decrement the sem with one
    wait(bb->empt_buff);
    //adds element
    bb->buff_data[bb->count] = fd;
    bb->count ++;
    
    signal(bb->fill_buff);
    return fd;
}
