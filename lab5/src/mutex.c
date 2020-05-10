/********************************************************
 * An example source module to accompany...
 *
 * "Using POSIX Threads: Programming with Pthreads"
 *     by Brad nichols, Dick Buttlar, Jackie Farrell
 *     O'Reilly & Associates, Inc.
 *  Modified by A.Kostin
 ********************************************************
 * mutex.c
 *
 * Simple multi-threaded example with a mutex lock.
 */
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void do_one_thing(int *);
void do_another_thing(int *);
void do_wrap_up(int);
int common = 0; /* A shared variable for two threads */
int r1 = 0, r2 = 0, r3 = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER; //мьютекс, инициализация динамичекси созданного семафора и установка для него счетчика использования

int main() {
  pthread_t thread1, thread2; //индентификатор потока

  if (pthread_create(&thread1, NULL, (void *)do_one_thing, //создание потока
			  (void *)&common) != 0) { //потоковая фунция, потом вызывающая сторона продолжает выполнять какие-то свои действия параллельно потоковой функции
              //параметры - адрес для хранения идентификатора создаваемого потока, -указатель на потоковую void * функцию, принимающей бестиповый указатель в качестве единственной переменной, -бестиповый указатель, содержащий аргументы потока (так как не аргументов, то НАЛ), -бестиповый указатель атрибутов потока
    perror("pthread_create");
    exit(1);
  }

  if (pthread_create(&thread2, NULL, (void *)do_another_thing,
                     (void *)&common) != 0) {
    perror("pthread_create");
    exit(1);
  }

  if (pthread_join(thread1, NULL) != 0) { //ждем завершения исполнения потока1
  //!!!!чтобы синхронизировать потоки
  //При удачном завершении возвращает 0
    perror("pthread_join");
    exit(1);
  }
  //Несколько потоков не могут ждать завершения одного, иначе ошибка -ESRCH!!!
    if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join");
    exit(1);
  }

  do_wrap_up(common);

  return 0;
}

void do_one_thing(int *pnum_times) { //контроль переходит потоковой функции
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    // pthread_mutex_lock(&mut);
    printf("doing one thing\n");
    work = *pnum_times;
    printf("counter = %d\n", work);
    work++; /* increment, but not write */
    for (k = 0; k < 500000; k++)
      ;                 /* long cycle */
    *pnum_times = work; /* write back */
	// pthread_mutex_unlock(&mut);
  }
}

void do_another_thing(int *pnum_times) {
  int i, j, x;
  unsigned long k;
  int work;
  for (i = 0; i < 50; i++) {
    // pthread_mutex_lock(&mut);
    //Захват мьютекса - ни один другой поток не сможет получить доступ к этим данным или поток блокируется (если мьютекс уже захвачен другим потоком)
    printf("doing another thing\n");
    work = *pnum_times;
    printf("counter = %d\n", work);
    work++; /* increment, but not write */
    for (k = 0; k < 500000; k++)
      ;                 /* long cycle */
    *pnum_times = work; /* write back */
    // pthread_mutex_unlock(&mut);
    //Освобождение мьютекса - ресурс не нужен, чтобы и другие потоки могли получить доступ к этому ресурсу - доступ одному из ожидающих потоков
  }
}

void do_wrap_up(int counter) {
  int total;
  printf("All done, counter = %d\n", counter);
}
