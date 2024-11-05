#include "ThreadPool.h"
#include "TaskQueue.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void swapPP(ThreadPool **tp1, ThreadPool **tp2) {
    ThreadPool *temp = *tp1;
    *tp1 = *tp2;
    *tp2 = temp;
}

ThreadPool* initThreadPool(unsigned int maxThreads, atomic_int runningThreads, 
                    atomic_int *vPerLevel) {
    // Allocate memory for the ThreadPool structure
    ThreadPool *t = (ThreadPool*) malloc(sizeof(ThreadPool));
    if (t == NULL) {
        perror("Failed to allocate memory for ThreadPool");
        exit(EXIT_FAILURE);
    }
    // Initialize ThreadPool members
    t->maxThreads = maxThreads;
    t->runningThreads = runningThreads;
    t->vPerLevel = vPerLevel;
    t->q = (TaskQueue*) malloc(sizeof(TaskQueue));
    initQueue(t->q);
    return t;
}


void runThreadPool(ThreadPool *tp1, ThreadPool *tp2) {
    // while we still have more levels
    atomic_int* vertexLevel = tp1->vPerLevel;
    for(int i = 0; vertexLevel[i] > 0; i++) {
        // printf("level: %d\n", i);
        vertexLevel[i+1] = 0;
        // we keep going until everyone finished in the level;
        while(vertexLevel[i] > 0) {
            // printf("left: %d\n", vertexLevel[i]);
            /* If we are not past max number of threads */
            if (tp1->runningThreads < tp1->maxThreads && !isEmpty(tp1->q)) {
                /* Run task */
                TaskData data = pop(tp1->q);

                pthread_t thread;
                if (pthread_create(&thread, NULL, data.function, data.args)) {
                    perror("pthread_create failed.");
                    exit(1);
                } else {
                    ++(tp1->runningThreads);
                }
            }
        }
        swapPP(&tp1, &tp2);
    }
}