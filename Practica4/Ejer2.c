#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    int num;
    char prio;

} Hstruct;
void *thread_usuario(void *arg)
{
    Hstruct *aux= (Hstruct *)arg;
    int numero=aux->num;
    char prio=aux->prio;
    free(arg);
    printf("ID hilo : %lu \n",pthread_self());
    printf("Num hilo : %d \n",numero);
    printf("Prioridad hilo : %c \n",prio);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	 int numhilos = atoi(argv[1]);
    pthread_t hilos[numhilos];

    for(int i=0;i<numhilos;i++){
      Hstruct *args = (Hstruct *)malloc(sizeof(Hstruct));
      args->num=i+1;
      if((i+1) %2 !=0){
   	args->prio='N';
      }
      else{
   	args->prio='P';
      }
      if(pthread_create(&hilos[i],NULL,thread_usuario,(void *)args)!=0){
    	perror("Error al crear el hilo");
    	exit(1);
      }
    }
    
    for(int i=0;i<numhilos;i++){
	  	if(pthread_join(hilos[i],NULL)){
   	 perror("Error al esperar al hilo");
        	exit(1);
     	}
    }

    return 0;
}
