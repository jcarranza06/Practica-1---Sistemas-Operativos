#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "../imports/interfaz.h"
#include "../imports/metodosCSV.h"
#include "../imports/hashTable.h"

#define PORT 3535
#define BACKLOG 2

struct peticion getPeticion(char *str)
{
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

    loadHashTable("../data/tablahash.a"); // cargar tabla hash

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
    int continuate =0;
    while (1)
    {
        clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
        if (clientfd < 0)
        {
            perror("\n Error en accept ");
            exit(-1);
        }
        r = recv(clientfd, buffer, 32, 0);
        //printf("pm: %s \n", buffer);
        continuate = atoi(buffer);
        while (continuate ==1)
        {r = recv(clientfd, buffer, 32, 0); // se recibe mensaje
            buffer[r] = 0;
            printf("\n mensaje recibido: %s \n", buffer);

            struct peticion busqueda = getPeticion(buffer); // se convierte el mensaje en una estructura
            printf("%d %d %d\n", busqueda.origen, busqueda.destino, busqueda.hora);

            struct in_addr clientIp = client.sin_addr;              // se extrae la ip de cliente
            char ipStr[INET_ADDRSTRLEN];                            // se declara string de size INET_ADDRSTRLEN
            inet_ntop(AF_INET, &clientIp, buffer, INET_ADDRSTRLEN); // se pasa de binario a string
            printf("\n mensaje leido de fs: %s \n", buffer);

            char modified_identifier[30] = ""; // linea de identificador
            char identifier[8] = "";
            sprintf(identifier, "%04d", busqueda.origen);
            strcat(modified_identifier, identifier);
            sprintf(identifier, "%04d", busqueda.destino); // se concatenan busqueda origen y destino
            strcat(modified_identifier, identifier);

            char buf[20];

            int index = (search(atoi(modified_identifier))) == NULL ? -1 : *search(atoi(modified_identifier)); // se verifica que la busqueda no haya dado error al no dar un indice dentro del rango                                               // indice del ultimo elemnto con el origen y destino especificado
            struct fila row;
            if (index >= 0)
            {
                row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", index); // se busca el indice en el archivo
                while (row.next > 0 && row.hod != busqueda.hora)                                  // mientras se llega al ultimo de la lsita o se encuentra la hora sigue buscando
                {
                    row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", row.next);
                }
            }
            else
            {
                row.hod = -1;
            }

            if (row.hod == busqueda.hora)
            { // en caso de que si se encuentre
                // printf("tiempo medio: %f\n", row.mean_travel_time);
                sprintf(buf, "%f", row.mean_travel_time);
                // write(fdRespuesta, buf, sizeof(buf));
                r = send(clientfd, buf, sizeof(buf), 0); // se devuelve mensaje
            }
            else
            { // en caso de no haber en contrado el elemento en la lista enlazada
                // printf("no encontrado :( \n");
                strcpy(buf, "-1");
                // write(fdRespuesta, buf, sizeof(buf));
                r = send(clientfd, buf, sizeof(buf), 0); // se devuelve mensaje
            }

            r = send(clientfd, "hola mundo", 10, 0); // se devuelve mensaje
            if (r < 0)
            {
                perror("\n Error en send ");

                exit(-1);
            }
            r = recv(clientfd, buffer, 32, 0); // se recibe mensaje
            continuate = atoi(buffer);
        }

        close(clientfd);
        printf("finalizado cliente\n");
    }
    close(serverfd);
}