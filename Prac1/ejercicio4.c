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
    printf("Ejercicio 4............\n");

    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("Fork no se pudo inicializar...\n");
        break;
        case 0: //Child
            printf("\tPrimer Hijo (%d), creando 3 procesos\n",getpid());
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
                            printf("\t\t\tHijo terminal... PID delPadre:%d\n",getppid());
                            sleep(1);
                            sleep(1);
                            exit(0);
                        }
                    }
                    exit(0);
                }else if(pid > 0)
                {
                    printf("\tProceso padre, PID del hijo: %d\n",pid);
                }else
                {
                    perror("Fork no se pudo inicializar...\n");
                }
            }
        break;
        default: //Padre
            printf("Primer Padre, PID del Hijo: %d\n",pid);
        break;
    }
}