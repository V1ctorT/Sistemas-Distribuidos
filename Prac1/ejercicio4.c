#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

/*------------------------------------------
Ejercicio 4
Crea un proceso hijo a partir de un proceso padre,
el hijo creado a su vez creara 3 procesos mas
Cada uno de los 3 procesos creara dos procesos mas

Cada uno de los procesos creados imprimira 
    el PID de su proceso padre si es hijo terminal
    o el PID de sus hijos si es proceso padre
*/

/*
Datos Entrada: Ninguna
Datos Auxiliares: PID de los procesos
Datos Salida: mensaje 
    PID de proceso padre    
    PID de proceso hijo
*/

int main(){
    pid_t pid;
    char *message;

    printf("Ejercicio 4............\n");
    pid = fork();
    switch(pid)
    {
        case -1:
            perror("Fork no se pudo inicializar...\n");
        break;
        case 0: //Child
            printf("\tPrimer Hijo, creando 3 procesos\n");
            for(int n = 0; n < 3; n++)
            {
                pid = fork();
                if(pid == 0)
                {
                    printf("\t\tProceso no %d, PID %d\n",n,getpid());
                    printf("\t\tAhora creando 2 procesos mas.\n");
                    for(int n1 = 0; n1 < 2; n1++)
                    {
                        pid = fork();
                        if(pid == 0)
                        {
                            printf("\t\t\tHijos terminales... PPID:%d\n",getppid());
                            exit(0);
                        }
                    }
                    exit(0);
                }
            }
        break;
        default: //Padre
        //    printf("PID parent: %d\n",pid);
        break;
    }
    sleep(1);
    sleep(1);
    sleep(1);
}