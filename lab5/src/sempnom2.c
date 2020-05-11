#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>

sem_t semaphore;
static int factorial_answer = 1;

typedef struct {
  int* numbers_array;
  int begin;
  int end;
} 
FactorialArgs;

void *start_factorial(void *args) 
{
  FactorialArgs *local_part = (FactorialArgs*)args;
  int local_answer = 1;
  sem_wait(&semaphore);
  for (int i = local_part->begin; i < local_part->end; i++)

    local_answer *= local_part->numbers_array[i];
  factorial_answer *= local_answer;
  sem_post(&semaphore);
}

int main(int argc, char **argv) {

  uint32_t k = -1;
  uint32_t pnum = -1;
  uint32_t mod = -1;

  // Параметры командой строки
  while (1) {
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "?", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            break;
          case 1:
            pnum = atoi(optarg);
            break;
          case 2:
            mod = atoi(optarg);
            break;
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

  if (k == -1 || pnum == -1 || mod == -1) {
        printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = (int*)malloc(k * sizeof(int));
  for (int i = 0; i < k; i++) {
    array[i] = i + 1;
  }

  int active_step = k > pnum ? (k / pnum) : 1;
  // pthread_mutex_init(&factorial_mutex, NULL);
  sem_init(&semaphore, 0, 1);
  pthread_t threads[pnum];
  FactorialArgs factorial_args[pnum];
  for(int i = 0; i < pnum; i++) {
    factorial_args[i].numbers_array = array;
    factorial_args[i].begin = active_step * i;
    factorial_args[i].end = (i + 1) * active_step;
  }

  for(uint32_t i = 0; i < pnum; i++) {
    if (pthread_create(&threads[i], NULL, start_factorial, (void*)&factorial_args[i])) {
      perror("\nERROR CREATE THREAD\n");
      return 1;
    }
  }

  for(uint32_t i = 0; i < pnum; i++) {
    pthread_join(threads[i], NULL);
  }
  int result=factorial_answer % mod;
  // pthread_mutex_destroy(&factorial_mutex);
  sem_destroy(&semaphore);
  printf("The factorial of %i equals %i.\n", k, result);
  printf("The factorial %lli by mod %lli is: %lli \n", k, mod, result);

  return 0;
}