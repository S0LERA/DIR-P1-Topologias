/*************************************************************************************
* Proyecto:                Primera Práctica de Diseño de Infraestructura de red
*
* Nombre del programa:     MaximoHipercubo.c
*
* Autor:                   Pablo Rodríguez Solera
*
* Fecha de creación:       13/03/2019
*
* Proposito:               - Econtrar número máximo entre los distribuidos en los
*							 nodos de una red Hipercubo.
*************************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "funciones.c"
#include <math.h>

#define HIPERCUBO 0

//Variables MPI
int rank, size;
MPI_Status status;

float maximo(float a, float b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}

/* Función que calcula los vecinos de cada elemento de la red */
void calcularVecinos(int vecinos[], int dimensiones)
{
    for (unsigned int i = 0; i < dimensiones; i++)
    {
        vecinos[i] = (rank ^ ((int)pow(2, i)));
    }
}

/* Función que calcula el número máximo de la red */
float calcularMaximo(int vecinos[], int dimensiones, float numero)
{
    float numero_vecino = 0;
    for (unsigned int i = 0; i < dimensiones; i++)
    {
        MPI_Bsend(&numero, 1, MPI_FLOAT, vecinos[i], rank, MPI_COMM_WORLD);
        MPI_Recv(&numero_vecino, 1, MPI_FLOAT, vecinos[i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        numero = maximo(numero, numero_vecino);
    }

    return numero;
}

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float numero = 0;
    int dimensiones = atoi(argv[1]);
    int distribuidor = size - 1;

    if (argc != 2)
    {
        if (rank == 0)
        {
            fprintf(stderr, "\x1b[31mError en la entrada de argumentos del programa.\n \x1b[0m");
            fprintf(stderr, "Uso: mpirun -n *nº procesos* MaximoHipercubo.\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if (size < (int)pow(2,dimensiones)+1)
    {
        if (rank == 0)
        {
            fprintf(stderr, "\x1b[31mError: No se han lanzado los suficientes procesos para un hipercubo de dimensiones %d, se deben lanzar %d procesos.\n \x1b[0m", dimensiones, (int)pow(2,dimensiones)+1);
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if (rank == distribuidor)
    {
        FILE *f;
        int total_numeros = 0;
        int elementos_hipercubo = (int)pow(2, dimensiones);

        f = abrirArchivo();
        total_numeros = numerosContenidos(f);
        float array_numeros[total_numeros];
        if (elementos_hipercubo > total_numeros)
        {
            printf("\x1b[31mError: No hay suficientes números en el archivo datos para un hipercubo de dimensiones %d\n \x1b[0m", dimensiones);
            MPI_Finalize();
            return EXIT_FAILURE;
        }
        obtenerNumeros(f, &array_numeros);
        distribuirNumeros(elementos_hipercubo, &array_numeros, HIPERCUBO);
        MPI_Recv(&numero, 1, MPI_FLOAT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("El número máximo de la red es %f\n.", numero);
    }
    else
    {
        int vecinos[dimensiones];

        numero = recibirNumero(distribuidor);
        calcularVecinos(&vecinos, dimensiones);
        numero = calcularMaximo(&vecinos, dimensiones, numero);
        if (rank == 1)
        {
            MPI_Bsend(&numero, 1, MPI_FLOAT, distribuidor, rank, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
