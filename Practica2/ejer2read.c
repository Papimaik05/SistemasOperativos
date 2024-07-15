#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char *loadstr(FILE *file)
{
    size_t longitud=0;
    char caracter;

    while(fread(&caracter,sizeof(char),1,file)==1 && caracter != '\0'){

   	 longitud++;
    }

    if (longitud==0){
   	 return NULL;
    }
    fseek(file,-(longitud+1),SEEK_CUR);
    
    char *cadena=(char*)malloc(longitud+1);
    if(cadena==NULL){
   	 perror("Error al reservar memoria");
   	 exit(1);
    }
    fread(cadena,sizeof(char),longitud+1,file);
    return cadena;    
}

int main(int argc, char *argv[])
{
    if(argc != 2){
    fprintf(stderr,"Uso: %s <nombre_archivo> ",argv[0]);
    return 1;
    }
    
    FILE* archivo=fopen(argv[1],"rb");

    if (archivo==NULL){
    perror("No se puede abrir el archivo");
    return 1;
    }

    while(!feof(archivo)){
   	 char* cadena=loadstr(archivo);
   	 if(cadena !=NULL){
   		 printf("%s\n",cadena);
   		 free(cadena);
   	 }

    }
    fclose(archivo);
    return 0;
}
