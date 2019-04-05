#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


int rank, size;
MPI_Status status;

/* Función que abre el archivo datos.dat donde se encuentran los números a distribuir. */
FILE *abrirArchivo()
{
	char nombre_archivo[] = "datos.dat";
	FILE *f;

	f = fopen(nombre_archivo, "r");

	if (f == NULL)
	{
		perror("Error al abrir el archivo datos.dat.\n");
		exit(EXIT_FAILURE);
	}
	return f;
}

/* Función que obtiene el número de números que hay en el archivo. */
int numerosContenidos(FILE *f)
{
	int total = 0;
	float numero = 0;

	fscanf(f, "%f,", &numero);
	while (!feof(f))
	{
		total++;
		fscanf(f, "%f,", &numero);
	}

	rewind(f);

	return total;
}

/* Función que obtiene los números que hay en el archivo */
void obtenerNumeros(FILE *f, float array_numeros[])
{
	float numero = 0;

	//Contamos los numeros que hay en el fichero
	fscanf(f, "%f,", &numero);
	array_numeros[0] = numero;
	for (int i = 0; i < size - 1; i++)
	{
		fscanf(f, "%f,", &numero);
		array_numeros[i + 1] = numero;
	}
}

/* Función que distribuye los números a los procesos de la red */
int distribuirNumeros(int elementos, float array_numeros[], int total_numeros)
{
	float elemento = 0;

	for (unsigned int i = 0; i < elementos; i++)
	{
		elemento = array_numeros[i];
		MPI_Bsend(&elemento, 1, MPI_FLOAT, i + 1, rank, MPI_COMM_WORLD);
	}

	return EXIT_SUCCESS;
}

/* Función que recibe el número correspondiente a cada proceso de la red */
float recibirNumero()
{
	float elemento = 0;
	MPI_Recv(&elemento, 1, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	return elemento;
}