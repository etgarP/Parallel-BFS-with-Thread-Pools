# Target executable name
TARGET = part_III.out

# Source files
SRCS = main.c BFS/parrallelBFS.c ThreadPool/ThreadPool.c ThreadPool/TaskQueue.c SyncGraph/graph.c

# Compiler and flags
CC = gcc -g
# CFLAGS = place flags if needed

# Default target builds the executable
$(TARGET): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o $(TARGET)

# Clean target removes the executable
clean:
	rm -f $(TARGET)

