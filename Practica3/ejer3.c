#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int opt;
    int N=0;
    int ultimos=0;
    char *entrada;
    while ((opt = getopt(argc, argv, "n:e")) != -1)
    {
   	 switch (opt)
   	 {
   	 case 'n':
   		 N=atoi(optarg);
   		 break;
   	 case 'e':
   		 ultimos=1;
   		 break;
   	 default:
   		 perror("Modo de uso:[-n Nbytes | -e]");
   		 exit(EXIT_FAILURE);
   	 }
    }
    if(optind < argc){
   	 entrada=argv[optind];
    }
    else{
   	perror("No se ha especificado el archivo de entrada");
   	exit(1);
    }
    int origen=open(entrada,O_RDONLY);
    if(origen==-1){
   	 perror("No se puede abrir el archivo");
   	 exit(1);
    }
    
    if(ultimos!=1){
     lseek(origen,N,SEEK_SET);
     char buffer;
     while(read(origen,&buffer,1)>0){
     	write(STDOUT_FILENO,&buffer,1);
     }
    }
    else{
     lseek(origen,-N,SEEK_END);
     char buffer;
     while(read(origen,&buffer,1)>0){
     	write(STDOUT_FILENO,&buffer,1);
     }
    }
    close(origen);
    return 0;
}
