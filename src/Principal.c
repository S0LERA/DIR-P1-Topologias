//#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>


/* Función que abre el archivo datos.dat donde se encuentran los números a distribuir. */
FILE *abrirArchivo(){
	char nombre_archivo[] = "datos.dat";
	FILE *f;

	f = fopen(nombre_archivo, "r"); // read mode

	if (f == NULL)
	{
		perror("Error al abrir el archivo datos.dat.\n");
		exit(EXIT_FAILURE);
	}
	return f;
}

/* Función que obtiene los numeros del archivo. */

int numerosContenidos(FILE *f){
	int total = 0;
	int numero = 0;

	fscanf (f, "%d,", &numero);
	while (!feof (f)){
		total++;
		fscanf (f, "%d,", &numero);
	}

	rewind(f);

	return total;
}

void obtenerNumeros(FILE *f, int total_numeros, int array_numeros[]){
	int numero = 0;
	int contador = 0;

	//Contamos los numeros que hay en el fichero
	fscanf (f, "%d,", &numero);
	array_numeros[contador] = numero;
	for (int i = 0; i < total_numeros-1; i++) {
		contador++;
		fscanf (f, "%d,", &numero);
		array_numeros[contador] = numero;
	}
}

int distribuirNumeros(int lado, int array_numeros[], int total_numeros){
	if((lado*lado) < total_numeros){
		printf("No hay suficientes numeros en el archivo datos para un toroide de lado %d\n",lado);
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}

int main(int argc, char const *argv[]) {
	FILE *f;
	int total_numeros = 0;
	f = abrirArchivo();
	total_numeros = numerosContenidos(f);
	int array_numeros [total_numeros];
	obtenerNumeros(f,total_numeros,&array_numeros);
	//Numeros obtenidos y guardados en array.
	return EXIT_SUCCESS;
}
