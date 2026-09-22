#include<stdio.h>
#include<pthread.h>

void *funcionHilo(void *args);

void main(void)
{
    pthread_t id_hilo;
    int arg[2] = {2,3};
	printf("Creacion del hilo...\n");
	pthread_create(&id_hilo, NULL, funcionHilo, NULL);  //Iniciando hilo
	printf("Hilo Creado, Esperando Finalizacion...\n");
	pthread_join(id_hilo,NULL); 			    //Esperando a que termine
	printf("Hilo Finalizado...\n\tValor 1: %i\n\tValor2: %i\n",*arg,*(arg+1));
}

void *funcionHilo(void *argu)
{
    int args = (int *) (argu);
    printf("Valor 1: %i\nValor2: %i\n",*args,*(args+1));
    *args = 7;
    *(args+1)=8;
    pthread_exit(NULL);
}