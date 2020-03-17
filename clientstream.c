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
#define MAXDATASIZE 300
/*Esscritura del cliente*/
void write_text (int socket_fd,const char* text)
{
  int length = strlen (text) -1 ;
  write(socket_fd,&length,sizeof(length));

  write(socket_fd,text,length);
}
int main(int argc, char *argv[]){
  int socket_fd, numbytes;
  char bufEnviar[MAXDATASIZE];
  char bufRecibir[MAXDATASIZE];
  struct hostent *he;
  /*Esta es la estructura que recibira informacion
  del nodo remoto */

  /*hostent necesita nombre del nodo 
  alias,tipo de direccion 
  longitus de la direccion
  lista de direcciones del nombre del servidor */

  // connectors address information
  struct sockaddr_in their_addr;
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
  if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
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
  /*Connect se usa igual que en el servidro */
  if(connect(socket_fd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
    perror("connect()");
    exit(1);
  }
  else
    printf("Client-The connect() is OK...\n");

  // if((numbytes = recv(socket_fd, bufEnviar, MAXDATASIZE-1, 0)) == -1){
  //   perror("recv()");
  //   exit(1);
  // }
  // else
  //   printf("Client-The recv() is OK...\n");
  // int enviado=send(socket_fd, "This is a test string from server!\n", 37, 0);

  // printf("enviado= %d\n",enviado);
  // bufEnviar[numbytes] = '\0';
  // printf("Client-Received: %s", bufEnviar);
  printf("Ingrese mensaje al servidor:\n");
  gets(bufEnviar);
  int len=send(socket_fd,bufEnviar,strlen(bufEnviar)+1,0);
  printf("%d bytes enviado ",len);
  int recibido;
    if((recibido= recv(socket_fd,bufRecibir,sizeof(bufRecibir),0)) == -1)
    {
      perror("Error en el mensaje recibido");
      exit(1);
    }
    bufRecibir[recibido]= '\0';
    printf("El mensaje del cliente es %s\n",bufRecibir);
    //close(socket_fd);
    //exit(0);
  
  printf("Client-Closing sockfd\n");
  close(socket_fd);
  return 0;
}
