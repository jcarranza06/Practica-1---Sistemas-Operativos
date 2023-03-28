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

