#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3535

struct peticion {   // en esta estructura se va a guardar la informacion para la busqueda
  int origen;           
  int destino;
  int hora;       
};

char* peticionToString(struct peticion busqueda){
    char *str = malloc(10);
    char *buffer = malloc(10);
    sprintf(str, "%04d", busqueda.origen);
    sprintf(buffer, "%04d", busqueda.destino);
    strcat(str,buffer);
    sprintf(buffer, "%02d", busqueda.hora);
    strcat(str,buffer);
    return str;
}

int main(int argc, char *argv[])
{

    int clientfd, r;
    struct sockaddr_in client;
    struct hostent *he;
    char buffer[32];

    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd < 0)
    {

        perror("\n Error en socket ");
        exit(-1);
    }

    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    inet_aton(argv[1], &client.sin_addr);

    r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if (r < 0)
    {

        perror("\n Error en connect ");
        exit(-1);
    }

    struct peticion a;
    a.origen = 152;
    a.destino = 879;
    a.hora = 5;
    char *peticionString = peticionToString(a);

    r = send(clientfd, peticionString, 11, 0);
    if (r < 0)
    {
        perror("\n Error en send ");

        exit(-1);
    }

    r = recv(clientfd, buffer, 32, 0);
    buffer[r] = 0;
    printf("\n mensaje recibido: %s \n", buffer);
    close(clientfd);
}