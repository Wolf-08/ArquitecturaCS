#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Write TEXT to the socket given by file descriptor SOCKET_FD. */
void write_text (int socket_fd, const char* text)
{
 /* Write the number of bytes in the string, including
 NUL-termination. */
 int length = strlen (text) + 1; 
 /*Almacena la cadena enviada por el cliente agregando un espacio para el 
    caracter de null */
 write (socket_fd, &length, sizeof (length));
 /*Verifica que socket_fd sea un descriptor de archivo 
   y verifica el tamaño del bufer para el tamaño del mansaje
   asi como los bits que se van a escribir */
 /* Write the string. */
 write (socket_fd, text, length);
 /*Manda el texto y el numero de bits que se van escribir por el cliente 
  haciendo uso del descriptor de archivo*/
}

int main (int argc, char* const argv[])
{
 const char* const socket_name = argv[1];
 const char* const message = argv[2];
 int socket_fd;
 struct sockaddr_un name;
 printf("Montecillo Sandoval Jose Alejandro");
 /* Create the socket. */
 socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
 
 /* Store the server's name in the socket address. */
 name.sun_family = AF_LOCAL;
 strcpy (name.sun_path, socket_name);
 
 /* Connect the socket. */
 connect (socket_fd, (struct sockaddr *)&name, SUN_LEN (&name));
 
 /* Write the text on the command line to the socket. */
 write_text (socket_fd, message);
 close (socket_fd);
 return 0;
}
