#include "sem.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

typedef struct SEM {
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t conditional;
} SEM;



SEM *sem_init(int initialValue) {
    int check_error;

    //Allocates the semaphore here
    SEM *semPh = malloc(sizeof(SEM));

    //Checks semaphoure 
    if(semPh == NULL){
        return NULL;
    }

    semPh->count = initialValue;

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

int sem_del(SEM *sem){
    int n;
    n = pthread_mutex_destroy(&sem->mutex);
    free(sem);
    return n;
}



void signal(SEM *sem){
    /* V (signal) operation.
    *
    * Increments the semaphore value by 1 and notifies P operations that are 
    * blocked on the semaphore of the change.
    *
    * Parameters:
    *
    * sem           handle of the semaphore to increment
    */
    pthread_mutex_lock(&sem->mutex);
    sem->count ++;
    if(sem->count == 1 ) {
        //Unlocks one of the threads
        pthread_cond_signal(&sem->conditional);
    }
    pthread_mutex_unlock(&sem->mutex);
    return;
}

void wait(SEM *sem){
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
    pthread_mutex_lock(&sem->mutex);
    while(sem->count == 0){
        pthread_cond_wait(&sem->conditional, &sem->mutex);
    }

    sem->count --;

    pthread_mutex_unlock(&sem->mutex);
    return;
}