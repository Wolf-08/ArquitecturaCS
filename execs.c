#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(void)
{
 char *args[] = {"ls", NULL};
 if(execvp("ls", args) == -1) {//con execvp no necesita el NULL
  //execve no va a la varianle path por lo tanto no encuentra el comando y no lo ejecuta 
	 perror("execve");
  exit(EXIT_FAILURE);
 }
 puts("shouldn't get here");
 exit(EXIT_SUCCESS);
}
