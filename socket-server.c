#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int server (int client_socket)
{
 while (1)
 {
  int length;
  char* text;
  /* First, read the length of the text message from the socket. If
  read returns zero, the client closed the connection. */
  if (read (client_socket, &length, sizeof (length)) == 0)
	 /*Verifica el tamaño de bytes leidos en el desciptor de archivo client-socket si es igual a cero se cierra la conexión */
   return 0;
  /* Allocate a buffer to hold the text. */
  text = (char*) malloc (length);
  /*malloc reserva el espacio de memoria para el tamaño de text dinamicamente*/
  /* Read the text itself, and print it. */
  read (client_socket, text, length);
  /*En esta parte read verifica que client_socket sea un descriptor de archivo y en text crea el buffer del tamaño reservado con malloc anteriormente, y lenght es el tamaño de bits que se van a leer */
  printf("Montecillo Sandoval Jose Alejandro\n ");
  printf ("%s\n", text);
  /* If the client sent the message "quit," we're all done. */
  if (!strcmp (text, "quit")) /*strcmp verifica que la cadena enviado por el cliente no sea quit si es quit termina el progrma liberando la memoria si no es quit sigue enviando un 1 para mantener el ciclo while*/
   return 1;
  else
  {
   /* Free the buffer. */
   free (text);
   /*Libera el espacio de memoria reservado por malloc una vez terminada la función */
  }
 }
}

int main (int argc, char* const argv[])
{
 int socket_fd; /*file descriptor for socket*/
 struct sockaddr_un name; /*server socket structure*/
 int client_sent_quit_message;
 const char* const socket_name = argv[1];

 //printf("Montecillo Sandoval Jose Alejandro");
 /* Create the socket. */
 socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
 /* Indicate that this is a server. */
 name.sun_family = AF_LOCAL;
 strcpy (name.sun_path, socket_name);
 bind (socket_fd, (struct sockaddr *)&name, SUN_LEN (&name));
 /* Listen for connections. */
 listen (socket_fd, 5);
 /* Repeatedly accept connections, spinning off one server() to deal
 with each client. Continue until a client sends a “quit” message. */
 do
 {
  struct sockaddr_un client_name;
  socklen_t client_name_len;
  int client_socket_fd;

  /* Accept a connection. */
  client_socket_fd = accept (socket_fd, (struct sockaddr *)&client_name, &client_name_len);
  /* Handle the connection. */
  client_sent_quit_message = server (client_socket_fd);
  /* Close our end of the connection. */
  close (client_socket_fd);
 }
 while (!client_sent_quit_message);

 /* Remove the socket file. */
 close (socket_fd);
 unlink (socket_name);
 return 0;
}
