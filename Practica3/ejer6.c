#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Modo de uso inadecuado: [-f <Nombre del fichero> -r o -w]\n");
        exit(1);
    }
    
    int opt;
    char* nombre = NULL;
    int flags = O_CREAT | O_TRUNC;
    int rw = 0;

    while ((opt = getopt(argc, argv, "f:rw")) != -1) {
        switch (opt) {
            case 'f':
                nombre = optarg;
                break;
            case 'r':
                flags |= O_RDONLY;
                rw = 1;
                break;
            case 'w':
                flags |= O_WRONLY;
                rw = 1;
                break;
            default:
                fprintf(stderr, "Modo de uso: [-f <Nombre del fichero> -r o -w]\n");
                exit(1);
        }
    }

    if (rw == 0 || nombre == NULL) {
        fprintf(stderr, "Debes proporcionar al menos un modo de escritura/lectura y un nombre de fichero.\n");
        exit(1);
    }

    int fd = open(nombre, flags);
    if (fd == -1) {
        perror("Error al abrir el fichero");
        exit(1);
    }

    char buffer[] = "Ejercicio 6\n";
    ssize_t bytes_written = write(fd, buffer, strlen(buffer);
    if (bytes_written == -1) {
        perror("Error al escribir en el fichero");
        close(fd);
        exit(1);
    }

    char read_buffer[256];
    ssize_t bytes_read = read(fd, read_buffer, sizeof(read_buffer));
    if (bytes_read == -1) {
        perror("Error al leer del fichero");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}



//RESPUESTAS

//Otorga permisos de lectura y escritura a un fichero existente en el sistema y ejecuta el programa apertura con las tres opciones disponibles (-r, -w y -rw):

//Si el archivo tiene permisos de lectura y escritura, el programa apertura.x podrá leer y escribir en el archivo en todos los casos. No se generará ningún error.

//Elimina el permiso de lectura sobre el fichero destino:

//Si eliminas el permiso de lectura sobre el archivo, el programa apertura.x no podrá realizar la operación de lectura (read) en el archivo. Esto resultará en un error durante la ejecución del programa. La función read() devolverá -1, y el programa mostrará un mensaje de error similar a "Error al leer del fichero."

//Elimina el permiso de ejecución del fichero ejecutable apertura.x:

//Si eliminas el permiso de ejecución del archivo ejecutable apertura.x, no podrás ejecutar el programa usando ./apertura.x. Al intentar ejecutarlo, recibirás un mensaje de error similar a "Permiso denegado".