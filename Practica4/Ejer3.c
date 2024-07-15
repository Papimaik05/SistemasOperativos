#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>



/*programa que temporiza la ejecución de un proceso hijo */
void alarm_handler(int sig){
 kill(getpid(),SIGKILL);
}

int main(int argc, char **argv)
{
    if(argc <2){
    perror("Uso inadecuado : Programa Parametro");
    return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
   		 perror("Error en el fork");
   		 exit(1);
   	 }
    else if (pid == 0) {
   		 // Código del proceso hijo
   		 if(execvp(argv[1],&argv[1])){
   	 perror("Error al ejecutar execvp");
   		 exit(1); // Salir si execvp falla
   	 }
   	 }
    else {
   		 // Código del proceso padre
   	 struct sigaction sa;
   	 sa.sa_handler=alarm_handler;
   	 sa.sa_flags=0;
   	 sigaction(SIGALRM,&sa,NULL);
   	 alarm(5);
   	 int status;
   	 pid_t pid2=waitpid(pid,&status,0);
   		 
   	 if (pid2 == -1) {
      		 perror("Error al esperar al proceso hijo");
       		 exit(EXIT_FAILURE);
  			 }
  			  // Comprobar la causa de la finalización del hijo
   		 if (WIFEXITED(status)) {
      		  printf("El proceso hijo ha terminado normalmente con código de salida: %d\n", WEXITSTATUS(status));
   		 } else if (WIFSIGNALED(status)) {
       		 printf("El proceso hijo ha sido terminado por una señal: %d\n", WTERMSIG(status));
   		 }

   	 }    

    return 0;
}#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>



/*programa que temporiza la ejecución de un proceso hijo */
void alarm_handler(int sig){
 kill(getpid(),SIGKILL);
}

int main(int argc, char **argv)
{
    if(argc <2){
    perror("Uso inadecuado : Programa Parametro");
    return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
   		 perror("Error en el fork");
   		 exit(1);
   	 }
    else if (pid == 0) {
   		 // Código del proceso hijo
   		 if(execvp(argv[1],&argv[1])){
   	 perror("Error al ejecutar execvp");
   		 exit(1); // Salir si execvp falla
   	 }
   	 }
    else {
   		 // Código del proceso padre
   	 struct sigaction sa;
   	 sa.sa_handler=alarm_handler;
   	 sa.sa_flags=0;
   	 sigaction(SIGALRM,&sa,NULL);
   	 alarm(5);
   	 int status;
   	 pid_t pid2=waitpid(pid,&status,0);
   		 
   	 if (pid2 == -1) {
      		 perror("Error al esperar al proceso hijo");
       		 exit(EXIT_FAILURE);
  			 }
  			  // Comprobar la causa de la finalización del hijo
   		 if (WIFEXITED(status)) {
      		  printf("El proceso hijo ha terminado normalmente con código de salida: %d\n", WEXITSTATUS(status));
   		 } else if (WIFSIGNALED(status)) {
       		 printf("El proceso hijo ha sido terminado por una señal: %d\n", WTERMSIG(status));
   		 }

   	 }    

    return 0;
}
