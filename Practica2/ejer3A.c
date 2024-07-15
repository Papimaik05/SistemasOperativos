#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"


/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100


static inline char* clone_string(char* original)
{
    char* copy;
    copy=malloc(strlen(original)+1);
    strcpy(copy,original);
    return copy;
}


int print_text_file(char *path){
    FILE* archivo;
    student_t* students;
    student_t* cur_student;
    char line[MAXLEN_LINE_FILE+1];
    char* token;
    char* lineptr;
    token_id_t token_id;
    int entry_idx;
    int nregistros =0;
    int cur_line;

    if ((archivo=fopen(path,"r"))==NULL) {
   	 fprintf(stderr, "El archivo no se puede abrir");
   	 perror(NULL);
   	 return 1;
    }

    if(fscanf(archivo,"%d",&nregistros) != 1 ){
   	 perror("No se ha podido obtener el numero de registros");
   	 return 1;
    }
    
    fgetc(archivo);

    students=malloc(sizeof(student_t));
    /* zero fill the array of structures */
    memset(students,0,sizeof(student_t));

    /* Parse file */

    if(students==NULL){
   	 fprintf(stderr, "El archivo no ha podido reservar memoria");
   	 perror(NULL);
   	 return 1;
    }
    entry_idx=0;
    cur_line=1;
    int i=0;
    while (fgets(line, MAXLEN_LINE_FILE + 1, archivo) != NULL) {

   	 fprintf(stdout,"[Entry #%d]\n",i);
   	 
   	 /* Discard lines that begin with # */
   	 if (line[0]=='#'){
   		 cur_line++;
   		 continue;
   	 }

   	 /* Point to the beginning of the line */
   	 lineptr=line;
   	 token_id=STUDENT_ID_IDX;
   	 cur_student=&students[entry_idx];

   	 while((token = strsep(&lineptr, ":"))!=NULL) {
   		 
   		 switch(token_id) {
   		 case STUDENT_ID_IDX:
   			 if (sscanf(token,"%d",&cur_student->student_id)!=1) {
   				 perror("Warning: Mal formato para el estudiante ID");
   				 cur_student->student_id=0;
   			 }
   			 else{
   			 fprintf(stdout,"\tstudent_id=%d\n",
   		     	cur_student->student_id);
   			 }
   			 break;
   		 case NIF_IDX:
   			 strncpy(cur_student->NIF,token,MAX_CHARS_NIF);
   			 cur_student->NIF[MAX_CHARS_NIF]='\0';
   			 fprintf(stdout,"\tNIF=%s\n",
   		     	cur_student->NIF);
   			 break;
   		 case FIRST_NAME_IDX:
   			 cur_student->first_name=clone_string(token);
   			 fprintf(stdout,"\tfirst_name=%s\n",
   		     	cur_student->first_name);
   			 break;
   		 case LAST_NAME_IDX:
   			 token[strlen(token)-1]='\0';
   			 cur_student->last_name=clone_string(token);
   			 fprintf(stdout,"\tlast_name=%s\n",
   		     	cur_student->last_name);
   			 break;
   		 default:
   			 break;
   		 }
   		 
   		 token_id++;
   	 }
   	 if (token_id!=NR_FIELDS_STUDENT) {
   		 fprintf(stderr, "No se han podido procesar todos los tokens de la linea %d del archivo\n",entry_idx+1);
   		 return 1;
   	 }
   	 cur_line++;
   	 i++;
   	 free(cur_student->first_name);
   	 free(cur_student->last_name);
    }
    free(students);
    fclose(archivo);

    return 0;
}

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

int print_binary_file(char *path,struct options* options)
{    

    FILE* archivo;
    if ((archivo=fopen(path,"rb"))==NULL) {
   	 fprintf(stderr, "El archivo no se puede abrir");
   	 perror(NULL);
   	 return 1;
    }
    int nregistros;
    student_t* students;
    students=malloc(sizeof(student_t));

    fread(&nregistros,sizeof(int),1,archivo);

    int i=0;    
    char* NIF;
    while(i<nregistros){
   	 fprintf(stdout,"[Entry #%d]\n",i);
   	 fread(&students->student_id,sizeof(int),1,archivo);
   	 fprintf(stdout,"\tstudent_id=%d\n",students->student_id);
   	 NIF=loadstr(archivo);
   	 if(NIF !=NULL){
   		 fprintf(stdout,"\tNIF=%s\n",
   		     	NIF);
   		 free(NIF);
   	 }
   	 students->first_name=loadstr(archivo);
   	 if(students->first_name !=NULL){
   		 fprintf(stdout,"\tfirst_name=%s\n",
   		     	students->first_name);
   		 free(students->first_name);
   	 }
   	 students->last_name=loadstr(archivo);
   	 if(students->last_name !=NULL){
   		 fprintf(stdout,"\tlast_name=%s\n",
   		     	students->last_name);
   		 free(students->last_name);
   	 }
   	 i++;
   	 
    }
    fclose(archivo);

    return 0;
}


