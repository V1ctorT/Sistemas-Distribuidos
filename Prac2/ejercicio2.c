#include<stdio.h>
#include<pthread.h>

void *funcionHilo(void *args); 				    //Prototipo de funcion

void main(void)
{
	pthread_t id_hilo;
	printf("Creacion del hilo...\n");
	pthread_create(&id_hilo, NULL, funcionHilo, NULL);  //Iniciando hilo
	printf("Hilo Creado, Esperando Finalizacion...\n");
	pthread_join(id_hilo,NULL); 			    //Esperando a que termine
	printf("Hilo Finalizado...\n");
}

void *funcionHilo(void *args)
{
	printf("\tDentro del hilo\n");
	pthread_exit(NULL);
}
