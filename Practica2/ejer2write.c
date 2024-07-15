#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <nombre_archivo> <cadena1> <cadena2> ...\n", argv[0]);
        return 1;
    }

    FILE* archivo = fopen(argv[1], "wb");

    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return 2;
    }

    for (int i = 2; i < argc; i++) {
        const char* cadena = argv[i];
        size_t longitud = strlen(cadena);
	/*DAVID
	size_t longitud = strlen(argv[i]);
	char* ptr = (char*) malloc(longitud+1)
	*/
        // Escribir la cadena junto con el carácter terminador '\0'
        fwrite(cadena, sizeof(char), longitud + 1, archivo);
    }

    fclose(archivo);
    printf("Cadenas escritas en el archivo: %s\n", argv[1]);
    return 0;
}
