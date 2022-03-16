#include <sem.h>

//Reference: https://www.geeksforgeeks.org/use-posix-semaphores-c/
void* thread(void* arg) {
    sem_wait(&mutex);
    printf("\nEntered..\n");

    sleep(5);

    printf("\nJust Exiting\n");
    sem_post(&mutex);
}