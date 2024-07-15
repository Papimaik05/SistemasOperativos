#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>

#define NUMITER 3
#define MAX_BUFFER 1024


int finish = 0;

typedef struct {
	int raciones;
	int cocineros_esp;
	int salvajes_esp;
} tAux;

tAux *buffer;
sem_t *sem_salvajes;
sem_t *sem_cocineros;
sem_t *sem_mtx;

void eat(void)
{
    unsigned long id = (unsigned long) getpid();
    printf("Savage %lu eating\n", id);
    //sleep(rand() % 5);
}

int getServingsFromPot(void)
{
    eat();
    buffer->raciones--;
}

void savages(void)
{
    printf("raciones : %d \n" , buffer->raciones);
    sem_wait(sem_mtx);
    while (buffer->raciones == 0) {
   	 printf("Hay %d cocineros esperando \n",buffer->cocineros_esp);
   	 if (buffer->cocineros_esp >0){
   	 sem_post(sem_cocineros);
   	 buffer->cocineros_esp--;
   	 }
   	 buffer->salvajes_esp++;
   	 sem_post(sem_mtx);
   	 sem_wait(sem_salvajes);
   	 sem_wait(sem_mtx);
    }
    getServingsFromPot();
    
    sem_post(sem_mtx);


}

int main(int argc, char *argv[])
{
    //printf("he llegao vivo \n");
    const char *shm_name="/potServings";
    //printf("he llegao vivo antes open \n");
    int fd=shm_open(shm_name,O_RDWR,0666);
    if(fd==-1){
   	 perror("Error shm_open, llama a cocinero");
   	 close(fd);
   	 exit(1);
    }
    //printf("he llegao vivo antes mmap \n");
    buffer=(tAux *)mmap(NULL,sizeof(tAux),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    if(buffer==MAP_FAILED){
   	 perror("Error en el mmap");
   	 munmap(buffer,sizeof(tAux));
   	 close(fd);
   	 exit(1);
    }
    //Ini semaforos
    //printf("he llegao vivo antes semaforo \n");

    sem_cocineros = sem_open("/sem_cocineros",0, 0666, 0);
    if (sem_cocineros == SEM_FAILED) {
  			 perror("Error al abrir semáforo sem_cocineros");
  	 }

    sem_salvajes = sem_open("/sem_salvajes", 0, 0666, 0);
    if (sem_salvajes == SEM_FAILED) {
  			 perror("Error al abrir semáforo sem_salvajes");
  	 }

    sem_mtx = sem_open("/sem_mtx", 0, 0666, 1);
    if (sem_mtx == SEM_FAILED) {
  			 perror("Error al abrir semáforo sem_mtx");
  	 }    

    printf("he llegao vivo antes savage \n");
    savages();
   	 
    //cerrar recurso

    sem_close(sem_cocineros);
    sem_close(sem_salvajes);
    sem_close(sem_mtx);
    munmap(buffer,sizeof(tAux));
    close(fd);
    return 0;
}