int write_binary_file(char *input_file, char *output_file,struct options* options)
{    
    FILE* archivo;
    FILE* salida;
    student_t* students;
    student_t* cur_student;
    char line[MAXLEN_LINE_FILE+1];
    char* token;
    char* lineptr;
    token_id_t token_id;
    int entry_idx;
    int nregistros =0;
    int cur_line;

    if ((salida=fopen(output_file,"wb"))==NULL) {
   	 fprintf(stderr, "El archivo salida no se puede abrir");
   	 perror(NULL);
   	 return 1;
    }

    if ((archivo=fopen(input_file,"r"))==NULL) {
   	 fprintf(stderr, "El archivo no se puede abrir");
   	 perror(NULL);
   	 return 1;
    }

    if(fscanf(archivo,"%d",&nregistros) != 1 ){
   	 perror("No se ha podido obtener el numero de registros");
   	 return 1;
    }

    
    fgetc(archivo);

    fwrite(&nregistros,sizeof(int),1,salida);

    students=malloc(sizeof(student_t));
    /* zero fill the array of structures */
    if(students==NULL){
   	 fprintf(stderr, "El archivo no ha podido reservar memoria");
   	 perror(NULL);
   	 return 1;
    }
    memset(students,0,sizeof(student_t));

    /* Parse file */
    entry_idx=0;
    cur_line=1;
    while (fgets(line, MAXLEN_LINE_FILE + 1, archivo) != NULL) {

   	 lineptr=line;
   	 token_id=STUDENT_ID_IDX;
   	 cur_student=&students[entry_idx];

   	 while((token = strsep(&lineptr, ":"))!=NULL) {
   		 
   		 switch(token_id) {
   		 case STUDENT_ID_IDX:
   			 if (sscanf(token,"%d",&cur_student->student_id)!=1) {
   				 perror("Warning: Mal formato para el estudiante ID");
   				 cur_student->student_id=0;
   			 }

   			 fwrite(&cur_student->student_id,sizeof(int),1,salida);
   			 
   			 break;
   		 case NIF_IDX:
   			 strncpy(cur_student->NIF,token,MAX_CHARS_NIF);
   			 fwrite(cur_student->NIF,sizeof(char),strlen(cur_student->NIF)+1,salida);
   			 break;
   		 case FIRST_NAME_IDX:
   			 cur_student->first_name=clone_string(token);
   			 fwrite(cur_student->first_name,sizeof(char),strlen(cur_student->first_name)+1,salida);
   			 break;
   		 case LAST_NAME_IDX:
   			 token[strlen(token)-1]='\0';
   			 cur_student->last_name=clone_string(token);
   			 fwrite(cur_student->last_name,sizeof(char),strlen(cur_student->last_name)+1,salida);
   			 break;
   		 default:
   			 break;
   		 }
   		 
   		 token_id++;

   	 }
   	 if (token_id!=NR_FIELDS_STUDENT) {
   		 fprintf(stderr, "No se han podido procesar todos los tokens de la linea %d del archivo\n",entry_idx+1);
   		 return 1;
   	 }
   	 cur_line++;
    free(cur_student->first_name);
    free(cur_student->last_name);
    }
    free(students);
    fclose(archivo);
    fclose(salida);
    return 0;
}

int main(int argc, char *argv[])
{
    int ret_code, opt;
    struct options options;

    /* Initialize default values for options */
    options.input_file = NULL;
    options.output_file = NULL;
    options.action = NONE_ACT;
    ret_code = 0;

    /* Parse command-line options (incomplete code!) */
    while ((opt = getopt(argc, argv, "hi:po:b")) != -1)
    {
   	 switch (opt)
   	 {
   	 case 'h':
   		 fprintf(stderr, "Usage: %s [ -h | -p | -i file | -o <output_file> | -b]\n", argv[0]);
   		 exit(EXIT_SUCCESS);
   	 case 'i':
   		 options.input_file = optarg;
   		 break;
   	 case 'p':
   		 options.action = PRINT_TEXT_ACT;
   		 break;
   	 case 'o':
   		 options.action = WRITE_BINARY_ACT;
   		 options.output_file = optarg;
   		 break;
   	 
   	 case 'b':
   		 options.action = PRINT_BINARY_ACT;
   		 break;
   	 default:
   		 exit(EXIT_FAILURE);
   	 }
    }

    if (options.input_file == NULL)
    {
   	 fprintf(stderr, "Must specify one record file as an argument of -i\n");
   	 exit(EXIT_FAILURE);
    }

    switch (options.action)
    {
    case NONE_ACT:
   	 fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
   	 ret_code = EXIT_FAILURE;
   	 break;
    case PRINT_TEXT_ACT:
   	 ret_code = print_text_file(options.input_file);
   	 break;
    case WRITE_BINARY_ACT:
   	 /* Part B */
   	 ret_code = write_binary_file(options.input_file, options.output_file,&options);
   	 break;
    case PRINT_BINARY_ACT:
   	 /* Part C */
   	 ret_code = print_binary_file(options.input_file,&options);
   	 break;
    default:
   	 break;
    }
    exit(ret_code);
}
