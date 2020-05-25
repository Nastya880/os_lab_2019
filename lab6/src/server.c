#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

struct FactorialArgs
{
	uint64_t begin;
	uint64_t end;
	uint64_t mod;
};
uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

uint64_t Factorial(const struct FactorialArgs *args)
{
	uint64_t ans = 1;
	int i = args->begin;
    for(i; i<args->end; i++)
    {
        ans = (ans * i) % args->mod;
    }
	return ans;
}
bool flag= false;
void *ThreadKeyHandler()
{
    getchar();
    flag=true;
}
void *ThreadFactorial(void *args)
{
	struct FactorialArgs *fargs = (struct FactorialArgs *)args;
	return (void *)(uint64_t *)Factorial(fargs);
}

int main(int argc, char **argv)
{
	int tnum = -1;
	int port = -1;

	while (!flag)
	{
		int current_optind = optind ? optind : 1;

		static struct option options[] = { { "port", required_argument, 0, 0 },
		{ "tnum", required_argument, 0, 0 },
		{ 0, 0, 0, 0 } };

		int option_index = 0;
		int c = getopt_long(argc, argv, "", options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 0:
		{
			switch (option_index)
			{
			case 0:
				port = atoi(optarg);
				printf("port = %d\n", port);
				break;
			case 1:
				tnum = atoi(optarg);
				printf("tnum = %d\n", tnum);
				break;
			default:
				printf("Index %d is out of options\n", option_index);
			}
		} break;

		case '?':
			printf("Unknown argument\n");
			break;
		default:
			fprintf(stderr, "getopt returned character code 0%o?\n", c);
		}
	}

	if (port == -1 || tnum == -1)
	{
		fprintf(stderr, "Using: %s --port 20001 --tnum 4\n", argv[0]);
		return 1;
	}

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		fprintf(stderr, "Server: Can not create server socket!");
		return 1;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons((uint16_t)port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt_val = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

	int err = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
	if (err < 0)
	{
		fprintf(stderr, "Server: Can not bind to socket!");
		return 1;
	}

	err = listen(server_fd, 128);
	if (err < 0)
	{
		fprintf(stderr, "Could not listen on socket\n");
		return 1;
	}

	printf("Server listening at %d\n", port);

	while (true)
	{
		struct sockaddr_in client;
		socklen_t client_len = sizeof(client);
		int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);

		if (client_fd < 0)
		{
			fprintf(stderr, "Server: Could not establish new connection\n");
			continue;
		}

		while (!flag)
		{
			unsigned int buffer_size = sizeof(uint64_t) * 3;
			char from_client[buffer_size];
			int read = recv(client_fd, from_client, buffer_size, 0);

			if (!read)
				break;
			if (read < 0)
			{
				fprintf(stderr, "Server: Client read failed\n");
				break;
			}
			if (read < buffer_size)
			{
				fprintf(stderr, "Server: Client send wrong data format\n");
				break;
			}

			pthread_t threads[tnum];

			uint64_t begin = 0;
			uint64_t end = 0;
			uint64_t mod = 0;
			memcpy(&begin, from_client, sizeof(uint64_t));
			memcpy(&end, from_client + sizeof(uint64_t), sizeof(uint64_t));
			memcpy(&mod, from_client + 2 * sizeof(uint64_t), sizeof(uint64_t));

			fprintf(stdout, "Server: Receive: %llu %llu %llu\n", begin, end, mod);

			struct FactorialArgs args[tnum];
			for (uint32_t i = 0; i < tnum; i++) 
			{
			    if(i == 0)
			    {
			        args[i].begin = begin;
				    args[i].end = begin + ((end-begin)/tnum + 1);
				    args[i].mod = mod;
				    //printf("XXXXXXXb = %d e = %d m = %d",  args[i].begin,  args[i].end,  args[i].mod);
			    }
			    else
			    {
			        args[i].begin = args[i-1].end;
				    args[i].end = args[i].begin + ((end-begin)/tnum + 1);
				    args[i].mod = mod;
				    if(args[i].end>end)
				    {
				        args[i].end = end;
				    }
				    //printf("XXXXXXXb = %d e = %d m = %d",  args[i].begin,  args[i].end,  args[i].mod);
			    }

				if (pthread_create(&(threads[i]), NULL, (void *)ThreadFactorial, (void *)&(args[i])))
				{
					printf("Server: Error: pthread_create failed!\n");
					return 1;
				}
			}

			uint64_t total = 1;
			for (uint32_t i = 0; i < tnum; i++)
			{
				uint64_t result = 0;
				pthread_join(threads[i], (void *)&result);
				total = MultModulo(total, result, mod);
			}

			printf("Server: Total: %llu\n", total);

			char buffer[sizeof(total)];
			memcpy(buffer, &total, sizeof(total));
			err = send(client_fd, buffer, sizeof(total), 0);
			if (err < 0)
			{
				fprintf(stderr, "Server: Can't send data to client\n");
				break;
			}
		}

		shutdown(client_fd, SHUT_RDWR);
		close(client_fd);
	}

	return 0;
}
