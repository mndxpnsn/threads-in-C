//
//  main.c
//  ThreadsInC
//
//  Created by mndx on 17/02/2022.
//  Playing with threads on macOS.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/sysctl.h>

#define NUM_THREADS 10

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

int get_number_hw_threads(void) {
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    
    return count;
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
    
    // get number of hardware threads used
    int num_hardware_threads = get_number_hw_threads();

    // wait for the threads to exit
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(thread_ids[i], NULL);
    
    // print data
    for (int i = 0; i < NUM_THREADS; ++i)
        printf("%i: %i\n", i, args_and_data_vec[i].data);
    
    printf("number of hardware threads: %i\n", num_hardware_threads);
    
    return 0;
}
