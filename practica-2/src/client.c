#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

// se incluyen archivos de imports
#include "../imports/interfaz.h"

#define PORT 3535

char *peticionToString(struct peticion busqueda)
{
    char *str = malloc(10);
    char *buffer = malloc(10);
    sprintf(str, "%04d", busqueda.origen);
    sprintf(buffer, "%04d", busqueda.destino);
    strcat(str, buffer);
    sprintf(buffer, "%02d", busqueda.hora);
    strcat(str, buffer);
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

    char buf[20];
    int seleccion;
    float tiempo_medio;
    struct peticion busqueda;
    busqueda.origen = 0;
    busqueda.destino = 0;
    busqueda.hora = 0;
    struct timeval start, end;
    long seconds, microseconds;

    r = send(clientfd, "1", 11, 0);
    if (r < 0)
    {
        perror("\n Error en send ");
        exit(-1);
    }

    while (seleccion != 5)
    { // se imprime el menú y se pide la opcion del usuario
        seleccion = seleccionarOpcion();
        double time_spent = 0.0;
        switch (seleccion)
        {
        case 1:
            busqueda.origen = ingresarOrigen();
            break;
        case 2:
            busqueda.destino = ingresarDestino();
            break;
        case 3:
            busqueda.hora = ingresarHora();
            break;
        case 4:
            sprintf(buf, "%d", 1);
            // write(fdSolicitud, buf, sizeof(buf));  // se manda un "1" para que el proceso busqueda continue la ejecucion
            //  se mide tambien el tiempo, bein guarda tiempo inical
            gettimeofday(&start, NULL); // se almacena la hora de inicio
            // mandarPeticion(fdSolicitud, busqueda); // se guardan en el pipe el origen. destino y hora
            char *peticionString = peticionToString(busqueda);
            r = send(clientfd, peticionString, 11, 0);
            if (r < 0)
            {
                perror("\n Error en send ");
                exit(-1);
            }

            r = recv(clientfd, buf, 32, 0);
            // read(fdRespuesta, buf, sizeof(buf)); // se detiene a esperar la respuesta
            tiempo_medio = atof(buf);
            if (tiempo_medio > 0)
            {
                printf("El tiempo es: %0.2f\n", tiempo_medio); // se muestra el resultado de la busqueda
            }
            else
            {
                printf("no encontrado\n");
            }
            gettimeofday(&end, NULL); // se almacena la hora de finalizacion
            seconds = end.tv_sec - start.tv_sec;
            microseconds = end.tv_usec - start.tv_usec;
            printf("La busqueda toma %d.%0.6d segundos \n", seconds, microseconds); // se imprime el tiempo que toma la busqueda

            r = send(clientfd, "1", 11, 0);
            if (r < 0)
            {
                perror("\n Error en send ");
                exit(-1);
            }

            break;
        default:
            // caso de salida del programa, se detiene el ciclo
            sprintf(buf, "%d", 0);
            // write(fdSolicitud, buf, sizeof(buf)); // se pasa 0 para la variabla continuar de proceso busqueda, para que este acabe el while y finalice
            printf("SALIENDO\n");
            seleccion = 5;
            break;
        }
        printBusqueda(busqueda);
    }

    buffer[r] = 0;
    printf("\n mensaje recibido: %s \n", buffer);
    close(clientfd);
}