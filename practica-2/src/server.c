/*
se compila con gcc server.c -o server -pthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include "../imports/interfaz.h"
#include "../imports/metodosCSV.h"
#include "../imports/hashTable.h"

#define PORT 3535
#define BACKLOG 2

int serverfd, r, *rh0, opt = 1;
//int clientfd;
struct sockaddr_in server, client;
socklen_t tamano;


struct datos
{
    int fd;
};
// Definición de la estructura de un nodo
typedef struct Node
{
    pthread_t thread;
    struct Node *next;
} Node;

// Definición de la estructura del stack
typedef struct Stack
{
    Node *top; // Puntero al nodo en la cima del stack
} Stack;

// Función para crear un nuevo nodo
Node *createNode(pthread_t thread)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->thread = thread;
    newNode->next = NULL;
    return newNode;
}

// Función para crear un nuevo stack
Stack *createStack()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

// Función para verificar si el stack está vacío
int isEmpty(Stack *stack)
{
    return stack->top == NULL;
}

// Función para agregar un elemento al stack (push)
void push(Stack *stack, pthread_t thread)
{
    Node *newNode = createNode(thread);
    newNode->next = stack->top;
    stack->top = newNode;
    //printf("Thread %ld agregado al stack\n", thread);
}

// Función para extraer un elemento del stack (pop)
pthread_t pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        printf("Error: Stack vacío\n");
        pthread_t nullThread;
        return nullThread;
    }
    Node *temp = stack->top;
    pthread_t thread = temp->thread;
    stack->top = stack->top->next;
    free(temp);
    return thread;
}


struct peticion getPeticion(char *str)
{
    struct peticion a;
    sscanf(str, "%04d%04d%02d", &a.origen, &a.destino, &a.hora);
    return a;
}

void confConection()
{
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
    {
        perror("\n Error en socket ");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);

    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

    r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
    if (r < 0)
    {
        perror("\n Error en bind ");
        exit(-1);
    }

    r = listen(serverfd, BACKLOG);
    if (r < 0)
    {
        perror("\n Error en listen ");
        exit(-1);
    }
}

void realizarBusqueda(char buffer[], int clientfd)
{

    struct peticion busqueda = getPeticion(buffer); // se convierte el mensaje en una estructura
    printf("%d %d %d\n", busqueda.origen, busqueda.destino, busqueda.hora);

    struct in_addr clientIp = client.sin_addr;              // se extrae la ip de cliente
    char ipStr[INET_ADDRSTRLEN];                            // se declara string de size INET_ADDRSTRLEN
    inet_ntop(AF_INET, &clientIp, buffer, INET_ADDRSTRLEN); // se pasa de binario a string
    printf("\n mensaje leido de fs: %s \n", buffer);

    char modified_identifier[30] = ""; // linea de identificador
    char identifier[8] = "";
    sprintf(identifier, "%04d", busqueda.origen);
    strcat(modified_identifier, identifier);
    sprintf(identifier, "%04d", busqueda.destino); // se concatenan busqueda origen y destino
    strcat(modified_identifier, identifier);

    char buf[20];

    int index = (search(atoi(modified_identifier))) == NULL ? -1 : *search(atoi(modified_identifier)); // se verifica que la busqueda no haya dado error al no dar un indice dentro del rango                                               // indice del ultimo elemnto con el origen y destino especificado
    struct fila row;
    if (index >= 0)
    {
        row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", index); // se busca el indice en el archivo
        while (row.next > 0 && row.hod != busqueda.hora)                                  // mientras se llega al ultimo de la lsita o se encuentra la hora sigue buscando
        {
            row = leerCSV("../data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv", row.next);
        }
    }
    else
    {
        row.hod = -1;
    }

    if (row.hod == busqueda.hora)
    { // en caso de que si se encuentre
        // printf("tiempo medio: %f\n", row.mean_travel_time);
        sprintf(buf, "%f", row.mean_travel_time);
        // write(fdRespuesta, buf, sizeof(buf));
        printf("se devuelve: %s \n", buf);
        r = send(clientfd, buf, sizeof(buf), 0); // se devuelve mensaje
    }
    else
    { // en caso de no haber en contrado el elemento en la lista enlazada
        // printf("no encontrado :( \n");
        strcpy(buf, "-1");
        // write(fdRespuesta, buf, sizeof(buf));
        printf("se devuelve: %s \n", buf);
        r = send(clientfd, buf, sizeof(buf), 0); // se devuelve mensaje
    }
}

void *manageClient(void *datos)
{
    struct datos *datosh;
    datosh = datos;
    int clientfd = datosh->fd;
    char buffer[32];
    int continuate = 0;
    r = recv(clientfd, buffer, 32, 0);
    continuate = atoi(buffer);
    while (continuate == 1)
    {
        r = recv(clientfd, buffer, 32, 0); // se recibe mensaje
        buffer[r] = 0;
        printf("\n mensaje recibido: %s \n", buffer);

        realizarBusqueda(buffer, clientfd);

        r = recv(clientfd, buffer, 32, 0); // se recibe mensaje
        continuate = atoi(buffer);
    }

    close(clientfd); // se finaliza el cliente
    printf("finalizado cliente\n");
}

int main()
{
    Stack *stack = createStack(); // Crear un nuevo stack
    int clientfd;

    loadHashTable("../data/tablahash.a"); // cargar tabla hash
    confConection();                      // se configura la coneccion
    int i=0;
    int seleccion=0;
    while (seleccion != 1)
    {

        struct datos datosh;
        pthread_t hilo;

        datosh.fd = accept(serverfd, (struct sockaddr *)&client, &tamano);
        if (datosh.fd < 0)
        {
            perror("\n Error en accept ");
            exit(-1);
        }
        int ra;
        ra = pthread_create(&hilo, NULL, (void *)manageClient, (void *)&datosh);
        if (ra == 0)
        {
            printf("creado th %d\n",i);
            push(stack, hilo);// se agrega hilo creado al stack
        }
        if (ra != 0)
        {
            perror("\n-->pthread_create error: ");
            exit(-1);
        }
        i++;
        printf("oprima 0 para continuar recibiendo clientes, 1 para detenerse y esperar a que acaben:\n ");
        scanf("%d", &seleccion);
    }
    close(serverfd);

    while (!isEmpty(stack)) // se devuelve por todos los hilos creados esperando a que finalicen 
    {
        pthread_t hilo = pop(stack);
        r = pthread_join(hilo, (void **)&rh0);
        if (r != 0)
        {
            perror("\n-->pthread_join error: ");
            exit(-1);
        }
    }
    return 0;
}