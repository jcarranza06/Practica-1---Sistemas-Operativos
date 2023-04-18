// este script permite tomar el archivo y dejar las columnas de todas las filas con el mismo
// tamano con el fin de poder avanzar una cantidad especifica de bytes para saltar a un indice 
// especifico dentro del archivo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//se incluye la tabla hash 
#include "../imports/hashTable.h"

#define MAX_LINE_LENGTH 1000 // Longitud máxima de una línea del archivo

int main()
{
    FILE *input_file = fopen("../data/bogota-cadastral-2019-3-All-HourlyAggregate.csv", "r");   // Abrir archivo CSV en modo lectura
    FILE *output_file = fopen("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", "w"); // Abrir archivo CSV modificado en modo escritura

    if (input_file == NULL || output_file == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, input_file);
    fputs(line, output_file); 
    int i = 0;

    /*insert(963258, 10);
    insert(116011602, 35);
    insert(116011602, 9);
    insert(116011602, 32);
    printf("resultado: %d\n", *search(116011602));*/
    while (fgets(line, MAX_LINE_LENGTH, input_file))
    { // Leer cada línea del archivo
        
        if(i%200000==0){
            printf("%d\n",i);// para ver el proreso en consola
        }
        char *field;
        int field_count = 0;
        char modified_line[MAX_LINE_LENGTH] = ""; // Línea modificada
        char modified_identifier[MAX_LINE_LENGTH] = ""; // linea de identificador 
        char identifier[16] = "";
        // Separar los campos por comas y convertirlos al formato deseado
        for (field = strtok(line, ","); field != NULL; field = strtok(NULL, ","))
        {
            field_count++;
            char modified_field[8] = ""; // Campo modificado
            
            switch (field_count)
            {
            case 1:
            case 2:
                sprintf(modified_field, "%04d", atoi(field)); // Rellenar con 0 a la izquierda hasta 4 dígitos
                sprintf(identifier, "%07d", atoi(field)); // Rellenar con 0el identificador
                strcat(modified_identifier, identifier); // agregado a la linea identificador
                break;
            case 3:
                sprintf(modified_field, "%02d", atoi(field)); // Rellenar con 0 a la izquierda hasta 2 dígitos
                break;
            case 7:
                sprintf(modified_field, "%05.2f", atof(field)); // Rellenar con 0 a la izquierda hasta 5 dígitos, con 2 decimales
                break;
            default:
                sprintf(modified_field, "%07.2f", atof(field)); // Rellenar con 0 a la izquierda hasta 7 dígitos, con 2 decimales
            }

            strcat(modified_line, modified_field); // Agregar campo modificado a la línea modificada
            strcat(modified_line, ",");            // Agregar coma después de cada campo
        }

        // se le asigna una lista enlazada en la que se relaciona cada par de sourceid, dstid
        char modified_field[8] = ""; // Campo modificado
        if(search(atoi(modified_identifier))==NULL){// en caso de que no se haya guardado antes 
            sprintf(modified_field, "%07d", -1); // en caso ed ser el primero que aparece (o el ultimo de la lista) se le asigna -1
        }else{ //en caso de estar ya en la tabla
            sprintf(modified_field, "%07d", *search(atoi(modified_identifier))); // se guarda el anterior indice del csv relacionado a la dupla sourceid,dstid
        }
        insert(atoi(modified_identifier), i);// se inserta en el mapa la llave identificador, indice, identificador: es la concat de dos enteros de 7 digitos ej 00001590000023. indice es el indice respecto a los elem del csv
        strcat(modified_line, modified_field); // Agregar campo modificado a la línea modificada
        strcat(modified_line, ",");

        modified_line[strlen(modified_line) - 1] = '\n'; // Reemplazar última coma con salto de línea
        fputs(modified_line, output_file);               // Escribir línea modificada en el archivo modificado
        i++;
    }

    fclose(input_file); // Cerrar archivos
    fclose(output_file);
    printf("Finalizado aparentemente bien \n");
    return 0;
}