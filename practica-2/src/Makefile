all: programa

programa: compilar
	echo 'iniciando papi'

crearTablaHash:
	gcc crearHashTable.c -o crearHashTable
	echo 'creando tabla Hash'
	./crearHashTable

compilar: crearTablaHash
	gcc busqueda.c -o busqueda
	gcc inicia.c -o inicia
	echo 'finalizado ya puede iniciar con ./inicia'
