#include <stdio.h>

//se incluyen archivos de imports
#include "../imports/interfaz.h"

int main()
{
    int seleccion;
    struct peticion busqueda;

    while (seleccion!=5)
    {   // se imprime el menú y se pide la opcion del usuario 
        seleccion = seleccionarOpcion();
        
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
            printf("ZERO\n");
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