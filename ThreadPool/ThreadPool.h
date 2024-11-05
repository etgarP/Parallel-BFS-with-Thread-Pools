#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "TaskQueue.h"
#include <stdatomic.h>
#include <pthread.h>

struct ThreadPool {
    unsigned int maxThreads;
    atomic_int runningThreads;
    atomic_int * vPerLevel;
    TaskQueue *q;
};

typedef struct ThreadPool ThreadPool;

void runThreadPool(ThreadPool *tp1, ThreadPool *tp2);

ThreadPool* initThreadPool(unsigned int maxThreads, atomic_int runningThreads, 
                    atomic_int *vPerLevel);

#endif
