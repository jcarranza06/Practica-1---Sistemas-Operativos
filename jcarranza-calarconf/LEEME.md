para usarlo hay que verificar que el archivo "data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv" haya descargado, en caso de que no (el archivo no descarga al clonar el repositorio porque es muy pesado por lo tanto toca descargarlo y agregarlo manualmente a la carpeta), hay que descargarlo en https://drive.google.com/file/d/1Rh44sExYChU85yQrNxoNBla8_JuLasH6/view?usp=sharing este archivo es el resultado de agregar al archivo original una cierta cantidad de caracteres "0" para que cada linea del archivo ocupe la misma cantidad de bytes y así poder recorrerlo más rapidamente, tambien se agrega una columna sin nombre que contiene el indice del anterior elemento con la misma id de source y dest formando una lista enlazada por cada combinacion de id de source y dest del archivo, además la tabla hash termina en -1 siempre. el archivo data/bogota-cadastral-2019-3-All-HourlyAggregate1.csv es generado por "src/crearArchivoBusqueda.c".

Despues es necesario "data/tablahash.c" que almacena el indice del ultimo elemento de cada combinacion id source y iddest de la tabla. este archivo es generado por "src/crearTablaHash.c"

el programa se ejecuta en el proceso generado p1-odProgram.c por el cual inicia el proceso generado por busqueda.c el cual se debe llamar busqueda. El proceso "inicia" ejecuta la interfaz de usuario y tambien manda por medio de named pipes los datos de la busqueda a el porceso "busqueda" el cual realiza la busqueda y devuelve el resultado al porceso "inicia".

el programa en un solo proceso está en "src/programa.c"

el archivo Makefile crea la hashtTable y compila los archivos de proceso e inicia, este se puede inicar desde la linea de comandos con "make", comando el cual llama el objetivo "all" que compila lo necesario
