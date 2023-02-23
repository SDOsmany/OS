/*
Osmany Pujol
PID: 6284064
Program Description: A program to practice synchronization using
semaphores. The program creates three threads that increment a shared
variable. The first thread increments the shared variable by 1 or by
100 every time the value of the shared variable is divisible by 100
and the second and third thread increments the shared variable by 1. The
program uses sem_wait() and sem_post() to ensure that the shared variable is
updated correctly. The program also prints the number of times
each thread updates the shared variable and the final value of
the shared variable.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

struct shared_data
{
    int value; /* shared variable to store result*/
};
#define NUM_THREADS 3
sem_t key; // semaphore
struct shared_data *counter;

/**
 * This function increments the shared variable by 1
 * @param param is there for the function to be compatible with pthread_create
 */
void *increment(void *thread_num)
{
    int thread_update = 0;
    for (;;)
    {
        /* START OF THE CRITICAL SECTION*/
        int i = sem_wait(&key); // lock semaphore
        if (i != 0)
        {
            printf("Error locking semaphore\n");
            exit(1);
        }
        // We check if the thread has done 2 million updates
        if (thread_update == 2000000)
        {
            printf("Im Thread %d, I did %d updates, counter = %d\n", *(int *)thread_num, thread_update, counter->value);
            sem_post(&key); // unlock sempahore
            return NULL;
        }

        counter->value += 1;
        /* EXIT SECTION*/
        sem_post(&key); // unlock sempahore
        /* REMAINDER SECTION*/
        thread_update++;
    }

    return NULL;
}

/**
 * This function increments the shared variable by 1 or by 100
 * every time the value of the shared variable is divisible by 100.
 * The function also prints the number of times the thread updates
 * the shared variable and the final value of the shared variable.
 * @param param is there for the function to be compatible with pthread_create
 */
void *increment_bonus(void *thread_num)
{
    int thread_update = 0;
    int bonus = 0;
    for (;;)
    {
        /* START OF THE CRITICAL SECTION*/
        int i = sem_wait(&key); // lock semaphore
        if (i != 0)
        {
            printf("Error locking semaphore\n");
            exit(1);
        }
        // We check if the thread has done 2 million updates
        if (thread_update >= 2000000)
        {
            printf("Im Thread %d, I did %d updates and i got bonus for %d times, counter = %d\n", *(int *)thread_num, thread_update, bonus, counter->value);
            sem_post(&key); // unlock sempahore
            return NULL;
        }
        /* We check if the value of the shared variable is divisible by 100
         and prevent the addition of 100 to go beyond 2 million */
        if (counter->value % 100 == 0 && thread_update <= 1999900)
        {
            counter->value += 100;
            bonus += 1;
            thread_update += 100;
        }
        else
        {
            counter->value += 1;
            thread_update++;
        }
        /* EXIT SECTION*/
        sem_post(&key); // unlock sempahore
        /* REMAINDER SECTION*/
    }

    return NULL;
}
int main(int argc, char *argv[])
{
    sem_init(&key, 0, 1);                                               // initialize semaphore
    pthread_t threads[NUM_THREADS];                                     // declare three threads
    counter = (struct shared_data *)malloc(sizeof(struct shared_data)); // allocate memory for shared data
    counter->value = 0;                                                 // initialize shared data

    /* Create all the threads*/
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int *thread_num = malloc(sizeof(int));
        *thread_num = i + 1;
        /*ENTRY SECTION*/
        if (i == 0)
        {
            pthread_create(&threads[i], NULL, increment_bonus, thread_num); // create thread1
        }
        else
        {
            pthread_create(&threads[i], NULL, increment, thread_num); // create thread2 and thread3
        }
    }


    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL); // wait for all threads to finish
    }

    sem_destroy(&key); // destroy semaphore
    free(counter);     // free memory

    printf("from parent counter = %d\n", counter->value); // print the final value of the shared variable
    return 0;
}