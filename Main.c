#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

#define ARRAY_SIZE 1000

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

int main(int argc, char *argv[]) {
    int array[ARRAY_SIZE];
    int n_children = 4; // Ejemplo con 4 hijos
    int segment_size = ARRAY_SIZE / n_children;
    pid_t pid;
    char filename[50];

    generate_random_array(array, ARRAY_SIZE);

    for (int i = 0; i < n_children; i++) {
        pid = fork();
        if (pid == 0) {
            // Código del hijo
            snprintf(filename, sizeof(filename), "child_%d.txt", i);
            FILE *fp = fopen(filename, "w");
            if (!fp) exit(1); // Si no se puede abrir el archivo, termina el hijo

            // Escribe el dato a buscar y el sub-arreglo en el archivo
            fprintf(fp, "%d\n", 123); // Hardcodeado, ejemplo buscando el número 123
            for (int j = i * segment_size; j < (i + 1) * segment_size; j++) {
                fprintf(fp, "%d ", array[j]);
            }
            fclose(fp);

            // Ejecuta el programa de búsqueda con el nombre del archivo como argumento
            char *args[] = {"./search_program", filename, NULL};
            execv("./search_program", args);
            exit(0); // Terminar este proceso hijo específico
        }
    }

    // Espera a todos los hijos
    while (wait(NULL) > 0);

    // Procesamiento de resultados, etc.

    return 0;
}
