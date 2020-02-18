#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  char *newargv[] = { "./myecho", "hola", "mundo", "buen", "dia",  NULL };

  if( argc != 2 )
  {
    fprintf( stderr, "La sintaxis es: %s <archivo-a-ejecutar>\n", argv[0] );
    exit( EXIT_FAILURE );
  }

  execve( argv[1], newargv, NULL );
  perror("execve");
  exit( EXIT_FAILURE );
}
