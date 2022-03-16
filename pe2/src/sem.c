#include <sem.h>
#include <semaphore.h>
#include <pthread.h>

SEM sem = sem_init(2);






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