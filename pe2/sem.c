#include "sem.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

typedef struct SEM {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
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
SEM *sem_init(int initVal) {

    int error_checker;
    //allocates the semaphore here
    SEM *semPh = malloc(sizeof(SEM));

    // checks if it was a success - if not; return NULL
    if(semPh == NULL) {
        return NULL;
    }

    //counter == initial value 
    semPh->count = initVal;
    semPh->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    semPh->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    error_checker = pthread_mut_init(&semPh->mutex, NULL);
    
    if(error_checker != 0) {
        //free sempahores
        free(semPh);
        //returns NULL if there occurs an error
        return NULL;
    }

    error_checker = pthread_cond_init(&semPh->condition, NULL);
    if(error_checker != 0){
        pthread_mut_destroy(&semPh->mutex);
        //frees semaphores
        free(semPh);
        //returns NULL if there occurs an error
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

    int error_checker;
    //destroy mutex
    error_checker = pthread_mut_destroy(&sem->mutex);
    //destroy the conditions
    error_checker -= pthread_cond_destroy(&sem->condition)
    //free the semaphore
    free(sem);

    
    return error_checker;
    
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
void P(SEM *semPh){

    //locks the mutex
    pthread_mut_lock(&semPh->mutex);
    while( (semPh->count) == 0){
        pthread_condition_wait(&semPh->condition, &semPh->mutex);
    }

    //avoid threads from entering
    semPh->count --;
    //unlocks the semaphore
    pthread_mut_unlock(&semPh->mutex);
    return;
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

void V(SEM *semPh){
    //locks the semaphore
    pthread_mut_lock(&semPh->mutex);

    if(semPh->count == 0 ) {
        //unblocks at least one of the threads that are blocked on the specified condition variable cond 
        pthread_cond_signal(&semPh->condition);
    }

    semPh->count ++;
    pthread_mut_unlock(&semPh->mutex);
    return;
}