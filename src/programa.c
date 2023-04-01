#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 

//se incluyen archivos de imports
#include "../imports/interfaz.h"
#include "../imports/metodosCSV.h"
#include "../imports/hashTable.h"

void realizarBusqueda(struct peticion busqueda){
    //se extrae el identificador(key) de la tabla y se guarda en mdified identifier
    char modified_identifier[30] = ""; // linea de identificador
    char identifier[8] = "";
    sprintf(identifier, "%04d", busqueda.origen);
    strcat(modified_identifier, identifier);
    sprintf(identifier, "%04d", busqueda.destino); // se concatenan busqueda origen y destino
    strcat(modified_identifier, identifier);

    int index = *search(atoi(modified_identifier)); // indice del ultimo elemnto con el origen y destino especificado
    struct fila row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", index);// se busca el indice en el archivo 
    while (row.next > 0 && row.hod!=busqueda.hora)// mientras se llega al ultimo de la lsita o se encuentra la hora sigue buscando
    {
        row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", row.next); 
    }

    if(row.hod == busqueda.hora){// en caso de que si se encuentre
        printf("tiempo medio: %f\n", row.mean_travel_time);
    }else{
        printf("no encontrado :( \n");
    }
}

int main()
{
    int seleccion;
    struct peticion busqueda;
    loadHashTable("../data/tablahash.a"); // cargar tabla hash
    busqueda.origen=0;
    busqueda.destino=0;
    busqueda.hora=0;
    clock_t begin;
    clock_t end;
    //ejemplo del uso de metodos.csv
    /*struct fila row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", 1000000);
    printFila(row);*/

    //ejemplo de guarar tabla hash
    //insert(963258, 10);
    /*insert(116011602, 35);
    int i ;
    for(i=0; i< 1000000; i++){
        insert(i, i);
    }
    saveHashTable("tablahash.a");*/

    //ejemplo de cargar tabla hash
    /*loadHashTable("../data/tablahash.a");
    printf("resultado: %d\n", *search(5820929));*/

    while (seleccion!=5)
    {   // se imprime el menú y se pide la opcion del usuario 
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
            begin = clock();
            realizarBusqueda(busqueda);
            end = clock();
            time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
            printf("La busqueda toma %f segundos \n", time_spent);
            break;
        default: 
            // caso de salida del programa, se detiene el ciclo
            printf("SALIENDO\n");
            seleccion=5;
            break;
        }
        printBusqueda(busqueda);
    }

    return 0;
}