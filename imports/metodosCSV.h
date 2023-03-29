// se deben incluir las liubrerias stdio.h string.h stdlib.h
#define BUFFER_SIZE (int)1024;

struct fila
{ // Structure declaration
    int sourceid;
    int dstid;
    int hod;
    float mean_travel_time;
    float standard_deviation_travel_time;
    float geometric_mean_travel_time;
    float geometric_standard_deviation_travel_time;
};

void printFila(struct fila row){
    printf("elem: %d, %d, %d, %f, %f, %f, %f\n", row.sourceid, row.dstid, row.hod, row.mean_travel_time, row.standard_deviation_travel_time, row.geometric_mean_travel_time, row.geometric_standard_deviation_travel_time);
}

// string (string) lleva el buffer del archivo csv para extraer cada elemento 
struct fila crearFila(char string[])
{   // el separador por defecto es ','
    // para cada campo se hasce el casting y se asigna
    struct fila row;
    row.sourceid = atoi(strtok(string, ","));
    row.dstid = atoi(strtok(NULL, ","));
    row.hod = atoi(strtok(NULL, ","));
    row.mean_travel_time = atof(strtok(NULL, ","));
    row.standard_deviation_travel_time = atof(strtok(NULL, ","));
    row.geometric_mean_travel_time = atof(strtok(NULL, ","));
    row.geometric_standard_deviation_travel_time = atof(strtok(NULL, ","));

    return row;
}

// (string) direccionArchivo contiene la direccion del archivo .csv
// (int) index lleva el indice dentro del archivo que se busca
struct fila leerCSV(char direccionArchivo[], int index)
{
    FILE *pInput;
    char buffer[1024];
    int size;

    pInput = fopen(direccionArchivo, "r");
    // get para los elementos de la cabecera, tambien se mueve el apuntador a el primer elemento (fila 0) de las lista
    fgets(buffer, sizeof(buffer), pInput);
    //se mueve el apuntador a la direccion donde se encuentra el primer elemento de la fila [index] esto tiene en cuenta que cada fila ocupe 44 bytes
    fseek(pInput,index*44,SEEK_CUR);
    //se obtiene guarda en buffer el elemento a buscar
    fgets(buffer, sizeof(buffer), pInput);
    // se crea la estructura
    struct fila row = crearFila(buffer);
    return row;
}