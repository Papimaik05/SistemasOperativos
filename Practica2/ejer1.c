#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
    FILE* file = NULL;


    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    /* Open file */
    if ((file = fopen(argv[1], "rb")) == NULL)
        err(2, "The input file %s could not be opened", argv[1]);

    char *byte;  // Un solo byte para leer y escribir

    /* Read file byte by byte */
    while (fread(byte, sizeof(char),1, file) == 1) {
        /* Write byte to stdout */
        if (fwrite(byte, sizeof(char), 1, stdout) != 1) {
            fclose(file);
            err(3, "fwrite() failed!!");
        }
    }

    fclose(file);
    return 0;
}