

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>

#define PORT 3535
#define BACKLOG 2

struct peticion {   // en esta estructura se va a guardar la informacion para la busqueda
  int origen;           
  int destino;
  int hora;       
};

struct peticion getPeticion(char *str){
    struct peticion a;
    sscanf(str, "%04d%04d%02d", &a.origen, &a.destino, &a.hora);
    return a;
}

int main()
{
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client;
    socklen_t tamano;
    char buffer[32];

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
    {
        perror("\n Error en socket ");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);

    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

    r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
    if (r < 0)
    {
        perror("\n Error en bind ");
        exit(-1);
    }

    r = listen(serverfd, BACKLOG);
    
    if (r < 0)
    {
        perror("\n Error en listen ");
        exit(-1);
    }
    while (1)
    {
        clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
        printf("accept \n");
        if (clientfd < 0)
        {
            perror("\n Error en accept ");
            exit(-1);
        }
        r = recv(clientfd, buffer, 32, 0);
        buffer[r] = 0;
        printf("\n mensaje recibido: %s \n", buffer);

        struct peticion busqueda = getPeticion(buffer);  // se convierte el mensaje en una estructura 
        printf("%d %d %d\n",busqueda.origen, busqueda.destino, busqueda.hora);

        struct in_addr clientIp = client.sin_addr; // se extrae la ip de cliente
        char ipStr[INET_ADDRSTRLEN]; // se declara string de size INET_ADDRSTRLEN
        inet_ntop(AF_INET, &clientIp, buffer, INET_ADDRSTRLEN); //se pasa de binario a string
        printf("\n mensaje leido de fs: %s \n", buffer);

        r = send(clientfd, "hola mundo", 10, 0);
        if (r < 0)
        {
            perror("\n Error en send ");

            exit(-1);
        }

        close(clientfd);
        printf("finalizado cliente\n");
    }
    close(serverfd);
}