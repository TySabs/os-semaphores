/***********************************************************************
 * Name: Tyler Saballus
 * Date: 10/26/2018
 * Operating Systems 480 - Hutchins - Boley/Crumpton
 * Assignment 5 - Process Synchronization
 ***********************************************************************/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using std::cerr;
using std::endl;

const int P_NUMBER = 6;
const int C_NUMBER = 4;
const int BUFFER_SIZE = 12;
const int P_STEPS = 4;
const int C_STEPS = 6;

int COUNTER = 0;
sem_t NOT_FULL;
sem_t NOT_EMPTY;
pthread_mutex_t lock;


int insert_widget(void *tid) {
  pthread_mutex_lock(&lock);
  COUNTER++;
  cerr << "Producer " << (long) tid << " inserted one item. Total is now " << COUNTER << endl;
  pthread_mutex_unlock(&lock);
  return 0;
}

int remove_widget(void *tid) {
  pthread_mutex_lock(&lock);
  COUNTER--;
  cerr << "Consumer " << (long) tid << " removed one item. Total is now " << COUNTER << endl;
  pthread_mutex_unlock(&lock);
  return 0;
}

void *produce(void *tid) {
  for (int i = 0; i < P_STEPS; i++) {
    sem_wait(&NOT_FULL);
    insert_widget(tid);
    sleep(1);
    sem_post(&NOT_EMPTY);
  }

  pthread_exit(0);
  return NULL;
}

void *consume(void *tid) {
  for (int i = 0; i < C_STEPS; i++) {
    sem_wait(&NOT_EMPTY);
    remove_widget(tid);
    sleep(1);
    sem_post(&NOT_FULL);
  }

  pthread_exit(0);
  return NULL;
}

int main (int argc, char *argv[]) {
  pthread_t threads[6];
  int insertCode, removeCode;

  cerr << "Simulation of Producers and Consumers" << endl << endl;

  sem_init(&NOT_FULL, 0, BUFFER_SIZE);
  sem_init(&NOT_EMPTY, 0, 0);
  pthread_mutex_init(&lock, NULL);

  for (long i = 0; i < P_NUMBER; i++) {
    insertCode = pthread_create(&threads[i], NULL, produce, (void *) i);
    if (insertCode) {
      printf("ERROR! return code from pthread_create() is %d\n", insertCode);
      exit(-1);
    }
  }

  for (long i = 0; i < C_NUMBER; i++) {
    removeCode = pthread_create(&threads[i], NULL, consume, (void *) i);
    if (removeCode) {
      printf("ERROR! return code from pthread_create() is %d\n", removeCode);
      exit(-1);
    }
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(threads[i], NULL);
  }

  cerr << endl << "All the producer and consumer threads have been closed." << endl << endl;


  pthread_mutex_destroy(&lock);
  sem_destroy(&NOT_FULL);
  sem_destroy(&NOT_EMPTY);

  cerr << "The semaphores and mutex have been deleted." << endl;

  pthread_exit(NULL);
}
