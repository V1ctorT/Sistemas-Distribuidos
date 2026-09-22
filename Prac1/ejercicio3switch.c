#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(void)
{
	int pid=0;
	pid=fork();

	switch (pid)
	{
	case -1:
		perror("Error al crear el proceso...");
		exit(1);
	case 0:
		printf("Codigo a ejecutar en el proceso hijo\n");
		exit (0);
	
	default:
		printf("Codigo a ejecutar en el proceso padre\n");
		exit(0);
	}
} 