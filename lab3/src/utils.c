#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  int i = 0;
  for (; i < array_size; i++) {
    // array[i] = rand() / 100000;
    array[i] = rand() / 100;
  }
}
