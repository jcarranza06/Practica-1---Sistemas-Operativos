struct peticion
{ // en esta estructura se va a guardar la informacion para la busqueda
    char ipStr[INET_ADDRSTRLEN];  
    char* date;
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
    //printf("Ingrese ID del origen: ");
    //scanf("%d", &origen);
    //return origen;
    do {
        printf("Ingrese ID del origen: ");
        scanf("%d", &origen);
        if (origen < 1 || origen > 1160) {
        printf("El valor de origen no es válido.\n");
    }
    } while (origen < 1 || origen > 1160);
    return origen;
  
}

int ingresarDestino(){
    int destino;
    //printf("Ingrese ID del destino: ");
    //scanf("%d", &destino);
    //return destino;
    do {
        printf("Ingrese ID del destino: ");
        scanf("%d", &destino);
        if (destino < 1 || destino > 1160) {
        printf("El valor de destino no es válido.\n");
    }
    } while (destino < 1 || destino > 1160);
    return destino;
}

int ingresarHora(){
    int hora;
    //printf("Ingrese hora del dia: ");
    //scanf("%d", &hora);
    //return hora;
    do {
        printf("Ingrese hora del día: ");
        scanf("%d", &hora);
        if (hora < 1 || hora > 24) {
        printf("El valor de hora no es válido.\n");
    }
    } while (hora < 1 || hora > 24);
    return hora;
}

void printBusqueda(struct peticion busqueda){
    printf("Id Origen: %d\nId Destino: %d\nHora del dia: %d\n", busqueda.origen, busqueda.destino, busqueda.hora);
}

