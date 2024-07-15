 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define M 10
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

void putServingsInPot(int servings) {
	buffer->raciones = servings;
	printf("Rellenando el caldero con 10 raciones por %d \n", getpid());
}

void cook(void) {
	while(!finish){
    	sem_wait(sem_mtx);
    	printf("raciones: %d \n",buffer->raciones);
    	while (buffer->raciones > 0 && !=finish) {
   	 printf("Cocinero %d esperando \n", getpid());
   	 buffer->cocineros_esp++;
   	 sem_post(sem_mtx);
   	 sem_wait(sem_cocineros);
   	 sem_wait(sem_mtx);
    	}   
    	putServingsInPot(M);  
    
    	printf("Salvajes esperandov1: %d \n", buffer->salvajes_esp);
    	while (buffer->salvajes_esp > 0 && !=finish) {
   	 printf("Salvajes esperandov2: %d\n", buffer->salvajes_esp);
   	 sem_post(sem_salvajes);
   	 buffer->salvajes_esp--;
    	}
   	 	sem_post(sem_mtx);
	}
}

void handler(int signo) {
	finish = 1;
	printf("Has parado mediante señales \n");
}

int main(int argc, char *argv[]) {
	const char *shm_name = "/potServings";
	int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	if (fd == -1) {
    	perror("Error shm_open");
    	exit(1);
	}
	if (ftruncate(fd, sizeof(tAux)) == -1) {
    	perror("ERROR EN TRUNCATE");
    	shm_unlink(shm_name);
    	close(fd);
    	exit(1);
	}
	buffer = (tAux *)mmap(NULL, sizeof(tAux), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if (buffer == MAP_FAILED) {
    	perror("Error en el mmap");
    	shm_unlink(shm_name);
    	close(fd);
    	exit(1);
	}

	// Inicialización de semáforos
	sem_cocineros = sem_open("/sem_cocineros", O_CREAT, 0666, 0);
	if (sem_cocineros == SEM_FAILED) {
    	perror("Error al abrir semáforo sem_cocineros");
    	exit(1);
	}
	sem_salvajes = sem_open("/sem_salvajes", O_CREAT, 0666, 0);
	if (sem_salvajes == SEM_FAILED) {
    	perror("Error al abrir semáforo sem_salvajes");
    	exit(1);
	}
	sem_mtx = sem_open("/sem_mtx", O_CREAT, 0666, 1);
	if (sem_mtx == SEM_FAILED) {
    	perror("Error al abrir semáforo sem_mtx");
    	exit(1);
	}

	buffer->raciones=0;
	buffer->cocineros_esp=0;
	buffer->salvajes_esp=0;


	struct sigaction sa;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGTERM, &sa, NULL) == -1) {
    	perror("Error al establecer sigaction para SIGTERM");
    	exit(1);
	}

	if (sigaction(SIGINT, &sa, NULL) == -1) {
    	perror("Error al establecer sigaction para SIGINT");
    	exit(1);
	}

	cook();

	while (!finish) {
    	pause();
	}

	// Cerrar y liberar recursos
	sem_close(sem_cocineros);
	sem_close(sem_salvajes);
	sem_close(sem_mtx);
	sem_unlink("/sem_cocineros");
	sem_unlink("/sem_salvajes");
	sem_unlink("/sem_mtx");
	munmap(buffer, sizeof(tAux));
	shm_unlink(shm_name);
	close(fd);

	return 0;
}




