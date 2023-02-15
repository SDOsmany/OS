/*
Osmany Pujol
PID: 6284064
Program Description: A program that tests the collatz conjecture
using a parent and two child processes that run concurrently.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct shared_data
{
    int value; /* shared variable to store result*/
};

static pthread_mutex_t mutex; // mutex

void *increment(void *counter)
{
    int thread_update = 0;
    struct shared_data *data = (struct shared_data *)counter;
    for (;;)
    {
        pthread_mutex_trylock(&mutex); // lock mutex
        if (data->value == 4000000  || thread_update >= 2000000)
        {
            printf("Im Thread 2, I did %d updates, counter = %d\n", thread_update, data->value);
            pthread_mutex_unlock(&mutex); // unlock mutex
            return NULL;
        }
        data->value+=1;
        pthread_mutex_unlock(&mutex); // unlock mutex
        thread_update++;
    }

    return NULL;
}

void *increment_bonus(void *counter)
{
    int thread_update = 0;
    int bonus = 0;
    struct shared_data *data = (struct shared_data *)counter;
    for (;;)
    {
        pthread_mutex_trylock(&mutex); // lock mutex
        if (data->value == 4000000 || thread_update >= 2000000)
        {
            printf("Im Thread 1, I did %d updates and i got bonus for %d times, counter = %d\n", thread_update, bonus, data->value);
            pthread_mutex_unlock(&mutex); // unlock mutex
            return NULL;
        }
        if (data->value % 100 == 0){
            data->value += 100;
            bonus+=1;
            thread_update += 100;
        }
        else{
            data->value+=1;
            thread_update++;
        }
       
        pthread_mutex_unlock(&mutex); // unlock mutex
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    /* Required to schedule thread independently. Otherwise, use NULL in place of attr. */
    // pthread_attr_init(NULL);
    // pthread_attr_setscope(NULL, PTHREAD_SCOPE_SYSTEM); /* system-wide contention */
    /* end to schedule thread independently */

    pthread_mutex_init(&mutex, NULL); // initialize mutex
    pthread_t thread1, thread2;       // declare two threads
    struct shared_data *counter;      // shared data
    counter = (struct shared_data *)malloc(sizeof(struct shared_data));
    counter->value = 0; // initialize shared data

    pthread_create(&thread1, NULL, increment_bonus, counter); // create thread1
    pthread_create(&thread2, NULL, increment, counter);       // create thread2

    pthread_join(thread1, NULL); // wait for thread1 to finish
    pthread_join(thread2, NULL); // wait for thread1 to finish
    printf("from parent counter = %d\n", counter->value);
    return 0;
}