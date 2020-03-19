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
#define MYPORT 3490   
//Este es el puerto por el que va escuhar el 
//Servidor al cliente el mismo puerto debe 
//Estar definido en ambos progrmas

#define MAXDATASIZE 1024
//Tamño del buffer para los mensajes que aceptara el chat 

/* how many pending connections queue will hold */
#define BACKLOG 10
//Conexiones que permite de clientes
void sigchld_handler(int s){
  while(wait(NULL) > 0);
}
/*Esta funcion es para matar procesos zombies
que se generan ya que se generan procesos hijos por cada conexion
al servidor wait bloque al papa si aun no termina
Se espera hasta que el valor devuelto sea -1 que es cuando no hay procesos hijos*/

int main(int argc, char *argv[ ]){
  /* listen on sock_fd, new connection on new_fd */
  int sockfd, new_fd;
  char enviar[MAXDATASIZE]; //Para enviar el primer mensaje
	char enviar2[MAXDATASIZE];  //Para el chat 
  char buf[MAXDATASIZE]; //Para recibir mensajes
	int inicio=0;
	int bytesE; 
	int bytesS; //variable para cada ciclo de lectura y escritura
  /*new_fd es para las nuevas conexiones y sockdf donde se hace la primera
  conexion */
  /* my address information */
  struct sockaddr_in my_addr;
  /*Con sockaddr_in podemos obtener mas informacion 
  de la conexion la familia,el puerto,la ip.
  Aqui estara toda la informacion del servidor*/

  /* connectors address information */
  struct sockaddr_in their_addr;/*Informacion del cliente*/
  int sin_size;
  struct sigaction sa;
  int yes = 1;
  /*Esta esta la llamada al socket */
  /*AF_INET para usar los protocolos de arpanet
  Luego especificamos que queremos usar un socket de flujo 
  Y el protocolo que se puede estblecer a 0 */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    /*Devuleve un valor para el descriptor de archivo si es -1 es un error */
    perror("Server-socket() error lol!");
    exit(1);
  }
  else
    //Se ha realizado correctamente la conexion al servidor 
    printf("Server-socket() sockfd is OK...\n");

  /*Recibe el descriptor ,para indicar que queremos usar opciones de socket
  Mandamos sol_sockter y so_reuseaddr es para indicar que las direcciones 
  proporcionadas por bind puedan ser reutilizadas para que pueda sopartar mas clientes*/
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
    perror("Server-setsockopt() error lol!");
    exit(1);
  }
  else
    printf("Server-setsockopt is OK...\n");

  /* host byte order */
  my_addr.sin_family = AF_INET;
  /*Son atributos de la estructura my_addr que definimos 
  le indicamos la familia que usara*/

  /* EL puerto que usara con la variable MYPORT 
  Htons convierne un host a variable de red */
  my_addr.sin_port = htons(MYPORT);

  /* automatically fill with my IP */
  my_addr.sin_addr.s_addr = INADDR_ANY;
  /*ANADDR_ANY buca automaticamente nuestra IP */


  printf("Server-Using %s and port %d...\n", inet_ntoa(my_addr.sin_addr), MYPORT);

  /* zero the rest of the struct */memset(&(my_addr.sin_zero), '\0', 8);
  /*Para llenar los demas datos de la estructurra usada para el servidro */

  /*Para nombrar nuestro sokcet es decir asociar el socket con un puerto de la 
  maquina, es por eso que tenemos el puntero a la estructura del puerto y a la 
  longitus de la estructura */
  if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
    perror("Server-bind() error");
    exit(1);
  }
  else
    printf("Server-bind() is OK...\n");


  /*sockfd es el descriptor devuelto por socket, esta funcion 
  es usada si estamos esperando que haya conexiones a nuestra maquina
  BACKLOG es el numero de conexiones permitidas que definimos anteriormente*/
  if(listen(sockfd, BACKLOG) == -1){
    perror("Server-listen() error");
    exit(1);
  }
  printf("Server-listen() is OK...Listening...\n");
  /* Usamos signation para hacer una llamada al sistema
  y con sa_handler para hacer la union con nuestra funcion que 
  es la encargada de hacer esperar al padre hasta que termine el hijo 
  Especificamos las señales que deben bloquearse y con la bandera restart indicamos que 
  se pueda reiniciar el proceso  
  A la funcion para verificar que no hay mas procesos mandamos 
  sigchld donde obtine el id del proceso y la direccion de la estructura si devuelve -1 hay un error*/
  sa.sa_handler = sigchld_handler;  /*Sirve para matar procesos zombie*/
  sigemptyset(&sa.sa_mask);         
  sa.sa_flags = SA_RESTART;         
  if(sigaction(SIGCHLD, &sa, NULL) == -1){ 
    perror("Server-sigaction() error");
    exit(1);
  }
  else
    printf("Server-sigaction() is OK...\n");
  /* accept() loop */
  while(1){
    /*Se llama a acept para aceptar las conexion entrantes despues de
    listen sockfd es el descriptr y theiraddr ea para saber que nodo y desde 
    donde se esta conectando. */
    sin_size = sizeof(struct sockaddr_in);
    if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
      perror("Server-accept() error");
      continue;
      /*new_fd son las nuevas conexion y son las que se usaran para 
      send y recv*/
    }
    else
      printf("Server-accept() is OK...\n");
    printf("Server-new socket, new_fd is OK...\n");
    printf("Server: Got connection from %s\n", inet_ntoa(their_addr.sin_addr));
    /*inet_ntoa convierte una cadena que contiene una IP a un entero y devuelve 
    la direccion ip en ordenacion de bytes para redes por eso no se usa 
    /* this is the child process */
    if(!fork()){ 
      //close(sockfd);/*El hijo no lo necesita*/
      /*Codigo para el chat */
      //Envia mensaje de conexion 
      // printf("------SESION INICIADA------\n");
      //printf("CLIENTE CONECTADO\n");
      //printf("El cliente dijo: %s\n",enviar);
      //El servidor espera el primer mensaje
      if((bytesE=recv(new_fd,buf,MAXDATASIZE-1,0))==-1)
        perror("recv()");
      if(strcmp(&buf,"salir")==0){
        break;
      }
      buf[bytesE] = '/0';
      printf("Cliente: %s\n",buf);
 
    //El cliente recibe el mensaje del servidor
      printf("Escribir mensaje: ");
      scanf("%*c%[^\n]",enviar2);
      if((bytesS=send(new_fd,enviar2,MAXDATASIZE,0))==-1)
        perror("send()");
      if(strcmp(enviar2,"salir")==0){
        break;
        }
      /*Codigo para el chat */
    }
    else
    printf("Todo funciono bien");
    close(new_fd);
   
  close(sockfd);
  return 0;
}
