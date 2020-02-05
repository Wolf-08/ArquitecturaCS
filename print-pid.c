#include <stdio.h>
#include <unistd.h> //Declara funciones que son parte del estándar POSIX
//Interfaz del sistema operativo portable (POSIX)
void main ()
{
 printf ("The process ID is %d\n", getpid ());
 printf ("The parent process ID is %d\n", (int) getppid ());
 //sreturn 0;
}
