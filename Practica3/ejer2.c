//¿Qué tipo de fichero es mylinkcopy? ¿Cuál es el contenido del fichero mylinkcopy? 
//Se pueden usar los comandos ls, stat, cat y diff para obtener las respuestas a estas preguntas.

//El tipo de archivo mylinkcopy será un enlace simbólico, similar a mylink, ya que el programa de copia copy copiará el enlace simbólico existente y 
//creará uno nuevo que apuntará al mismo destino que el enlace original.
//El contenido seraa la ruta a la que apunta

//CODIGO

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

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
    
    struct stat st;
    
    if(lstat(argv[1],&st)==-1){    
   	 perror("no funciona lstat");
   	 exit(1);
    }

    if(S_ISREG(st.st_mode)){
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
    }
    else if(S_ISLNK(st.st_mode)){
   	 char *nenlace=(char*)malloc(st.st_size + 1);
   	 if(nenlace==NULL){
   		 perror("Error al reservar memoria");
   		 return 1;
   	 }
   	 ssize_t longitud=readlink(argv[1],nenlace,st.st_size);
   	 if(longitud==-1){
   		 perror("ERROR AL LEER LA RUTA");
   		 return 1;
   	 }
   	 nenlace[longitud]='\0';
   	 if(symlink(nenlace,argv[2])==-1){
   		 perror("Error al crear el enlace simbolico");
   		 free(nenlace);
   		 exit(1);
   	 }
   	 free(nenlace);
   	 
    }
    else{
    perror("No va el campo stat");
    return 1;
    }

    
    

    return 0;
}
