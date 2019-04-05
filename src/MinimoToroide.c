#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "funciones.c"

#DEFINE TOROIDE 1

#define NORTE 0
#define SUR 1
#define ESTE 2
#define OESTE 3

//Variables MPI
int rank, size;
MPI_Status status;

/* Función que calcula el mínimo de 2 números */
float minimo(float a, float b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}
/* Función que calcula los vecinos de cada elemento de la red */
void calcularVecinos(int vecinos[], int lado)
{
	int c = (rank - 1) % lado;
	int f = (rank - 1) / lado;
	if (c == 0)
	{
		vecinos[OESTE] = rank + (lado - 1);
	}
	else
	{
		vecinos[OESTE] = rank - 1;
	}
	if (f == 0)
	{
		vecinos[SUR] = rank + ((lado - 1) * lado);
	}
	else
	{
		vecinos[SUR] = rank - lado;
	}
	if (c == lado - 1)
	{
		vecinos[ESTE] = rank - (lado - 1);
	}
	else
	{
		vecinos[ESTE] = rank + 1;
	}
	if (f == lado - 1)
	{
		vecinos[NORTE] = rank - (f * lado);
	}
	else
	{
		vecinos[NORTE] = rank + lado;
	}
}

/* Función que calcula el número mínimo de la red */
float calcularMinimo(int vecinos[], int lado, float numero)
{
	float numero_vecino = 0;

	for (int i = 1; i < lado; i++)
	{
		MPI_Bsend(&numero, 1, MPI_FLOAT, vecinos[SUR], rank, MPI_COMM_WORLD);
		MPI_Recv(&numero_vecino, 1, MPI_FLOAT, vecinos[NORTE], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		numero = minimo(numero, numero_vecino);
	}
	for (int i = 1; i < lado; i++)
	{
		MPI_Bsend(&numero, 1, MPI_FLOAT, vecinos[ESTE], rank, MPI_COMM_WORLD);
		MPI_Recv(&numero_vecino, 1, MPI_FLOAT, vecinos[OESTE], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		numero = minimo(numero, numero_vecino);
	}

	return numero;
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	float numero = 0;
	int lado = 3;
	int distribuidor = 0;

	if (rank == distribuidor)
	{
		FILE *f;
		int total_numeros = 0;
		int elementos_toroide = lado * lado;

		f = abrirArchivo();
		total_numeros = numerosContenidos(f);
		float array_numeros[total_numeros];
		if (elementos_toroide > total_numeros)
		{
			printf("No hay suficientes números en el archivo datos para un toroide de lado %d\n", lado);
			return EXIT_FAILURE;
		}
		obtenerNumeros(f, &array_numeros);
		distribuirNumeros(elementos_toroide, &array_numeros, TOROIDE);
		MPI_Recv(&numero, 1, MPI_FLOAT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("El número mínimo de la red es %f\n.", numero);
	}
	else
	{
		int vecinos[4];

		numero = recibirNumero(distribuidor);
		calcularVecinos(&vecinos, lado);
		numero = calcularMinimo(&vecinos, lado, numero);
		if (rank == 1)
		{
			MPI_Bsend(&numero, 1, MPI_FLOAT, distribuidor, rank, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
