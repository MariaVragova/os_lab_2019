CC=gcc
CFLAGS=-I.
THREAD_FLAGS = -pthread -lpthread

all : task_1 task_3 parallel_sum


task_1 : parallel_min_max


task_3 : process_memory


parallel_sum : utils.o utils.h sum.o sum.h
	$(CC) parallel_sum.c utils.o sum.o -o parallel_min_max $(THREAD_FLAGS)

process_memory : 
	$(CC) process_memory.c -o process_memory

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) parallel_min_max.c utils.o find_min_max.o -o parallel_min_max $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

sum.o : sum.h
	$(CC) -o sum.o -c sum.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h	
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o process_memory parallel_min_max parallel_sum