#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int spawn (char* program, char** arg_list)
{ //Creacion de un proceso hijo
pid_t child_pid;
 child_pid = fork ();
 if (child_pid != 0)
 return child_pid;
 else {
 execvp (program, arg_list); //Espero un argumento terminado en null 
 fprintf (stderr, "an error occurred in execvp\n");
 abort ();
 }
}
int main ()
{
 char* arg_list[] = {
 "ls",
 "-l",
 "/",  //Hace un ls -l Desde la raiz
 NULL  //SI NO TIENE ESTE NULL MANDA EL ERROR
 };

 spawn ("ls", arg_list); //Se manda llamar spawn y espera otro argumento para terminar el programa
 //Un comando de linux. Gracias a execvp podemos mandar a ejecutar ls sin tener que mandarle todo el PATH del comando
 printf("Montecillo Sandoval Jose Alejandro\n");
 printf ("done with main program\n");
 return 0;
}

