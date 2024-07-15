#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void copy(int fdo, int fdd)
{
    char buffer[512];
    size_t bytes_leidos;

    while((bytes_leidos=read(fdo,buffer,512))>0){
   	 if(write(fdd,buffer,bytes_leidos)!= bytes_leidos){
   		 perror("No se puede escribir el archivo");    
   		 exit(1);
   	 }
    }
    
}

int main(int argc, char *argv[])
{
    if(argc != 3 ){
   	 perror("No es el número de argumentos adecuado");
    }

    int entrada;
    int salida;
    if((entrada=open(argv[1],O_RDONLY))==-1){
   	 perror("No se ha podido abrir el archivo de entrada");
    }
    if((salida=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666))==-1){
   	 perror("No se ha podido abrir el archivo de salida");
    }
    
    copy(entrada,salida);
    
    close(entrada);
    close(salida);

    return 0;
}
