#include <stdio.h>

struct peticion {   // en esta estructura se va a guardar la informacion para la busqueda
  int origen;           
  int destino;
  int hora;       
};

int seleccionarOpcion()
{
    int seleccion;
    printf("Bienvenido\n 1. Ingresar origen \n 2. Ingresar destino\n 3. Ingresar hora \n 4. Buscar tiempo de viaje medio \n 5. Salir\n");
    scanf("%d", &seleccion);
    return seleccion;
}

int ingresarOrigen(){
    int origen;
    printf("Ingrese ID del origen: ");
    scanf("%d", &origen);
    return origen;
}

int ingresarDestino(){
    int destino;
    printf("Ingrese ID del destino: ");
    scanf("%d", &destino);
    return destino;
}

int ingresarHora(){
    int hora;
    printf("Ingrese hora del dia: ");
    scanf("%d", &hora);
    return hora;
}

void printBusqueda(struct peticion busqueda){
    printf("Id Origen: %d\nId Destino: %d\nHora del dia: %d\n", busqueda.origen, busqueda.destino, busqueda.hora);
}

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