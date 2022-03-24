#include "sem.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

// Often used; https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html

/*
 * Semaphore implementation for the synchronization of POSIX threads.
 *
 * This module implements counting P/V semaphores suitable for the 
 * synchronization of POSIX threads. POSIX mutexes and condition variables 
 * shall be utilized to implement the semaphore operations.
 */

/* Opaque type of a semaphore. 
 * ...you need to figure out the contents of struct SEM yourself!
 */
typedef struct SEM {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t conditional;
} SEM;



/* Creates a new semaphore.
 *
 * This function creates a new semaphore. If an error occurs during the 
 * initialization, the implementation shall free all resources already 
 * allocated so far.
 *
 * Parameters:
 *
 * initVal      the initial value of the semaphore
 *
 * Returns:
 *
 * handle for the created semaphore, or NULL if an error occured.
 */
SEM *sem_init(int initialValue) {
    int check_error;

    //Allocates the semaphore here
    SEM *semPh = malloc(sizeof(SEM));

    //checks if we succeded in allocating the semaphore
    if (semPh == NULL) {
        return NULL;
    }

    //count == initial value
    semPh->count = initialValue;

    // https://stackoverflow.com/questions/14320041/pthread-mutex-initializer-vs-pthread-mutex-init-mutex-param
    semPh->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    semPh->conditional = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    check_error = pthread_mutex_init(&semPh->mutex, NULL);
    
    if(check_error != 0) {
        free(semPh);
        return NULL;
    }
    check_error = pthread_cond_init(&semPh->conditional, NULL);

    // https://pubs.opengroup.org/onlinepubs/007904875/functions/pthread_mutex_destroy.html
    if(check_error != 0){
        pthread_mutex_destroy(&semPh->mutex);
        free(semPh);
        return NULL;
    }

    return semPh;
}

/* Destroys a semaphore and frees all associated resources.
 *
 * Parameters:
 *
 * sem           handle of the semaphore to destroy
 *
 * Returns:
 *
 * 0 on success, negative value on error. 
 *
 * In case of an error, not all resources may have been freed, but 
 * nevertheless the semaphore handle must not be used any more.
 */
int sem_del(SEM *sem){
    int detecting_error;
    //destroy mutex (attempting)
    detecting_error = pthread_mutex_destroy(&sem->mutex);
    //destroy conditional (attempting)
    detecting_error -= pthread_cond_destroy(&sem->mutex);
    free(sem);
    return detecting_error;
}


/* V (signal) operation.
*
* Increments the semaphore value by 1 and notifies P operations that are 
* blocked on the semaphore of the change.
*
* Parameters:
*
* sem           handle of the semaphore to increment
*/
void signal(SEM *sem){

    //Locks semaphore
    pthread_mutex_lock(&sem->mutex);
    sem->count ++;
    if(sem->count == 1 ) {
        //Unlocks one of the threads
        pthread_cond_signal(&sem->conditional);
    }
    pthread_mutex_unlock(&sem->mutex);
    return;
}

/* P (wait) operation.
* 
* Attempts to decrement the semaphore value by 1. If the semaphore value 
* is 0, the operation blocks until a V operation increments the value and 
* the P operation succeeds.
*
* Parameters:
*
* sem           handle of the semaphore to decrement
*/
void wait(SEM *sem){

    pthread_mutex_lock(&sem->mutex);
    while(sem->count == 0){
        pthread_cond_wait(&sem->conditional, &sem->mutex);
    }

    sem->count --;

    pthread_mutex_unlock(&sem->mutex);
    return;
}