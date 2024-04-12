#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <time.h>

#define ARRAY_SIZE 2000000 // Tamaño del arreglo
#define DATO_A_BUSCAR 1999999  // Dato a buscar codificado en el programa

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
    //shuffle_array(array, size);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N_CHILDREN>\n", argv[0]);
        return 1;
    }

    int N_CHILDREN = atoi(argv[1]);
    srand(time(NULL)); // Para la generación de números aleatorios

    int array[ARRAY_SIZE];
    int segment_size = ARRAY_SIZE / N_CHILDREN;
    pid_t pid;

    generate_random_array(array, ARRAY_SIZE);

    for (int i = 0; i < N_CHILDREN; i++) {
        pid = fork();
        if (pid == 0) { // Proceso hijo
            // Cada hijo trabajará con su segmento del arreglo aquí
            int iter = 0;
            clock_t start_time = clock(); // Tiempo inicial 
            int found = 0;
            for (int j = i * segment_size; j < (i + 1) * segment_size && !found; j++) {
                iter++;
                if (array[j] == DATO_A_BUSCAR) {
                    clock_t end_time = clock(); // Tiempo final
                    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
                    printf("Hijo %d encontró el dato %d en el índice %d. Tiempo transcurrido: %.6f segundos. En %d iteraciones.\n", i, DATO_A_BUSCAR, j, cpu_time_used, iter);
                    found = 1;
                }
            }
            if (!found) {
                //printf("Hijo %d no encontró el dato %d.\n", i, DATO_A_BUSCAR);
            }
            exit(0); // Finaliza el proceso hijo
        }
    }

    // Espera a que todos los hijos terminen
    while (wait(NULL) > 0);

    return 0;
}
