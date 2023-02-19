//Autor: Roy Emmanuel Leon Castillo
//mpicc -o Ejercicio1 Ejercicio1.c
//mpirun -np 4 ./Ejercicio1
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM 10000

int main(int argc, char **argv) {
    int rango, tamano;
    int num_a_buscar;
    int num_encontrados = 0;
    int num_encontrados_local = 0;
    int *matriz;
    MPI_Status estado;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &tamano);

    // Asignar memoria para la matriz
    matriz = (int*) malloc(sizeof(int) * TAM * TAM);

    // Inicializar matriz con valores aleatorios
    for (int i = 0; i < TAM*TAM; i++) {
        matriz[i] = rand() % 100;
    }

    if (rango == 0) {
        // Imprimir matriz desde el proceso 0
        /*printf("Matriz:\n");
        long cont = 0;
        for (long long i = 0; i < TAM; i++) {
            for (long long j = 0; j < TAM; j++) {
                cont++;
                printf("%lld-%d\t", i, matriz[i]);
            }
            printf("\n");
        }
        printf("\n");
        printf("cont %ld\n", cont);*/
        // Obtener número a buscar del usuario
        printf("Ingrese el número a buscar: ");
        fflush(stdout); // asegurarse de que el mensaje se muestre en pantalla
        scanf("%d", &num_a_buscar);

        // Enviar número a todos los procesos
        for (int i = 1; i < tamano; i++) {
            MPI_Send(&num_a_buscar, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Recibir número a buscar del proceso 0
        MPI_Recv(&num_a_buscar, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &estado);
    }

    // Dividir la matriz en fragmentos de tamaño igual para cada proceso
    int tam_fragmento = TAM / tamano;
    int fila_inicial = rango * tam_fragmento;
    int fila_final = fila_inicial + tam_fragmento;

    // Buscar número en las filas asignadas
    for (int i = fila_inicial*TAM; i < fila_final*TAM; i++) {
        if (matriz[i] == num_a_buscar) {
            num_encontrados_local++;
        }
    }

    // Combinar resultados de todos los procesos
    MPI_Reduce(&num_encontrados_local, &num_encontrados, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Imprimir resultado desde el proceso 0
    if (rango == 0) {
        printf("Número de veces que el número %d aparece en la matriz: %d\n", num_a_buscar, num_encontrados);
    }

    // Liberar memoria
    free(matriz);

    MPI_Finalize();
    return 0;
}