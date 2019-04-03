#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

//Variables MPI
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

/* Función que obtiene los números del archivo. */
int numerosContenidos(FILE *f)
{
	int total = 0;
	int numero = 0;

	fscanf(f, "%d,", &numero);
	while (!feof(f))
	{
		total++;
		fscanf(f, "%d,", &numero);
	}

	rewind(f);

	return total;
}

void obtenerNumeros(FILE *f, int total_numeros, float array_numeros[])
{
	float numero = 0;
	int contador = 0;

	//Contamos los numeros que hay en el fichero
	fscanf(f, "%f,", &numero);
	array_numeros[contador] = numero;
	for (int i = 0; i < total_numeros - 1; i++)
	{
		contador++;
		fscanf(f, "%f,", &numero);
		array_numeros[contador] = numero;
	}
}

int distribuirNumeros(int lado, float array_numeros[], int total_numeros)
{
	int elementos_toroide = lado * lado;
	float elemento=0;
	if (elementos_toroide > total_numeros)
	{
		printf("No hay suficientes números en el archivo datos para un toroide de lado %d\n", lado);
		return EXIT_FAILURE;
	}

	for (unsigned int i = 0; i < elementos_toroide; i++)
	{
		elemento = array_numeros[i];
		MPI_Send(&elemento,1,MPI_FLOAT,i+1,rank,MPI_COMM_WORLD);
		//printf("Se ha mandado el número %f.\n",elemento);
	}

	return EXIT_SUCCESS;
}

int recibirNumero(){
	//printf("Soy el proceso %d.\n",rank);
	float elemento=0;
	MPI_Recv(&elemento,1,MPI_FLOAT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	printf("Hola, soy el proceso %d y mi número es %f.\n",rank,elemento);
	return EXIT_SUCCESS;
	
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	FILE *f;
	int total_numeros = 0;
	int lado = 4;

	if (rank == 0)
	{
		f = abrirArchivo();
		total_numeros = numerosContenidos(f);
		float array_numeros[total_numeros];
		obtenerNumeros(f, total_numeros, &array_numeros);
		distribuirNumeros(lado,&array_numeros,total_numeros);
		
	}else{
		recibirNumero();
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
