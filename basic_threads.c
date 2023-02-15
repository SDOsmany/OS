#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *entry_point(void *value){
    printf("Hello from the second thread :)\n");

    int *num = (int *)value;
    printf("The value is: %d\n", *num);
    return NULL;
}

int main(int argc, char *argv[])
{
    
    pthread_mutex_init (&mutex, NULL); // initialize mutex
     /* Required to schedule thread independently. Otherwise, use NULL in place of attr. */
    pthread_attr_init(&attr[0]);
    pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);  /* system-wide contention */ 
    /* end to schedule thread independently */

    pthread_t thread1, thread2; // declare two threads
    int num = 123;

    printf("Hello from the main thread\n");
    pthread_create(&thread1,NULL,entry_point, &num); // create thread1


    pthread_join(thread1,NULL); // wait for thread1 to finish
    // printf("Hello World\n");
    return 0;
}