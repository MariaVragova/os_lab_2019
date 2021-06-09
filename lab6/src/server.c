#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "pthread.h"
#include "multmodulo.h"

struct FactorialArgs {
  uint64_t begin;
  uint64_t end;
  uint64_t mod;
};

/*
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
*/


uint64_t Factorial(const struct FactorialArgs *args) {
  uint64_t ans = 1;

  // TODO: your code here

  int i; 
  for (i = argc->begin; i <= argc->end; i++) {
    ans = MultModulo(ans, i, argc->mod);
  }

  return ans;
}

void *ThreadFactorial(void *args) {
  struct FactorialArgs *fargs = (struct FactorialArgs *)args;
  return (void *)(uint64_t *)Factorial(fargs);
}

int main(int argc, char **argv) {
  int tnum = -1;
  int port = -1;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"port", required_argument, 0, 0},
                                      {"tnum", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        port = atoi(optarg);
        // TODO: your code here
        if (port < 1024 || port > 49151) { // пользовательские порты 1024 - 49151
          printf("Invalid port\n");
          return 1;
        }

        FILE* file;
        bool correct = true;
        if ((file = fopen("port_list.txt", "r")) != NULL) 
        {
            while (getc(file) != EOF) // end of file — конец файла
            {
                fseek(file, -1, SEEK_CUR); // устанавливание позиции следующей операции ввода/вывода в потоке
                char buff[30];
                fgets(buff, 29, file);
                int read_port = atoi(buff);
                if (read_port == port)
                {
                    correct = false; 
                }
            }
        }
        if (!correct)
        {
            printf("Server with this port is already exist.\n");
            return 1;
        }
        break;
      case 1:
        tnum = atoi(optarg);
        // TODO: your code here
        if (tnum <= 0) {
          printf("Number of threads is a positive number\n");
          return 1;
        }
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

  if (port == -1 || tnum == -1) {
    fprintf(stderr, "Using: %s --port 20001 --tnum 4\n", argv[0]);
    return 1;
  }

  int server_fd = socket(AF_INET, SOCK_STREAM, 0); // создание сокета "окно"
  if (server_fd < 0) {
    fprintf(stderr, "Can not create server socket!");
    return 1;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons((uint16_t)port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)); // установили флаги на сокете

  int err = bind(server_fd, (struct sockaddr *)&server, sizeof(server)); // привязали адрес интерфейса и номер порта к сокету
  if (err < 0) {
    fprintf(stderr, "Can not bind to socket!");
    return 1;
  }

  err = listen(server_fd, 128); // слушаем соединения на сокете
  if (err < 0) {
    fprintf(stderr, "Could not listen on socket\n");
    return 1;
  }

  printf("Server listening at %d\n", port);

  FILE* file; // запись в файл для чтения портов клиентом
        if ((file = fopen("port_list.txt", "a")) != NULL) 
        {
            char buff[30];
            sprintf(buff, "%d", port);
            fputs(buff, file);
            fputc('\n', file);
            fclose(file);
        }
        else {
          fprintf(stderr, "Error with opening file \"port_list.txt\"\n");
          return 1;
        }

  while (true) {
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len); // принятие соединения на сокете 

    if (client_fd < 0) {
      fprintf(stderr, "Could not establish new connection\n");
      continue;
    }

    while (true) {
      unsigned int buffer_size = sizeof(uint64_t) * 3;
      char from_client[buffer_size];
      int read = recv(client_fd, from_client, buffer_size, 0); // принимаем данные от клиента

      if (!read)
        break;
      if (read < 0) {
        fprintf(stderr, "Client read failed\n");
        break;
      }
      if (read < buffer_size) {
        fprintf(stderr, "Client send wrong data format\n");
        break;
      }

      pthread_t threads[tnum];

      uint64_t begin = 0;
      uint64_t end = 0;
      uint64_t mod = 0;
      memcpy(&begin, from_client, sizeof(uint64_t));
      memcpy(&end, from_client + sizeof(uint64_t), sizeof(uint64_t));
      memcpy(&mod, from_client + 2 * sizeof(uint64_t), sizeof(uint64_t));

      fprintf(stdout, "Receive: %llu %llu %llu\n", begin, end, mod);

      int iterations = end - begin + 1;
      int needed_threads = iterations < tnum ? iterations : tnum;

      int count = begin;
      int end_count;

      if (tnum >= iterations) { // распределение по потокам
        end_count = 1;
      }
      else {
        if (iterations % tnum) {
          end_count = iterations / tnum;
        }
        else {
          end_count = iterations / tnum -1;
        }
      }

      struct FactorialArgs args[tnum];
      for (uint32_t i = 0; i < needed_threads; i++) {
        // TODO: parallel somehow
        args[i].begin = count;
        args[i].end = count + end_count <= end ? count + end_count : end;
        args[i].mod = mod;

        if (pthread_create(&threads[i], NULL, ThreadFactorial,
                           (void *)&args[i])) {
          printf("Error: pthread_create failed!\n");
          return 1;
        }
        count = count + end_count +1;
      }

      uint64_t total = 1;
      for (uint32_t i = 0; i < needed_threads; i++) {
        uint64_t result = 0;
        pthread_join(threads[i], (void **)&result);
        total = MultModulo(total, result, mod);
      }

      printf("Total: %llu\n", total);

      char buffer[sizeof(total)];
      memcpy(buffer, &total, sizeof(total));
      err = send(client_fd, buffer, sizeof(total), 0); // отправляем сообщения в сокет
      if (err < 0) {
        fprintf(stderr, "Can't send data to client\n");
        break;
      }
    }

    shutdown(client_fd, SHUT_RDWR); // отключение от клиента
    close(client_fd); // закрытие сокета
  }

  return 0;
}
