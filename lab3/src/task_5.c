#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include<sys/wait.h>
#include <unistd.h> 

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }
  pid_t pid = fork();
  if (pid == -1) {
      printf("Fork failed!\n");
      return 1;
  }
  else if (pid == 0) {
      execv("/projects/os_lab_2019/lab3/src/sequential_min_max", argv);
  }
  else {
      wait(&pid);
      printf("Successful\n");
  }
  }