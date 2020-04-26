#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

//#define _POSIX_SOURCE
#include <signal.h>//!!!!


#include <getopt.h>
//#include "find_min_max.c"
#include "find_min_max.h"
#include "utils.h"
//#include "utils.c"
void killproc(int sig) //function with special signal --timeout10
{
	kill(-1, SIGKILL);
	printf("forks timeout\n");
}

int main(int argc, char **argv) {
  int timeout = -1;
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
  //int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
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
            if(seed<0) //begin intervala
            {
               printf("seed is a positive number\n");
               return 1; 
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if(array_size<0)
            {
               printf("array_size is a positive number\n");
               return 1; 
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if(pnum<0)
            {
               printf("pnum is a positive number\n");
               return 1; 
            }
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            timeout = atoi(optarg); //schitivanie parametra if it have
            if(timeout<0)
            {
               printf("timeout is a positive number\n");
               return 1; 
            }
            break;

          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  
  int fds[2];//Добавление для pipe()-канала межроцессорного взаимодействия
  pipe(fds);
  int fdlen= array_size/pnum;//разделяю массив на части для процессов
  
  for (int i = 0; i < pnum; i++) 
  {
    pid_t child_pid = fork();//новый дочерний
    if (child_pid >= 0) 
    {
      active_child_processes += 1;
      if (child_pid == 0) 
      {
        struct MinMax minmax;//нахожу макисмум и минимум на частях!!! массива 
        if(i != pnum-1) minmax = GetMinMax(array,i*fdlen,(i+1)*fdlen);
        else minmax = GetMinMax(array,i*fdlen,array_size);
        if (with_files) 
        {
          FILE * file;
          file=fopen("file.txt","a");//запись результатов в файл
          if(file==0)
          {
              printf("Not open file\n");
              return 1;
          }else
          {
              fwrite(&minmax,sizeof(struct MinMax),1,file);
              //printf("\nЧастичные!!! значения для процесса %d  Min %d  ||Max  %d",i, minmax.min,minmax.max);
              
          }
          fclose(file);
        } else 
        {
          //printf("\nЧастичные значения для процесса %d  Min %d   ||Max  %d",i, minmax.min,minmax.max);
          write(fds[1],&minmax,sizeof(struct MinMax));
          
          // внесение результатов в поток 
        }
        return 0;
      }

    } else 
    {
      printf("Fork failed!\n");
      return 1;
    }
  }
  if(timeout > 0)
	{
		signal(SIGALRM, killproc); //задание процессом времени, через которое отправим сигнал
		alarm(timeout);
	}

  while (active_child_processes > 0) 
  {
    close(fds[1]);// окончание процессов
    wait(NULL);//ждём потомка - обратная связь
    active_child_processes -= 1;
    
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  struct MinMax Min_Max;
  if (with_files) 
  {
     FILE * file=fopen("file.txt","r");// читаю из файла
     if(file==0)
         {
             printf("Not open file\n");
             return 1;
         }else
         {
             for (int i = 0; i < pnum; i++)
             {
               //fseek(file,sizeof(struct MinMax)*i,0);
               fread(&Min_Max,sizeof(struct MinMax),1,file);
               if (Min_Max.min < min_max.min){min_max.min = Min_Max.min;}
               if (Min_Max.max > min_max.max){min_max.max = Min_Max.max;}
             }
             fclose(file);
         }
  }else
  {
      for (int i = 0; i < pnum; i++)
      {
        read(fds[0],&Min_Max,sizeof(struct MinMax));
        if (Min_Max.min < min_max.min){min_max.min = Min_Max.min;}
        if (Min_Max.max > min_max.max){min_max.max = Min_Max.max;}
      }
      close(fds[0]);// read from pipes
  }
  
  
  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
  free(array);
  printf("\nMin: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  remove("file.txt");
  return 0;
}