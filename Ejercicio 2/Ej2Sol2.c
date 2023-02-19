//Autor: Roy Emmanuel Leon Castillo
//mpicc -o Ej2Sol2 Ej2Sol2.c
//mpirun -np 4 ./Ej2Sol2
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define N 1000

int main(int argc, char **argv) {
    int size, rank;
    int matrix[N][N];
    int diagonal[N];
    int suma_diagonal = 0;
    int max_diagonal = 0;
    int i, j;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Generar la matriz en el proceso 0
    if (rank == 0) {
        srand(time(NULL));
        printf("Matriz generada:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                matrix[i][j] = rand() % 100;
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }

        // Enviar los elementos de la diagonal a los procesos esclavos
        for (i = 0; i < N; i++) {
            diagonal[i] = matrix[i][i];
            MPI_Send(&diagonal[i], 1, MPI_INT, i % (size - 1) + 1, 0, MPI_COMM_WORLD);
        }
    } else {
        // Recibir el elemento de la diagonal correspondiente al proceso esclavo
        MPI_Recv(&diagonal[rank - 1], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Calcular la suma de la diagonal
    for (i = 0; i < N; i++) {
        suma_diagonal += diagonal[i];
    }

    // Calcular el valor máximo de la diagonal
    for (i = 0; i < N; i++) {
        if (diagonal[i] > max_diagonal) {
            max_diagonal = diagonal[i];
        }
    }

    // Enviar los resultados al proceso maestro
    MPI_Send(&suma_diagonal, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    MPI_Send(&max_diagonal, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);

    // Recibir los resultados del proceso maestro
    if (rank == 0) {
        int suma_diagonal1 = 0, suma_diagonal2 = 0;
        int max_diagonal1 = 0, max_diagonal2 = 0;
        int max_total = 0, diagonal_mas_grande = 0;
        int opcion;

        // Menú de opciones
        printf("\nElige una opción:\n");
        printf("1. Calcular la suma de las diagonales\n");
        printf("2. Calcular el valor más alto de las diagonales\n");
        printf("3. ¿Cuál diagonal tiene el valor más alto?\n");
        printf("4. ¿Cuál diagonal es más grande?\n");
        printf("Ingresa el número de la opción que deseas realizar: ");
        fflush(stdout); // asegurarse de que el mensaje se muestre en pantalla
        scanf("%d", &opcion);

        // Ejecutar la función correspondiente a la opción elegida
        switch (opcion) {
            case 1:
            printf("\n--------------------------------------------\n");
                // Calcular la suma de las diagonales por separado
                for (i = 0; i < N; i++) {
                    suma_diagonal1 += matrix[i][i];
                    suma_diagonal2 += matrix[i][N-i-1];
                }
                printf("La suma de la diagonal 1 es %d\n", suma_diagonal1);
                printf("La suma de la diagonal 2 es %d\n", suma_diagonal2);
                printf("La suma total de las diagonales es %d\n", suma_diagonal);
                break;
            case 2:
                // Calcular el valor máximo de las diagonales por separado
                /*
                for (i = 0; i < N; i++) {
                    if (i % 2 == 0) {
                        printf("--- %d\n",i);
                        if (diagonal[i] > max_diagonal1) {
                            max_diagonal1 = diagonal[i];
                        }
                    } else {
                        printf("+++ %d\n",i);
                        if (diagonal[i] > max_diagonal2) {
                            max_diagonal2 = diagonal[i];
                        }
                    }   
                }
                */
                printf("\n--------------------------------------------\n");
                printf("Diagonal principal: ");
                for (i = 0; i < N; i++) {
                    printf("%d ", diagonal[i]);
                    if (diagonal[i] > max_diagonal1) {
                        max_diagonal1 = diagonal[i];
                    }
                }
                printf("\n");

                printf("Diagonal secundaria: ");
                for (i = 0; i < N; i++) {
                    printf("%d ", matrix[i][N-i-1]);
                    if (matrix[i][N-i-1] > max_diagonal2) {
                        max_diagonal2 = matrix[i][N-i-1];
                    }
                }
                printf("\n");
                printf("El valor máximo de la diagonal 1 es %d\n", max_diagonal1);
                printf("El valor máximo de la diagonal 2 es %d\n", max_diagonal2);
                // Recibir los resultados de los procesos esclavos
                for (i = 1; i < size; i++) {
                    int suma, max;
                    MPI_Recv(&suma, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    MPI_Recv(&max, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    suma_diagonal += suma;
                    if (max > max_diagonal) {
                        max_diagonal = max;
                    }
                }

                // Imprimir resultados finales
                
                //printf("El valor máximo de las diagonales es %d\n", max_diagonal);
                break;
            case 3:
                printf("\n--------------------------------------------\n");
                // Determinar cuál diagonal tiene el valor máximo
                for (i = 0; i < N; i++) {
                    if (diagonal[i] > max_diagonal1) {
                        max_diagonal1 = diagonal[i];
                    }
                }
                for (i = 0; i < N; i++) {
                    if (matrix[i][N-i-1] > max_diagonal2) {
                        max_diagonal2 = matrix[i][N-i-1];
                    }
                }
                if (max_diagonal1 > max_diagonal2) {
                    diagonal_mas_grande = 1;
                } else if (max_diagonal1 < max_diagonal2) {
                    diagonal_mas_grande = 2;
                }
                if (diagonal_mas_grande == 0) {
                    printf("Ambas diagonales tienen el mismo valor máximo de %d\n", max_diagonal1);
                } else {
                    printf("La diagonal %d tiene el valor máximo de %d\n", diagonal_mas_grande, (diagonal_mas_grande == 1) ? max_diagonal1 : max_diagonal2);
                }
                break;
            case 4:
                printf("\n--------------------------------------------\n");
                // Determinar cuál diagonal es más grande
                for (i = 0; i < N; i++) {
                    suma_diagonal1 += matrix[i][i];
                    suma_diagonal2 += matrix[i][N - i - 1];
                }
                if (suma_diagonal1 > suma_diagonal2) {
                    diagonal_mas_grande = 1;
                } else if (suma_diagonal1 < suma_diagonal2) {
                    diagonal_mas_grande = 2;
                }
                printf("La suma de la diagonal principal es: %d\n", suma_diagonal1);
                printf("La suma de la diagonal secundaria es: %d\n", suma_diagonal2);
                if (diagonal_mas_grande == 0) {
                    printf("Ambas diagonales tienen el mismo tamaño de %d\n", suma_diagonal1);
                } else {
                    printf("La diagonal %d es más grande con un tamaño de %d\n", diagonal_mas_grande, (diagonal_mas_grande == 1) ? suma_diagonal1 : suma_diagonal2);
                }
                break;
            default:
                printf("\n--------------------------------------------\n");
                printf("Opción inválida\n");
        }
    }

    MPI_Finalize();
    return 0;
}