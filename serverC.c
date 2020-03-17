#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

/* the port users will be connecting to */
//en el cliente se define el mismo puerto
#define MYPORT 3490

/* how many pending connections queue will hold */
#define BACKLOG 10

void sigchld_handler(int s){
  while(wait(NULL) > 0);
 //La función wait(NULL) pone en espera al proceso padre hasta que alguno
//de sus hijos haya terminado, si el proceso padre no tiene hojos envía in -1
//wait limpia los procesos zombie
}

int main(int argc, char *argv[ ]){
   int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */
	//descriptores de sockets


  //declaración de estructuras para cliente y servidor 
  /* my address information */
  struct sockaddr_in my_addr;
	//estructura para para guardar informacion que tiene que ver con ips
  /* connectors address information */
  struct sockaddr_in their_addr;
  int sin_size;
  struct sigaction sa;
  int yes = 1;

  //nueves declaraciones 
	int server_len, client_len; //tamaños de las estructuras
	char c[1024];
	char ch[1024];
	int inicio=0;
	int bufs; //tamaño del tamaño
	int ciclo=1; //variable para cada ciclo de lectura y escritura
	char cs[1024];	
//////////////////////////////////////////////////////////////////////////
	
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("Server-socket() error lol!");
    exit(1);
  }
  else
    printf("Server-socket() sockfd is OK...\n");

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
    perror("Server-setsockopt() error lol!");
    exit(1);

	//fución setcockopt -
  }
  else
    printf("Server-setsockopt is OK...\n");
//se llenan los datos 

  /* host byte order */
  my_addr.sin_family = AF_INET;

  /* short, network byte order */
  my_addr.sin_port = htons(MYPORT); //puerto

  /* automatically fill with my IP */
  my_addr.sin_addr.s_addr = INADDR_ANY;    //IP 
  printf("Server-Using %s and port %d...\n", inet_ntoa(my_addr.sin_addr), MYPORT);

  /* zero the rest of the struct */
  memset(&(my_addr.sin_zero), '\0', 8);

 //longitud server
	server_len= sizeof(my_addr);

  //avisamos al sistema operativo la creacion del socket y esperamos resultados
  if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
    perror("Server-bind() error");
    exit(1);
  }
  else
    printf("Server-bind() is OK...\n");



  if(listen(sockfd, BACKLOG) == -1){
    perror("Server-listen() error");
    exit(1);
  }
  else{
  printf("Server-listen() is OK...Listening...\n");
  listen(sockfd,5);
  /* clean all the dead processes */
  }

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sa, NULL) == -1){
    perror("Server-sigaction() error");
    exit(1);
  }
  else
    printf("Server-sigaction() is OK...\n");

  /* accept() loop */
  while(ciclo){

    sin_size = sizeof(struct sockaddr_in);
	//se hace la conexión
    if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
      perror("Server-accept() error");
      continue;
    }
    else
      printf("Server-accept() is OK...\n");
    printf("Server-new socket, new_fd is OK...\n");
    printf("Server: Got connection from %s\n", inet_ntoa(their_addr.sin_addr));
	if(inicio==0){
		strcpy(ch, "servidor conectado");
		send(new_fd, ch, 1204, 0);
		inicio=1;
	}
	recv(sockfd, cs, 1024, 0);
	printf("Cliente: %s --",cs);

	printf("Mensaje:\n ");
	scanf("%*c%[^\n]",c);
	send(new_fd, c, 1024,0);



//cerrar sockets
    /* this is the child process */
    if(!fork()){ //acepta multiples connecciones
      /* child doesnt need the listener */
      close(sockfd);
      if(send(new_fd, "This is a test string from server!\n", 37, 0) == -1)
        perror("Server-send() error lol xd!");
      close(new_fd);
      exit(0);
    }
    else
      printf("Server-send is OK...!\n");

    /* parent doesnt need this */
    close(new_fd);
    printf("Server-new socket, new_fd closed successfully...\n");
  }
  return 0;
}
