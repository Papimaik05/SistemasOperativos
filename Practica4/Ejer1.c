#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>

struct options {
	char *progname;
	int recurse;
};

struct options opt;

void usage(void)
{
	printf("%s [options] [dirname]\n\n", opt.progname);
	printf("lists the contents of dirname (default .)\n");
	printf("options:\n"
  	  "\t-h:\tshow this help\n"
  	  "\t-R:\trecursive\n"
	);
}

/* apartado b */
void list_dir(char *name)
{
	DIR* dir=opendir(name);
	if(dir==NULL){
  	  perror("No se puede abrir");
  	  exit(1);
  	  }
	struct dirent *leer;
  	  while((leer=readdir(dir))!=NULL){
  		  printf("%s\n",leer->d_name);
       }
	closedir(dir);
}



/* apartado c */
void process_recurse(char *dirname, char *name)
{
    char path[PATH_MAX];
	 if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
   	 pid_t pid = fork();
   	 if (pid < 0) {
   		 perror("Error en el fork");
   		 exit(1);
   	 }
    else if (pid == 0) {
   		 // Código del proceso hijo
   		 snprintf(path, sizeof(path), "%s/%s", dirname, name);
   		 execlp(opt.progname, opt.progname, "-R", path, (char *)NULL);
   		 perror("Error al ejecutar execlp");
   		 exit(1); // Salir si execlp falla
   	 }
    else {
   		 // Código del proceso padre
   		 wait(NULL);
   	 }    
    }    
}




/* apartado c */
void list_dir_recurse(char *name)
{
	DIR *d;
	d=opendir(name);
	if(d==NULL){
  	  perror("No se puede abrir");
  	  exit(1);
  	  }
	struct dirent *de;
  	  while((de=readdir(d))!=NULL){
     	if(de->d_type==DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){
  		  process_recurse(name,de->d_name);
  	  	}
  		  printf("%s\n",de->d_name);
  	 
       }

	closedir(d);

}

int main(int argc, char **argv)
{
	char *dirname = ".";
	int o;
	opt.progname = argv[0];
	opt.recurse = 0;

	/* Apartado a: procesar opciones con getopt */

	while ((o = getopt(argc, argv, "hR")) != -1)
  	  {
        switch (o)
        {
        case 'h':
     	   usage();
   	   exit(0);
  	   
     	   break;
        case 'R':
     	   opt.recurse = 1;
     	   break;
        default:
     	   usage();
     	   exit(EXIT_FAILURE);
        }
	}
	if(optind < argc){
  	 dirname=argv[optind];
	}

	/********************************************/
  	 
	if (opt.recurse){
     printf("%s:\n", dirname);
  	  list_dir_recurse(dirname);
	}
	else{
  	  list_dir(dirname);
	}
  	 

	return 0;
}


