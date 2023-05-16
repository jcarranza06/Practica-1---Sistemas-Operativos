#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// se incluyen archivos de imports
#include "../imports/hashTable.h"

#define MAX_LINE_LENGTH 1000 // Longitud máxima de una línea del archivo

int main()
{
    FILE *input_file = fopen("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", "r"); // Abrir archivo CSV en modo lectura

    if (input_file == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, input_file);
    int i = 0;
    while (fgets(line, MAX_LINE_LENGTH, input_file))
    { // Leer cada línea del archivo
        if (i % 200000 == 0)
        {
            printf("%d\n", i); // para ver el proreso en consola
        }
        char *field;
        int field_count = 0;
        char modified_identifier[MAX_LINE_LENGTH] = ""; // linea de identificador
        char identifier[8] = "";
        // Separar los campos por comas y convertirlos al formato deseado
        for (field = strtok(line, ","); field != NULL && field_count < 2; field = strtok(NULL, ","))
        {
            field_count++;
            sprintf(identifier, "%04d", atoi(field)); // Rellenar con 0 el identificador
            strcat(modified_identifier, identifier);  // agregado a la linea identificador
            //printf("%s\n",modified_identifier);
        }
        //printf("%d, %d\n", atoi(modified_identifier), i);
        insert(atoi(modified_identifier), i); // se inserta en el mapa la llave identificador, indice, identificador: es la concat de dos enteros de 7 digitos ej 00001590000023. indice es el indice respecto a los elem del csv
        i++;
    }
    fclose(input_file); // Cerrar archivos
    saveHashTable("../data/tablahash.a");
    printf("finalizado con aparente exito\n");
    return 0;
}
