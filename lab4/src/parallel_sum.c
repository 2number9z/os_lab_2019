#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include <pthread.h>

#include "/projects/2number9z-os_lab_2019/lab3/src/utils.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here
  printf("BEGINS %d   ENDS %d\n", (*args).begin, (*args).end);
  int i = (*args).begin;
  for (; i < (*args).end; i++){
      printf("%d = %d ", i+1, (*args).array[i]);
      sum += (*args).array[i];
  }
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  /*
   *  TODO:
   *  threads_num by command line arguments
   *  array_size by command line arguments
   *	seed by command line arguments
   */


  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;
//   pthread_t threads[threads_num];

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"threads_num", required_argument, 0, 0},
                                      {"timeout", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            if (seed <= 0) {
            printf("seed is a positive number\n");
            return 1;
            }
            // error handling
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            if (array_size <= 0) {
            printf("array_size is a positive number\n");
            return 1;
            }
            // error handling
            break;
          case 2:
            threads_num = atoi(optarg);
            // your code here
            if (threads_num < 1) {
            printf("at least 1 additional thread should be started\n");
            return 1;
            }
            // error handling
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case '?':
        break;
      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || threads_num == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --threads_num \"num\" \n",
           argv[0]);
    return 1;
  }

  pthread_t threads[threads_num];

  /*
   * TODO:
   * your code here
   * Generate array here
   */

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int k = 0;
  for (; k<array_size; k++){
      printf("%d ", array[k]);
  }
  printf("\n");

  struct SumArgs args[threads_num];
  uint32_t i = 0;
  for (; i < threads_num; i++) {
      args[i].array = array;
      args[i].begin = (array_size/threads_num)*i;
      args[i].end = (array_size/threads_num)*(i+1);
      printf("\n%d thread begin: %d , end %d\n", i+1, args[i].begin, args[i].end);
    // if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
    //   printf("Error: pthread_create failed!\n");
    //   return 1;
    // }
    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)(&args+1))) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  i = 0;
  for (; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    printf("\n%d thread sum = %d ", i+1, sum);
    total_sum += sum;
  }

  free(array);
  printf("Total: %d\n", total_sum);
  return 0;
}
