/*
Programa: Creacion de hilos
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

void *impresionMensaje(void *args);

void main()
{
    int numIn = 2;
    pthread_t idHilo[3];
    pid_t pid = fork();
    if(pid == 0){   //Primer hijo
        printf("Primer hijo, creando 3 hilos...\n");
        for(int n=0;n<3;n++)
        {
            printf("Hilo(%d) Creado\n",n);
            pthread_create(&idHilo[n],NULL,impresionMensaje,&numIn);
        }
        for(int n=0;n<3;n++)
        {
            pthread_join(idHilo[n],NULL);
        }
    }else{
        printf("Proceso Padre: %d\n",getppid());
    }

}

void *impresionMensaje(void *args)
{
    int n = *(int*)args;
    int hijoss = 0;
    pthread_t idHilo[2];
    if(n == 2) //Hilos Padres, Crean 2 hilos mas e imprimen id
    {
        //Obtener ID Hilo
        pthread_t id_actual = pthread_self();
        printf("\tHilo Padre, ID:%lu \n", (unsigned long)id_actual);
        for(int m=0;m<n;m++)
        {
            pthread_create(&idHilo[m],NULL,impresionMensaje,&hijoss);
        }
        for(int m=0;m<n;m++)
        {
            pthread_join(idHilo[m],NULL);
        }
    }else{
        printf("\t\tHilos Hijos: 'Hola Mundo' :) \n");
    }
    pthread_exit(NULL);
}