# Parallel BFS with Thread Pools

This repository implements a parallelized Breadth-First Search (BFS) algorithm on a graph using thread pools and task queues in C. The code leverages multi-threading to explore graph vertices concurrently, improving performance on large graphs by taking advantage of multi-core processors.

## Project Structure

- **SyncGraph/graph.h**: Contains the `Graph` structure and functions to create and manage graphs.
- **ThreadPool/ThreadPool.h** and **ThreadPool/TaskQueue.h**: Provide utilities for managing threads and task queues, enabling concurrent tasks to run in a thread-safe manner.
- **main.c**: Initializes the graph, inserts random edges, and runs the BFS algorithm.

## Detailed Implementation

### BFS Overview

The BFS implementation leverages concurrency by assigning each level of BFS traversal to a new set of tasks. The tasks are executed by worker threads within a thread pool, allowing multiple vertices at each BFS level to be explored simultaneously.

### Key Components

1. **Graph Representation**: The graph is represented using adjacency lists. Each `Graph` object holds an array of lists (`adjacencyLists`), where each list contains nodes corresponding to neighboring vertices. This structure enables efficient edge traversal.

2. **Thread Pools and Task Queues**:
   - `ThreadPool`: Manages the creation and execution of threads. Each thread executes tasks from the `TaskQueue`, a synchronized queue structure that holds BFS tasks.
   - `TaskQueue`: Stores tasks (i.e., function pointers and arguments) and supports synchronized insertion and removal using mutex locks to ensure thread safety.

3. **Parallel BFS Tasks**:
   - **Task Arguments (`bfsArgs`)**: To pass the relevant data to each BFS task, the `bfsArgs` structure holds the current vertex, graph, thread pools, and other required details.
   - **Task Execution**: The function `parallel_bfs_visit` is invoked for each vertex being visited. This function:
      1. Retrieves the neighbors of the current vertex.
      2. For each unvisited neighbor, increments a visit counter (protected by a mutex).
      3. Adds a new task to visit the neighbor in the next level if it hasn’t been visited before.
   - **Synchronization**: Atomic counters and mutexes ensure that shared resources (e.g., `vertexPerLevel` counters) are updated safely, avoiding race conditions.

4. **Control Flow in `runThreadPool`**:
   - The thread pool alternates between two queues (`q1` and `q2`) to manage BFS levels. Each level’s tasks are processed until no tasks remain in the current queue. The `swapPP` function swaps the active queues after each level is complete, allowing the next level to proceed.

5. **Final BFS Tree Output**:
   - The adjacency matrix (`m`) is updated during traversal, marking edges visited during BFS. After BFS completes, the matrix represents the BFS tree, displaying the shortest paths from each root vertex to other vertices.

## Building and Running the Code

### Makefile

The Makefile automates the build process, compiling all necessary files and creating an executable.

### Running the Program

1. **Build**: Run the following command to build the executable:
   ```bash
   make
2. **Execute**: After building, run the BFS program with:

   ```bash
   Copy code
   /part_III.out

This will:

   - Randomly generate a graph with vertices and edges.
   - Perform a parallel BFS traversal.
   - Output the initial adjacency matrix and the BFS tree adjacency matrix.
2. **Clean**: To remove the executable, run:

   ```bash
   Copy code
   make clean

### Sample Output
The program displays:

   - The initial adjacency matrix of the randomly generated graph.
   - The BFS tree adjacency matrix, representing the shortest paths after traversal.
