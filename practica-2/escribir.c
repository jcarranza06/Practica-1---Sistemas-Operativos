#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 100

struct peticion
{ // en esta estructura se va a guardar la informacion para la busqueda
    char ipStr[INET_ADDRSTRLEN];  
    char* date;
    int origen;
    int destino;
    int hora;
};

char* peticionToString(struct peticion p) {
    // Buffer para almacenar la cadena resultante
    char* buffer = (char*)malloc(sizeof(char) * MAX_BUFFER_SIZE);

    // Construir la cadena utilizando snprintf
    snprintf(buffer, MAX_BUFFER_SIZE, "%s Cliente %s, Origen: %d, Destino: %d, Hora: %d \n",
             p.date, p.ipStr, p.origen, p.destino, p.hora);

    return buffer;
}

char* getDate() {
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);

    // El formato. Mira más en https://en.cppreference.com/w/c/chrono/strftime
    char formato[] = "%Y%m%d%H%M%S";
    // El tamaño suficiente para almacenar la fecha y hora formateadas
    char* fechaHora = (char*)malloc(sizeof(char) * 70);

    // Intentar formatear
    int bytesEscritos = strftime(fechaHora, 70, formato, &tiempoLocal);
    if (bytesEscritos != 0) {
        // Si no hay error, los bytesEscritos no son 0
        printf("Fecha y hora: %s\n", fechaHora);
        return fechaHora;
    } else {
        printf("Error formateando fecha.\n");
        free(fechaHora);
        return NULL;
    }
}

void appendToFile(const char *filename, const char *string)
{
    FILE *file = fopen(filename, "a"); // Abrir el archivo en modo "append"

    if (file == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    fprintf(file, "%s", string); // Escribir el string en el archivo

    fclose(file); // Cerrar el archivo
}

void saveConsulta(struct peticion consulta){
    char* peticionStr = peticionToString(consulta);
    appendToFile("archivo.txt", peticionStr);
}

int main()
{
    struct peticion p;
    strcpy(p.ipStr, "192.168.0.1");
    p.date = getDate();
    p.origen = 100;
    p.destino = 200;
    p.hora = 15;

    char *nombreArchivo = "archivo.txt";
    char *modo = "w"; // w es para sobrescribir, a+ es para añadir al existente
    FILE *archivo = fopen(nombreArchivo, modo);
    // Si por alguna razón el archivo no fue abierto, salimos inmediatamente

    /*
     * Escribir el contenido usando fprintf.
     * */
    fseek(archivo, 0, SEEK_END); // mover puntero al final
    fprintf(archivo, "Hola mundo, estamos escribiendo en un archivo\n");
    fprintf(archivo, "También podemos formatear números por ejemplows %d\n", 1);
    fprintf(archivo, "¿Saltos de línea? sí\n\n\n");
    fprintf(archivo, "Podemos escribir cualquier cosa como si usáramos printf ;)\n");

    // Al final, cerramos el archivo
    fclose(archivo);
    puts("Contenido escrito correctamente");
    saveConsulta(p);
    saveConsulta(p);
    saveConsulta(p);
    printf("f: %s \n", getDate());
    return 0;
}