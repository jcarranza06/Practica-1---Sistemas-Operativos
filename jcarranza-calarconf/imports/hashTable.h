// include stdio.h stdlib.h stdbool.h
#define TABLE_SIZE 1000000 // Tamaño de la tabla hash

typedef struct HashNode
{
    int key;
    int value;
    struct HashNode *next; // Puntero al siguiente nodo en caso de colisión
} HashNode;

HashNode *hashTable[TABLE_SIZE]; // La tabla hash es un arreglo de punteros a nodos

int hashFunction(int key)
{
    // La función hash simplemente devuelve el residuo de la división del key por el tamaño de la tabla
    unsigned int hash = key;

    hash ^= hash >> 17;
    hash *= 0xed5ad4bb;
    hash ^= hash >> 11;
    hash *= 0xac4c1b51;
    hash ^= hash >> 15;
    hash *= 0x31848bab;
    hash ^= hash >> 14;

    return hash % TABLE_SIZE;
}

void insert(int key, int value)
{
    int index = hashFunction(key);                            // Calcula el índice de la tabla para el key dado
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode)); // Crea un nuevo nodo
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;
    bool continuate = true;
    if (hashTable[index] == NULL)
    {                               // Si el índice de la tabla está vacío
        hashTable[index] = newNode; // Asigna el nuevo nodo como el primer elemento en ese índice
    }
    else
    {                                             // Si el índice de la tabla ya tiene elementos
        HashNode *currentNode = hashTable[index]; // Obtiene el primer nodo en ese índice
        if (currentNode->key == key) // en caso de que la llave esté en la tabla, no se realiza la busqueda en la lista
        {
            continuate = false;
        }
        while (currentNode->next != NULL && continuate)
        { // Avanza hasta el último nodo en la lista
            if (currentNode->key == key)
            {
                continuate = false; // en caso de encontar la llave en la tabla, detiene la busqueda
            }
            else
            {
                currentNode = currentNode->next;
            }
        }
        if (currentNode->key == key)
        {
            currentNode->value = value; // sí se tiene la llave en la tabla solo se actualiza el valor 
        }
        else
        {
            currentNode->next = newNode; // Agrega el nuevo nodo al final de la lista
        }
    }
}

int *search(int key)
{
    int index = hashFunction(key);     // Calcula el índice de la tabla para el key dado
    HashNode *node = hashTable[index]; // Obtiene el primer nodo en ese índice
    if (node != NULL){                  // Se verifica en la lista enlazada si el nodo contiene algún valor o se concluye que está fuera del rango
    while (node != NULL)
    { // Recorre la lista de nodos en ese índice
        if (node->key == key)
        {                          // Si el nodo tiene la key buscada
            return &(node->value); // Retorna un puntero al valor asociado a esa key
        }
        node = node->next; // Avanza al siguiente nodo en la lista
    }
    }
    return NULL; // Retorna NULL si la key no se encontró en la tabla hash 
}

void destroy()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {                                  // Recorre todos los índices de la tabla
        HashNode *node = hashTable[i]; // Obtiene el primer nodo en ese índice
        while (node != NULL)
        {                                    // Recorre la lista de nodos en ese índice
            HashNode *nextNode = node->next; // Obtiene el siguiente nodo en la lista
            free(node);                      // Libera la memoria del nodo actual
            node = nextNode;                 // Avanza al siguiente nodo en la lista
        }
        hashTable[i] = NULL; // Asigna NULL al puntero en ese índice
    }
}

void saveHashTable(char *filename)
{
    FILE *fp = fopen(filename, "wb"); // Abre el archivo para escritura binaria
    if (fp != NULL)
    { // Si el archivo se abrió correctamente
        for (int i = 0; i < TABLE_SIZE; i++)
        {                                  // Recorre todos los índices de la tabla
            HashNode *node = hashTable[i]; // Obtiene el primer nodo en ese índice
            while (node != NULL)
            {                                               // Recorre la lista de nodos en ese índice
                fwrite(&(node->key), sizeof(int), 1, fp);   // Escribe la key en el archivo
                fwrite(&(node->value), sizeof(int), 1, fp); // Escribe el valor en el archivo
                node = node->next;                          // Avanza al siguiente nodo en la lista
            }
        }
        fclose(fp); // Cierra el archivo
    }
}

void loadHashTable(char *filename)
{
    FILE *fp = fopen(filename, "rb"); // Abre el archivo para lectura binaria
    if (fp != NULL)
    {              // Si el archivo se abrió correctamente
        destroy(); // Borra todos los elementos de la tabla hash actual
        int key, value;
        while (fread(&key, sizeof(int), 1, fp) && fread(&value, sizeof(int), 1, fp))
        {                       // Lee una key y un valor del archivo
            insert(key, value); // Agrega el par key-value a la tabla hash
        }
        fclose(fp); // Cierra el archivo
    }
}

/*int main() {
    // Inserta algunos pares key-value en la tabla hash
    /*insert(1, 10);
    insert(2, 20);
    insert(457981, 100);
    insert(232146, 20);
    insert(963258, 10);
    insert(116011602, 35);

    saveHashTable("tablahash.a");*/
/*loadHashTable("tablahash.a");

printf("resultado: %d\n", *search(116011602));

return 0;
}*/
