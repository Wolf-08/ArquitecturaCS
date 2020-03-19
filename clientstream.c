#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

// the port client will be connecting to
#define PORT 3490
// max number of bytes we can get at once
#define MAXDATASIZE 1024
/*Esscritura del cliente*/
int main(int argc, char *argv[]){
  int sockfd, numbytes;
  struct hostent *he;
	//declaraciones nuevas
	char buf[MAXDATASIZE]; //Para recibir mensajes 
  char enviar[MAXDATASIZE], recibir[MAXDATASIZE];
  // connectors address information
  struct sockaddr_in their_addr;
  
  /*Esta es la estructura que recibira informacion
  del nodo remoto */

  /*hostent necesita nombre del nodo 
  alias,tipo de direccion 
  longitus de la direccion
  lista de direcciones del nombre del servidor */

  // connectors address information
  //struct sockaddr_in their_addr;
  /*Sobre la informacion de cliente 
  y servidor  */
  // if no command line argument supplied
  if(argc != 2){
    fprintf(stderr, "Client-Usage: %s host_servidor\n", argv[0]);
    // just exit
    exit(1);
  }

  // get the host info
  if((he=gethostbyname(argv[1])) == NULL){
    perror("gethostbyname()");
    exit(1);
    /*Obtiene el nombre del ssrvidor la ip*/
  }
  else
    printf("Client-The remote host is: %s\n", argv[1]);
  /*Se crea el socket al igual que en el servidor */
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket()");
    exit(1);
  }
  else 
    printf("Client-The socket() sockfd is OK...\n");

  /*Se llenan los demas datos de la estructura 
  para poder hacer la conexion */

  // host byte order
  their_addr.sin_family = AF_INET;
  // short, network byte order
  printf("Server-Using %s and port %d...\n", argv[1], PORT);
  their_addr.sin_port = htons(PORT);/*htons de nuevo para obtener
  el puerto */
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  /*SE agrega la ip dler servidor a la lista 
  de direcciones de la estructura he*/

  // zero the rest of the struct
  memset(&(their_addr.sin_zero), '\0', 8);
  //bzero((char *)&their_addr,sizeof(their_addr));

  /*Connect se usa igual que en el servidro */
  if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
    perror("connect()");
    exit(1);
  }
  else
    printf("Client-The connect() is OK...\n");
  
  //Recibir mensaje de bienvenida
  // if ((numbytes=recv(sockfd,buf,MAXDATASIZE-1,0)) == -1){
  //   perror("recv()");
  //   exit(1);
  //   }
  // printf("%s\n",buf);
  while(1){

    printf("Escribir mensaje: ");
    scanf("%*c%[^\n]",enviar);
    send(sockfd,enviar,MAXDATASIZE,0);
    if(strcmp(enviar,"salir")==0){
      break;
    }
 //El cliente recibe el mensaje del servidor
    
    numbytes=recv(sockfd,buf,MAXDATASIZE-1,0);
    if(strcmp(buf,"salir")==0){
      break;
    }
    buf[numbytes]='/0';
    printf("Servidor: %s\n",buf);
    }
  close(sockfd);
  return 0;
}
