 #include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <sys/wait.h>
#include <unistd.h>
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

int main()
{
	const pid_t pid = fork();//create 2 processa

	if (pid < 0) {
		printf("fork failed\n");
		return -1;
	}

	if (pid) {
		int status;

		waitpid(pid, &status, 0);// в одном из процессов вызываем функцию waitpid и дожидаюсь пока изменится состояние процесса pid, который был возвращен форк, и получаем из статуса код возврата 
		printf("Return code: %d\n", WEXITSTATUS(status));
	} else {
		for (char c = 'n'; tolower(c) != 'y'; scanf("%c", &c))//дочерний процесс ждет пока не скажу ему умереть и потом возвращает код 42
			printf("Kill me? (y/n)\n");
		exit(42);
	}
	return 0;
}