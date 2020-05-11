#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <getopt.h>

struct FactorialArgs
{
   long long int begin;
   long long int end;
};

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
long long int result = 1;
long long int mod = -1;

void Factorial(struct FactorialArgs* num) {
  pthread_mutex_lock(&mut);
  for (int i = num->begin; i < num->end; i++)
  {
      result *= i; 
      result %= mod;
  }
  pthread_mutex_unlock(&mut);
}

int main(int argc, char **argv) {
  long long int k = -1;
  int pnum = -1;

  while (1) {
   // int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            // your code here
            // error handling
            if (k <= 0)
            {
                printf("k must be a positive number");
                return 1;
            }
            break;
          case 1:
            mod = atoi(optarg);
            // your code here
            // error handling
            if(mod <= 0)
            {
                printf("Mod must be a positive number");
                return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if(pnum <= 0)
            {
                printf("Number of threads must be a positive");
                return 1;
            }
            break;
          default:
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

  if (mod == -1 || k == -1 || pnum == -1) {
    printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }
  pthread_t threads[pnum];
  struct FactorialArgs args[pnum];
  int j = 1;
  for (int i = 0; i < pnum; i++)
  {
      if(i != pnum - 1)
      {
          args[i].begin = j;
          args[i].end = j + k/pnum;
      }
      else
      {
         args[i].begin = j;
         args[i].end = k + 1;
      }
      j += k/pnum;
  }
  for (uint32_t i = 0; i < pnum; i++) {
    if (pthread_create(&threads[i], NULL, (void*)Factorial, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }
  
  for (uint32_t i = 0; i < pnum; i++) {
      pthread_join(threads[i], NULL);
  }
  
  printf("The factorial %lli by mod %lli is: %lli \n", k, mod, result);
  return 0;
}