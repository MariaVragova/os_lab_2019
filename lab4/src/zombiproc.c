#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

 /* Создание дочернего процесса. */

 pid_t pid = fork();

 if (pid > 0) {

  /* Это родительский процесс — делаем паузу 30 секунд */

  sleep(30);

 } else {

  /* Это дочерний процесс — немедленно завершаем работу. */

  exit(0);

 }

 return 0;

}

