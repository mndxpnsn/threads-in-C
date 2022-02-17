//
//  main.c
//  ThreadsInC
//
//  Created by mndx on 17/02/2022.
//  Playing with threads.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 8

typedef struct user_type {
    int arg;
    int data;
} u_type;

void * runner(void * args_and_data) {
    
    // get arguments
    u_type * var = (u_type *) args_and_data;
    // set data
    var->data = var->arg / 2;
    // exit success
    pthread_exit(0);
}

int main(int argc, const char * argv[]) {
    
    // declarations
    pthread_attr_t thread_attr[NUM_THREADS];
    pthread_t thread_ids[NUM_THREADS];
    u_type * args_and_data_vec = (u_type *) calloc(NUM_THREADS, NUM_THREADS * sizeof(u_type));
    
    // set runner arguments
    for (int i = 0; i < NUM_THREADS; ++i)
        args_and_data_vec[i].arg = 2 * (i + 1);

    // create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        // set the default attributes of the thread
        pthread_attr_init(&thread_attr[i]);
        // create the thread
        pthread_create(&thread_ids[i], &thread_attr[i], runner, &args_and_data_vec[i]);
    }

    // wait for the threads to exit
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(thread_ids[i], NULL);
    
    // print data
    for (int i = 0; i < NUM_THREADS; ++i)
        printf("%i: %i\n", i, args_and_data_vec[i].data);
    
    return 0;
}
