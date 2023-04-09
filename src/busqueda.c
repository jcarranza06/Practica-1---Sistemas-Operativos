#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../imports/interfaz.h"
#include "../imports/metodosCSV.h"
#include "../imports/hashTable.h"

//devuelve una peticion con los valores almacenados en el pipe definido por fdSolicitud
struct peticion getPeticion(int fdSolicitud)
{
    struct peticion busqueda;
    char buf[20];

    read(fdSolicitud, buf, sizeof(buf));
    busqueda.origen = atoi(buf);
    read(fdSolicitud, buf, sizeof(buf));
    busqueda.destino = atoi(buf);
    read(fdSolicitud, buf, sizeof(buf));
    busqueda.hora = atoi(buf);

    return busqueda;
}

int main(void)
{
    int fdSolicitud; // por este named pipe se van a recibir las variables de la solicitud
    int fdRespuesta; // por este pipe se va a mandar la respuetsa de la busqueda 
    loadHashTable("../data/tablahash.a"); // cargar tabla hash
    char buf[20];

    fdSolicitud = open("solicitud", O_RDONLY); // se abre el pipe solicitud con permiso de lectura 
    fdRespuesta = open("respuesta", O_WRONLY); // se abre el piep respuesta con permiso de escritura 
    int continuar = 1; 
    while (continuar == 1)// se detiene en caso de haber recibido un numero diferente por el pipe de solicitud
    {

        read(fdSolicitud, buf, sizeof(buf));// se lee la prinmer variable que especifica si continuar con la ejecucion
        continuar = atoi(buf);
        if (continuar == 1)
        {
            struct peticion busqueda = getPeticion(fdSolicitud);

            char modified_identifier[30] = ""; // linea de identificador
            char identifier[8] = "";
            sprintf(identifier, "%04d", busqueda.origen);
            strcat(modified_identifier, identifier);
            sprintf(identifier, "%04d", busqueda.destino); // se concatenan busqueda origen y destino
            strcat(modified_identifier, identifier);

            int index = *search(atoi(modified_identifier));                                               // indice del ultimo elemnto con el origen y destino especificado
            struct fila row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", index); // se busca el indice en el archivo
            while (row.next > 0 && row.hod != busqueda.hora)                                              // mientras se llega al ultimo de la lsita o se encuentra la hora sigue buscando
            {
                row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", row.next); 
            }

            if (row.hod == busqueda.hora)
            { // en caso de que si se encuentre
                // printf("tiempo medio: %f\n", row.mean_travel_time);
                sprintf(buf, "%f", row.mean_travel_time);
                write(fdRespuesta, buf, sizeof(buf));
            }
            else
            { // en caso de no haber en contrado el elemento en la lista enlazada
                //printf("no encontrado :( \n");
                strcpy(buf, "-1");
                write(fdRespuesta, buf, sizeof(buf));
            }
        }
    }

    printf("acaba proceso busqueda\n");

    close(fdSolicitud);
    close(fdRespuesta);
    return 0;
}
