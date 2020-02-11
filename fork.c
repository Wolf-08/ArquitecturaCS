#include <stdio.h>
#include <unistd.h>

void main() //Proceso inicial 
{
	pid_t id_hijo;
	//pid se puede representar como un enetero
	printf("1: El id del proceso main es %d\n", getpid() );
	id_hijo = fork();
	/* 
	proceso padre id_hijo =! 0
	proceso hijo id_hijo = 0
	*/
	printf("2: Este printf saldra duplicado\n");
	printf("3: El id del proceso hijo %d\n", id_hijo);
}

