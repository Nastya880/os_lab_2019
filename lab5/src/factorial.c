#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //c99 (for sleep())
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
pthread_mutex_t factorial_mutex;
static int factorial_answer = 1;

typedef struct {
  int* numbers_array;
  int begin;
  int end;
} 
FactorialPart;

void *start_factorial(void *args) {
  FactorialPart *local_part = (FactorialPart*)args;
  int local_answer = 1;
  for (int i = local_part->begin; i < local_part->end; i++)
    local_answer *= local_part->numbers_array[i];
  pthread_mutex_lock(&factorial_mutex);
  factorial_answer *= local_answer;
  pthread_mutex_unlock(&factorial_mutex);
}

int main(int argc, char **argv) {

  uint32_t k = -1;
  uint32_t threads_num = -1;
  uint32_t mod_num = -1;

  /* Разбор параметров командой строки. */
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
            threads_num = atoi(optarg);
            break;
          case 2:
            mod_num = atoi(optarg);
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

  if (k == -1 || threads_num == -1 || mod_num == -1) {
        printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = (int*)malloc(k * sizeof(int));
  for (int i = 0; i < k; i++) {
    array[i] = i + 1;
  }

  int active_step = k > threads_num ? (k / threads_num) : 1;
  pthread_mutex_init(&factorial_mutex, NULL);
  pthread_t threads[threads_num];
  FactorialPart factorial_parts[threads_num];
  for(int i = 0; i < threads_num; i++) {
    factorial_parts[i].numbers_array = array;
    factorial_parts[i].begin = active_step * i;
    factorial_parts[i].end = (i + 1) * active_step;
  }

  for(uint32_t i = 0; i < threads_num; i++) {
    if (pthread_create(&threads[i], NULL, start_factorial, (void*)&factorial_parts[i])) {
      perror("\nERROR CREATE THREAD\n");
      return 1;
    }
  }

  for(uint32_t i = 0; i < threads_num; i++) {
    pthread_join(threads[i], NULL);
  }
  pthread_mutex_destroy(&factorial_mutex);
  printf("The factorial of %i equals %i.\n", k, factorial_answer);
  printf("The factorial of %i with module %i equals %i.\n", k, mod_num, factorial_answer % mod_num);

  return 0;
}