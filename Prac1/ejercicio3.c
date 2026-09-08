#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(void)
{
	int pid=0;
	pid=fork();
if(pid==0)
{
		printf("Codigo a ejecutar en el proceso hijo\n");
		exit(0);
}
else
{
		printf("Codigo a ejecutar en el proceso padre\n");
		exit(0);
}
} 
