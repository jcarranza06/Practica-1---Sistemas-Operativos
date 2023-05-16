#include <stdio.h>

int main() {
    
    char *nombreArchivo = "archivo.txt";
    char *modo = "w";// w es para sobrescribir, a+ es para añadir al existente
    FILE *archivo = fopen(nombreArchivo, modo);
    // Si por alguna razón el archivo no fue abierto, salimos inmediatamente
    if (archivo == NULL) {
        printf("Error abriendo archivo %s", nombreArchivo);
        return 1;
    }
    /*
     * Escribir el contenido usando fprintf.
     * */
    fseek(archivo, 0, SEEK_END);// mover puntero al final 
    fprintf(archivo, "Hola mundo, estamos escribiendo en un archivo\n");
    fprintf(archivo, "También podemos formatear números por ejemplows %d\n", 1);
    fprintf(archivo, "¿Saltos de línea? sí\n\n\n");
    fprintf(archivo, "Podemos escribir cualquier cosa como si usáramos printf ;)\n");

    // Al final, cerramos el archivo
    fclose(archivo);
    puts("Contenido escrito correctamente");
    return 0;
}