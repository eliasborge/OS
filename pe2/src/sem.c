#include <sem.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
 #include <stdio.h> 

SEM sem = sem_init(2);

struct SEM {
    int pshared;
    unsigned int val;
    //volatile int val; //number of tokens
    pthread_mutex_t m;
    pthread_cond_t c;
    
};

//Reference: https://stackoverflow.com/questions/60224504/how-to-create-semaphores-in-c
SEM *sem_init(int initVal) {
    SEM *sem = malloc(sizeof(SEM));
    if (!sem)
        goto Error1;

    sem->val = initVal;

    errno = pthread_mutex_init(&sem->m, NULL);
    if (!errno)
        goto Error2;

    errno = pthread_cond_init(&sem->c, NULL);
    if (!errno)
        goto Error3;

    return sem;

//Free the semaphores before you're done with it
Error3:
    pthread_mutex_destroy(&sem->m);
Error2:
    free(buf);
Error1:
    return NULL;
}


void P(SEM *sem) {
    pthread_mutex_lock(&sem->m);

   // Wait for the semaphore to have a positive value.
   while (sem->val < 1)
      pthread_cond_wait(&sem->c, &sem->m);

   --sem->val;

   // Wake up a thread that's waiting, if any.
   if (sem->val > 0)
      pthread_cond_signal(&sem->c);

   pthread_mutex_unlock(&sem->m);
}




//Reference: https://www.geeksforgeeks.org/use-posix-semaphores-c/
void* thread(void* arg) {
    sem_wait(&mutex);
    printf("\nEntered..\n");

    sleep(5);

    printf("\nJust Exiting\n");
    sem_post(&mutex);


    // Source: https://stackoverflow.com/questions/43801718/implementing-counting-semaphore-using-binary-semaphore
    struct counting_semaphore* counting_semaphore_alloc(int value) {

    struct counting_semaphore* sem = malloc(sizeof(struct counting_semaphore));
    sem->value = value;
    sem->binary_descriptor1= bsem_alloc();//locks the value
    sem->binary_descriptor2= bsem_alloc();//locks the sing of the value
    if (value <= 0) {
        bsem_down(sem->binary_descriptor2);
    }
    return sem;
    }

void down(struct counting_semaphore *sem){
    bsem_down(sem->binary_descriptor2);
    bsem_down(sem->binary_descriptor1);
    sem->value--;
    if (sem->value>0){
        bsem_up(sem->binary_descriptor2);
    }
    bsem_up(sem->binary_descriptor1);
}


void up(struct counting_semaphore* sem) {
    bsem_down(sem->binary_descriptor1);
    sem->value++;
    if (sem->value == 1) {
        bsem_up(sem->binary_descriptor2);
    }
    bsem_up(sem->binary_descriptor1);
}
}