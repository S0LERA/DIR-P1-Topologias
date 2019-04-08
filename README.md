# DIR-P1_Topologias
Asignación de valores desde un archivo a una red toroide e hipercubo y cálculo del valor mayor o menor de la misma a través de MPI.

## Uso
 - Para compilar el programa:
`make compilar`
 - Para lanzar el programa:
   - Número máximo en red Hipercubo:
`make MaximoHipercubo PROCESOS=numero_procesos DIMENSIONES=numero_dimensiones`
   - Número mínimo en red Toroide:
`make MínimoToroide PROCESOS=numero_procesos LADO=lado_toroide`

### Ejemplo:
`make MínimoToroide PROCESOS=10 LADO=3`

### Nota:
Siempre se debe lanzar al menos un proceso más que el número de nodos de la red.

## Versiones
### Versión 0.5
 - Creadas funciones para tratamiento del fichero datos y su contenido.

### Versión 0.8
 - Función distribuirNumeros creada, ligero avance en MPI.

### Versión 1.0
 - Función para calcular el mínimo de una red toroide lista.
   - Funciones comunes a ambas puestas en otro archivo.

### Versión 2.0
 - Función para calcular el máximo de una red hipercubo lista.

### Versión 2.3
 - Documentado README.
 - Makefile creado.
 - Gestión de errores básica.

### Versión 2.5
 - Añadida documentación.