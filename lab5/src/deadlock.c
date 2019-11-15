#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_one_thing();
void do_another_thing();
int var1 = 5;
int var2 = 6;
pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, (void *)do_one_thing, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)do_another_thing, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  return 0;
}

void do_one_thing() {
    int temp1;
    int temp2;
    pthread_mutex_lock(&mut1);
    printf("thread 1, getting var1\n");
    temp1 = var1;
    sleep(1);   // чтобы второй поток успел залочить mut2
    pthread_mutex_lock(&mut2);
    temp2 = var2;
    printf("thread 1, getting var2\n");
    printf("thread 1: sum = %d\n", temp1 + temp2);
    pthread_mutex_unlock(&mut2);
	pthread_mutex_unlock(&mut1);
}

void do_another_thing() {
    int temp1;
    int temp2;
    pthread_mutex_lock(&mut2);
    printf("thread 2, getting var2\n");
    temp1 = var2;
    // sleep(1);
    pthread_mutex_lock(&mut1);
    temp2 = var1;
    printf("thread 2, getting var1\n");
    printf("thread 2: sum = %d\n", temp1 + temp2);
    pthread_mutex_unlock(&mut1);
	pthread_mutex_unlock(&mut2);
}
