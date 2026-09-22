#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

/*
5 Procesos, cada uno escribira una palabra de la siguiente frase
"Hola, esta es mi practica uno"
El proceso padre pondra en pantalla la palabra 'uno'
*/

int main(){

    FILE *archivo = fopen("ejercicio5.txt","w");
    if(archivo == NULL){
        perror("Error al abrir el archivo");
        exit(1);
    }
    int n = 0;
    pid_t pid;
    char* palabras[] = {"Hola,","esta"," es"," mi"," practica"," uno"};

    for(;n<5;n++){
        pid = fork();
        if(pid == 0){   //Hijos
            printf("%s ", palabras[n]);
            fprintf(archivo, "%s ", palabras[n]);
            exit(0);
        }else if(pid > 0 && n == 4){ //Padre
            sleep(2);
            printf("%s\n", palabras[5]);
            fprintf(archivo, "%s\n", palabras[5]);
        }
        sleep(1);
    }
    fclose(archivo);
}