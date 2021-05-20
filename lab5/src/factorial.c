#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

struct Parameters {

  int num;
  int a;
  int value; 

};

pthread_mutex_t mutex1;

void Factor(struct Parameters*);

void Factor(struct Parameters* param) {
  for (int i = 0; i < param->num; i++) {
    pthread_mutex_lock(&mutex1);
    if (param->a == 0) {
      break;
    } else {
      param->value = param->value * param->a;
      param->a--;
    }
    pthread_mutex_unlock(&mutex1);
  }
}

int main(int argc, char **argv){
    int k = -1;
    int pnum = -1;
    int mod = -1;

    //Получение параметров
    static struct option long_options[] = {{"mod", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 1},
                                      {0, 0, 0, 0}};
    int c;
    while ((c = getopt_long_only(argc, argv, "k:", long_options, NULL)) != -1){
        switch (c) {

        case 0:
            {
                mod = atoi(optarg);
                if(mod<=0) {
                    printf("mod is a positive number\n");
                    return 1;
                }
            }
            break;

        case 1:
            {
                pnum = atoi(optarg);
                if (pnum<=0) {
                    printf("pnum is a positive number\n");
                    return 1;
                }
            }
            break;

        case 'k':
            {
                k=atoi(optarg);
                if(k<0) {
                    printf("k is a positive number\n");
                    return 1;
                }
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
        printf("Usage: %s -k \"num\" --pnum \"num\" --mod \"num\" \n", argv[0]);
        return 1;
    }


  struct Parameters param;

  if (k > 0) {

    if (pnum > k/2) {
      printf("Too much threads \n");
      pnum = k/2;

    }

    pthread_t threads[pnum];
    param.num = k / pnum;
    param.a = k - 1;
    param.value = k;
        //Создание тредов
        for(int i = 0; i < pnum; i++){
            if (pthread_create(&threads[i], NULL, (void*)Factor, (void*)&param)!= 0) {
                printf("Error: cannot create new thread!\n");
                return -1;
            }
        }

        //Ожидание завершения тредов
        for (int i = 0; i < pnum; i++) {
            if(pthread_join(threads[i], NULL) != 0) {
              printf("Error: cannot join thread %d!\n", i);
                return -1;
            }
        }

    } else {
        param.value = 1;
    }
    printf("%d! mod %d = %d\n", k, mod, param.value % mod);

    return 0;
    }
  


  