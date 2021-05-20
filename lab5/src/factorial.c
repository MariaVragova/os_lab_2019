#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>



int main(int argc, char **argv) {

  int k = -1;
  int pnum = -1;
  int mod = -1;
 

  while (1) {

    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int c = 0;
    while ((c = getopt_long_only(argc, argv, "k:", long_options, NULL)) != -1){
        switch (c) {

      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);

            if (k <= 0) {
             printf("k is a positive number\n");
             return 1;
            }

            break;
          case 1:
            pnum = atoi(optarg);

            if (pnum <= 0) {
             printf("pnum is a positive number\n");
             return 1;
            }

            break;
          case 2:
            mod = atoi(optarg);

            if (mod <= 0) {
             printf("mod is a positive number\n");
             return 1;
            }

            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
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

  pthread_t threads[threads_num];