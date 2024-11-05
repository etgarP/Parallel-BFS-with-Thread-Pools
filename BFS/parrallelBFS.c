#include "../SyncGraph/graph.h"
#include "../ThreadPool/ThreadPool.h"
#include "../ThreadPool/TaskQueue.h"

typedef struct bfsArgs {
    Graph *graph;
    vertex v;
    ThreadPool* q1;
    ThreadPool* q2;
    int **m;
    atomic_int *vertexPerLevel;
    int level;
} bfsArgs;

bfsArgs* bfsArgsInit(Graph *graph, vertex v, ThreadPool* q1, ThreadPool* q2,
                        int **m, atomic_int *l, int lvl) {
    bfsArgs *args = malloc(sizeof(bfsArgs));
    args->graph = graph;
    args->v = v;
    args->q1 = q1;
    args->q2 = q2;
    args->m = m;
    args->vertexPerLevel = l;
    args->level = lvl;
    return args;
}

typedef struct dfsArgs dfsArgs;

void * parallel_bfs_visit(void *args) {

    /* Extract arguments */
    bfsArgs *data = (bfsArgs *) args;
    Graph *graph = data->graph;
    vertex v = data->v;
    ThreadPool *q1 = data->q1;
    ThreadPool *q2 = data->q2;
    int **m = data->m;
    atomic_int *vertexPerLevel = data->vertexPerLevel;
    int level = data->level;
    free(data);

    // printf("started vector: %d\n", v);

    /* Start the visit */
    node *neighborsPtr = graph->adjacencyLists[v];

    while (neighborsPtr != NULL) {
        vertex neighbor = neighborsPtr->v;
        /* Synchronously increment the number of visits for the neighbor */
        pthread_mutex_lock(&graph->num_visits_mutexes[neighbor]);
        int neighborVisits = graph->numVisits[neighbor]++;
        pthread_mutex_unlock(&graph->num_visits_mutexes[neighbor]);

        /* If the neighbor has not been visited, add a new task to the queue */
        if (neighborVisits == 0) {
            m[neighbor][v]++;
            m[v][neighbor]++;
            bfsArgs *params = bfsArgsInit(graph, neighbor, q2, q1, m, vertexPerLevel, level + 1);
            TaskData td = {parallel_bfs_visit, params};
            ++vertexPerLevel[level+1];
            // printf("added: (%d, %d)", neighbor, v);
            insert(q2->q, td);
        }
        neighborsPtr = neighborsPtr->next;
    }

    /* Decrement the number of running threads */
    int valueBefore = vertexPerLevel[level];
    --(vertexPerLevel[level]);
    // printf("\nleft vector: %d. %d->%d\n", v, valueBefore, vertexPerLevel[level]);
    --(q1->runningThreads);
}

void bfs(Graph *graph, int **m) {
    int n = graph->numVertices;
    int numVertices = n;
    bfsArgs *args;
    /* For each vertex, if it is not visited, perform DFS. */
    for (vertex v = 0; v < numVertices; v++) {
        if (graph->numVisits[v] == 0) {
            /* Init thread pool */

            atomic_int *vertexPerLevel = malloc(n * sizeof(atomic_int));
            vertexPerLevel[0] = 1;
            ThreadPool* pool = initThreadPool(4, 0, vertexPerLevel);

            ThreadPool* pool2 = initThreadPool(4, 0, vertexPerLevel);

            /* Insert the root task to the queue */    
            args = bfsArgsInit(graph, v, pool, pool2, m, vertexPerLevel, 0);

            //pthread_mutex_lock(&graph->num_visits_mutexes[neighbor]);
            graph->numVisits[v]++;
            //pthread_mutex_unlock(&graph->num_visits_mutexes[neighbor]);

            TaskData td = {parallel_bfs_visit, args};
            insert(pool->q, td);
            
            /* Start the tasks */
            runThreadPool(pool, pool2);
        }
    }
}