#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "multmodulo.h"

struct Server {
  char ip[255]; // IP-адрес
  int port; // порт
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

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10); // конвертирует строку в беззнаковое целое число (unsigned long long                                                                                                                     integer)  
  if (errno == ERANGE) { // данная строка не входит в обрабатываемый диапазон чисел
    fprintf(stderr, "Out of uint64_t range: %s\n", str); // stderr - поток вывода ошибок
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  int count = 0;
  int* ports = (int*)malloc(sizeof(int));
  char servers[255] = {'\0'}; // TODO: explain why 255 (максимальная длина файла 255)

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here
        if(k<1) {
                    printf("k is a positive number\n");
                    free(ports); // освобождение памяти
                    return 1;
                }
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
                if(mod<1) {
                    printf("mod is a positive number\n");
                     free(ports);
                    return 1;
                }
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg)); // копирование из области памяти optarg в область памяти servers

        FILE* file;
        if ((file = fopen(servers, "r")) != NULL) 
        {
            while (getc(file) != EOF) // end of file — конец файла
            {
                if (count != 0)
                {
                    ports = (int*)realloc(ports, (count + 1) * sizeof(int)); 
                }
                fseek(file, -1, SEEK_CUR); // устанавливание позиции следующей операции ввода/вывода в потоке
                char buff[30];
                fgets(buff, 29, file);
                int read_port = atoi(buff);
                ports[count] = read_port;
                count++;
            }
        }
        else
        {
            printf("Error with opening file \"%s\".\n", servers);
            free(ports);
            return 1;
        }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  unsigned int servers_num = k > count ? count : k; // если k больше, чем кол-во серверов, тогда берем кол-во серверов
  struct Server *to = malloc(sizeof(struct Server) * servers_num);


  // TODO: delete this and parallel work between servers
  for (int i = 0; i < servers_num; i++) {
    to[0].port = ports[i]; // заполнение портов
    memcpy(to[0].ip, "127.0.0.1", sizeof("127.0.0.1"));
  }

  //uint64_t answer = 1;
  int current_begin = 1;
  int end_count;
  bool is_end = false; // флаг
  if (servers_num >= k) { // коэф. для расперделения по серверам
    end_count = 0;
  }
  else {
    if (k % servers_num) {
      end_count = k / servers_num;
    }
    else {
      end_count = k / servers_num - 1;
    }
  }



  // TODO: work continiously, rewrite to make parallel
  for (int i = 0; i < servers_num; i++) {
    struct hostent *hostname = gethostbyname(to[i].ip);
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
      free(ports);
      exit(1);
    }

    struct sockaddr_in server; // указываем параметры сервера
    server.sin_family = AF_INET; // домены Internet
    server.sin_port = htons(to[i].port); // или любой другой порт...
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0); // создание TCP-сокета
    if (sck < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      free(ports)
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) { // установка соединения с сервером
      fprintf(stderr, "Connection failed\n");
      free(ports);
      exit(1);
    }

    // TODO: for one server
    // parallel between servers
    uint64_t begin = current_begin;
    uint64_t end = current_begin + end_count <= k ? current_begin + end_count : k;
    current_begin = end < k ? end + 1 : k;
    
    if (!is_end) {
        char task[sizeof(uint64_t) * 3]; // массив битов "задание"
        memcpy(task, &begin, sizeof(uint64_t)); // начало
        memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
        memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));
        
        
        if (send(sck, task, sizeof(task), 0) < 0) { // запись данных в сокет
          fprintf(stderr, "Send failed\n");
          free(ports);
          exit(1);
      }

      char response[sizeof(uint64_t)]; // массив битов "ответ"
      if (recv(sck, response, sizeof(response), 0) < 0) { // чтение данных из сокета
        fprintf(stderr, "Recieve failed\n");
        free(ports);
        exit(1);
      }

      // TODO: from one server
      // unite results
      uint64_t answer = 0;
      memcpy(&current_answer, response, sizeof(uint64_t));
      answer = MultModulo(current_answer, answer, mod);

    }

    is_end = (end == k);

    close(sck);

  }

  printf("answer: %llu\n", answer);

  free(to);
  free(ports);

  return 0;
}
