#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <nombre_archivo>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error abriendo archivo");
        return 1;
    }

    // Lee el dato a buscar y el sub-arreglo del archivo
    int search_for;
    fscanf(fp, "%d", &search_for);
    // Implementa la lógica para leer y buscar en el sub-arreglo aquí

    fclose(fp);
    
    // Lógica de búsqueda y resultado

    return 0;
}
