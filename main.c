 /*
Osmany Pujol
PID: 6284064
Program Description: A program to practice synchronization using
mutexes. The program creates two threads that increment a shared
variable. The first thread increments the shared variable by 1
and the second thread increments the shared variable by 1 or by
100 every time the value of the shared variable is divisible by 100. The
program uses mutexes to ensure that the shared variable is
updated correctly. The program also prints the number of times
each thread updates the shared variable and the final value of
the shared variable.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct shared_data
{
    int value; /* shared variable to store result*/
};

static pthread_mutex_t mutex; // mutex
struct shared_data *counter;

/**
 * This function increments the shared variable by 1
 * @param param is there for the function to be compatible with pthread_create
 */
void *increment(void *param)
{
    int thread_update = 0;
    for (;;)
    {
        int i = pthread_mutex_trylock(&mutex); // lock mutex
        // We lazy wait for the mutex to be unlocked
        if (i != 0)
        {
            continue;
        }
        // We check if the thread has done 2 million updates
        if (thread_update == 2000000)
        {
            printf("Im Thread 2, I did %d updates, counter = %d\n", thread_update, counter->value);
            pthread_mutex_unlock(&mutex); // unlock mutex
            return NULL;
        }

        counter->value += 1;
        pthread_mutex_unlock(&mutex); // unlock mutex
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
void *increment_bonus(void *param)
{
    int thread_update = 0;
    int bonus = 0;
    for (;;)
    {
        int i = pthread_mutex_trylock(&mutex); // lock mutex
        // We lazy wait for the mutex to be unlocked
        if (i != 0)
        {
            continue;
        }
        // We check if the thread has done 2 million updates
        if (thread_update >= 2000000)
        {
            printf("Im Thread 1, I did %d updates and i got bonus for %d times, counter = %d\n", thread_update, bonus, counter->value);
            pthread_mutex_unlock(&mutex); // unlock mutex
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

        pthread_mutex_unlock(&mutex); // unlock mutex
    }

    return NULL;
}
int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);                                   // initialize mutex
    pthread_t thread1, thread2;                                         // declare two threads
    counter = (struct shared_data *)malloc(sizeof(struct shared_data)); // allocate memory for shared data
    counter->value = 0;                                                 // initialize shared data

    pthread_create(&thread1, NULL, increment_bonus, NULL); // create thread1
    pthread_create(&thread2, NULL, increment, NULL);       // create thread2

    pthread_join(thread1, NULL); // wait for thread1 to finish
    pthread_join(thread2, NULL); // wait for thread1 to finish

    pthread_mutex_destroy(&mutex); // destroy mutex
    // print the final value of the shared variable
    printf("from parent counter = %d\n", counter->value);
    return 0;
}