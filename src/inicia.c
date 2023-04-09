#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// se incluyen archivos de imports
#include "../imports/interfaz.h"

// en esta funcion se escribe en el named pipe con fd int fdSolicitud los valores de la busqueda
void mandarPeticion(int fdSolicitud, struct peticion busqueda)
{
    char buf[20];

    sprintf(buf, "%d", busqueda.origen);
    write(fdSolicitud, buf, sizeof(buf));
    sprintf(buf, "%d", busqueda.destino);
    write(fdSolicitud, buf, sizeof(buf));
    sprintf(buf, "%d", busqueda.hora);
    write(fdSolicitud, buf, sizeof(buf));
}

int main(void)
{
    pid_t pid;

    pid = fork();//se crea el segundo proceso 

    if (pid == -1) // se verifica que el fork se haya ejecutado correctamente 
    {
        printf("error en fork\n");
        exit(1);
    }
    if (pid == 0)
    {// codigo a ejecutar en el hijo 
    //se inicia un proceso apartir del ejecutable busqueda
        system("./busqueda");
    }
    else
    {//codigo a ejecutar e el padre
        int fdSolicitud; //aca se van amandar los datos de la peticion a el proceso busqueda
        int fdRespuesta; //aca se va a recibir la respuesta de el proceso busqueda
        char buf[20];
        // a continuacion se crean los named pipes con permiso 0666 es decir de lectura y escritura 
        mkfifo("solicitud", 0666); 
        mkfifo("respuesta", 0666); 
        fdSolicitud = open("solicitud", O_WRONLY); //se abre el pipe solicitud con permiso de escritura
        fdRespuesta = open("respuesta", O_RDONLY); // se abre el pipe respuesta con permiso de lectura

        int seleccion;
        float tiempo_medio;
        struct peticion busqueda;
        // loadHashTable("../data/tablahash.a"); // cargar tabla hash
        busqueda.origen = 0;
        busqueda.destino = 0;
        busqueda.hora = 0;
        clock_t begin;
        clock_t end;

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
                write(fdSolicitud, buf, sizeof(buf)); // se manda un "1" para que el proceso busqueda continue la ejecucion
                // se mide tambien el tiempo, bein guarda tiempo inical
                begin = clock();
                // realizarBusqueda(busqueda);
                mandarPeticion(fdSolicitud, busqueda); //se guardan en el pipe el origen. destino y hora

                read(fdRespuesta, buf, sizeof(buf));//se detiene a esperar la respuesta
                tiempo_medio = atof(buf);
                if(tiempo_medio > 0){
                    printf("El tiempo es: %0.2f\n", tiempo_medio); // se muestra el resultado de la busqueda
                }else {
                    printf("no encontrado\n");
                }
                end = clock();                                        // guarda toiempo final
                time_spent += (double)(end - begin) / CLOCKS_PER_SEC; // se pasa tiempo a segundos
                printf("La busqueda toma %f segundos \n", time_spent);
                break;
            default:
                // caso de salida del programa, se detiene el ciclo
                sprintf(buf, "%d", 0);
                write(fdSolicitud, buf, sizeof(buf)); // se pasa 0 para la variabla continuar de proceso busqueda, para que este acabe el while y finalice
                printf("SALIENDO\n");
                seleccion = 5;
                break;
            }
            printBusqueda(busqueda);
        }

        close(fdSolicitud);
        close(fdRespuesta);
    }

    return 0;
}
