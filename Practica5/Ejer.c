#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

int turno_actual_vip=0;
int turno_actual=0;
int dispensador_turnos=0;
int dispensador_turnos_vip=0;
int clientes_dentro=0;
int vip_waiting=0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t espera_turno = PTHREAD_COND_INITIALIZER;
pthread_cond_t espera_turno_vip = PTHREAD_COND_INITIALIZER;

void enter_normal_client(int id)
{
   int turno;
   pthread_mutex_lock(&m);
   turno=dispensador_turnos++;//coger turno
   while(turno!=turno_actual || clientes_dentro == CAPACITY || vip_waiting>0){
	 printf("Normal Client %d is waiting to enter the disco\n", id);
    pthread_cond_wait(&espera_turno,&m);
   }
   turno_actual++; //avanzar turno
   clientes_dentro++;
   pthread_cond_broadcast(&espera_turno); //avisar a todos
   printf("Normal Client %d enter the disco. Numero de clientes : %d \n", id,clientes_dentro);
   pthread_mutex_unlock(&m);
}

void enter_vip_client(int id)
{
   int turnov;
   pthread_mutex_lock(&m);
   turnov=dispensador_turnos_vip++;//coger turno vip
   vip_waiting++;
   while(turnov!=turno_actual_vip || clientes_dentro == CAPACITY){
	 printf("Vip Client %d is waiting to enter the disco\n", id);
    pthread_cond_wait(&espera_turno_vip,&m);
   }
   turno_actual_vip++; //avanzar turno vip
   vip_waiting--;
   clientes_dentro++;
   if(vip_waiting>0){
   	pthread_cond_broadcast(&espera_turno_vip); //avisar a todos los vip
   }
   else{
    pthread_cond_broadcast(&espera_turno); //avisar a todos los normales
   }
   printf("Vip Client %d enter the disco. Numero de clientes : %d \n", id,clientes_dentro);
   pthread_mutex_unlock(&m);
}
void dance(int id, int isvip)
{
    printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
    sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{
   pthread_mutex_lock(&m);
   clientes_dentro--;
   if(vip_waiting>0){
    pthread_cond_broadcast(&espera_turno_vip); //avisar a todos los vip
   }
   else{
    pthread_cond_broadcast(&espera_turno); //avisar a todos los normales
   }
   if(isvip==1){
  	printf("Vip Client %d exit . Numero de clientes : %d \n", id,clientes_dentro);
   }
   else{
  	printf("Normal Client %d exit. Numero de clientes : %d \n", id,clientes_dentro);
   }
   
   pthread_mutex_unlock(&m);
}

void *client(void *arg)
{
  	int *aux= (int *)arg;
  	int isvip=aux[1];
  	int id=aux[0];
    
  	if(isvip==1){
    enter_vip_client(id);
  	}
  	else{
    enter_normal_client(id);
  	}
  	dance(id,isvip);
  	disco_exit(id,isvip);
  	free(arg);
  	//pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc != 2){
    perror("FORMATO: ./disco ARCHIVO");
    exit(1);
    }
    FILE* arch;
    arch=fopen(argv[1],"r");
    if(arch==NULL){
    perror("No se ha podido abrir el archivo");
    exit(1);
    }
    int clientes,vip;
    fscanf(arch,"%d",&clientes);
    printf("%d",clientes);
    pthread_t hilos[clientes];

    for(int i=0;i<clientes;i++){
    fscanf(arch,"%d",&vip);
	 int *args = (int *)malloc(sizeof(int)*2);
    args[0]=i;
    args[1]=vip;
    if(pthread_create(&hilos[i],NULL,client,(void *)args)!=0){
    	perror("Error al crear el hilo");
    	exit(1);
      }
    
    }
    for(int i=0;i<clientes;i++){
	  	if(pthread_join(hilos[i],NULL)){
   	 perror("Error al esperar al hilo");
        	exit(1);
     	}
    }
    fclose(arch);    
    return 0;
}

