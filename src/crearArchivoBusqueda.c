// este script permite tomar el archivo y dejar las columnas de todas las filas con el mismo
// tamano con el fin de poder avanzar una cantidad especifica de bytes para saltar a un indice 
// especifico dentro del archivo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    while (fgets(line, MAX_LINE_LENGTH, input_file))
    { // Leer cada línea del archivo
        i++;
        if(i%200000==0){
            printf("%d\n",i);
        }
        char *field;
        int field_count = 0;
        char modified_line[MAX_LINE_LENGTH] = ""; // Línea modificada

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

        modified_line[strlen(modified_line) - 1] = '\n'; // Reemplazar última coma con salto de línea
        fputs(modified_line, output_file);               // Escribir línea modificada en el archivo modificado
    }

    fclose(input_file); // Cerrar archivos
    fclose(output_file);

    return 0;
}