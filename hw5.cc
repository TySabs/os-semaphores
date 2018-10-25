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

int insert(void *tid) {
  sem_wait(&NOT_FULL);
  COUNTER++;
  cerr << "Inserting to counter. Now at: " << COUNTER << endl;
  sem_post(&NOT_FULL);
  return 0;
}

int remove(void *tid) {
  sem_wait(&NOT_EMPTY);
  COUNTER--;
  cerr << "Removing from counter. Now at: " << COUNTER << endl;
  sem_post(&NOT_EMPTY);
  return 0;
}

void *produce(void *tid) {
  for (int i = 0; i < P_STEPS; i++) {
    insert(tid);
  }

  pthread_exit(0);
  return NULL;
}

void *consume(void *tid) {
  for (int i = 0; i < C_STEPS; i++) {
    remove(tid);
  }

  return NULL;
}

int main (int argc, char *argv[]) {
  pthread_t threads[5];
  int createCode;
  long i;

  cerr << "Simulation of Producers and Consumers" << endl << endl;

  sem_init(&NOT_FULL, 0, BUFFER_SIZE);
  sem_init(&NOT_EMPTY, 0, 0);

  for (i = 0; i < 5; i++) {
    createCode = pthread_create(&threads[i], NULL, produce, (void *) i);
    if (createCode) {
      printf("ERROR! return code from pthread_create() is %d\n", createCode);
      exit(-1);
    }
  }

  for (i = 0; i < 5; i++) {
    pthread_join(threads[i], NULL);
  }

 pthread_exit(NULL);
}
