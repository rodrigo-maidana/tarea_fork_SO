#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <time.h>

#define ARRAY_SIZE 1000 // Tamaño del arreglo
#define DATO_A_BUSCAR 500  // Dato a buscar codificado en el programa
#define N_CHILDREN 8     // Número de procesos hijos

void shuffle_array(int array[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Número aleatorio entre 0 e i
        // Intercambiar array[i] con el elemento en la posición aleatoria
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void generate_random_array(int array[], int size) {
    // Inicializar el arreglo con valores consecutivos o únicos
    for (int i = 0; i < size; ++i) {
        array[i] = i; // Aquí asumimos que size <= 10000
    }
    
    // Mezclar el arreglo para desordenar los números
    shuffle_array(array, size);
}

int main() {
    srand(time(NULL)); // Para la generación de números aleatorios

    clock_t start_time = clock(); // Tiempo inicial
    double start = ((double) (start_time)) / CLOCKS_PER_SEC;
    printf("%.6f", start);

    int array[ARRAY_SIZE];
    int segment_size = ARRAY_SIZE / N_CHILDREN;
    pid_t pid;

    generate_random_array(array, ARRAY_SIZE);

    for (int i = 0; i < N_CHILDREN; i++) {
        pid = fork();
        if (pid == 0) { // Proceso hijo
            // Cada hijo trabajará con su segmento del arreglo aquí
            int found = 0;
            for (int j = i * segment_size; j < (i + 1) * segment_size && !found; j++) {
                if (array[j] == DATO_A_BUSCAR) {
                    clock_t end_time = clock(); // Tiempo final
                    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
                    printf("Hijo %d encontró el dato %d en el índice %d. Tiempo transcurrido: %.6f segundos.\n", i, DATO_A_BUSCAR, j, cpu_time_used);
                    found = 1;
                }
            }
            if (!found) {
                printf("Hijo %d no encontró el dato %d.\n", i, DATO_A_BUSCAR);
            }
            exit(0); // Finaliza el proceso hijo
        }
    }

    // Espera a que todos los hijos terminen
    while (wait(NULL) > 0);


    clock_t end_time = clock(); // Tiempo final
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    return 0;
}
