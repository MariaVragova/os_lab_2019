#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

void do_one_thing();
void do_another_thing();

pthread_mutex_t mutex1, mutex2;

int common = 0; 

int main() {
  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, (void*)do_one_thing, (void *)&common);
  pthread_create(&thread2, NULL, (void*)do_another_thing, (void *)&common);
  
  pthread_join(thread1, NULL); 
  pthread_join(thread2, NULL);

  return 0;
}

void do_one_thing() {
  pthread_mutex_lock(&mutex2);           // Захват 2-го мьютекса 1-ым тредом
  sleep(4);
  pthread_mutex_lock(&mutex1);           // Неудачная попытка захвата 1-го мьютекса 1-ым тредом
  printf("thread №1\n");
  pthread_mutex_unlock(&mutex1);
  pthread_mutex_unlock(&mutex2);
  }

void do_another_thing() {
  pthread_mutex_lock(&mutex1);           // Захват 1-го мьютекса 2-ым тредом
  sleep(4);
  pthread_mutex_lock(&mutex2);           // Неудачная попытка захвата 2-го мьютекса 2-ым тредом
  printf("thread №2\n");
  pthread_mutex_unlock(&mutex2);
  pthread_mutex_unlock(&mutex1);
  }