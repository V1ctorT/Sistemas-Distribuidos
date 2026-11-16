#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 3000
#define TAM_MAX 1024

int main()
{
    int desc_socket, tam_dest;
    struct sockaddr_in direc, dest;
    char *buffer = "Saludos a todos desde servidorr";
    char men[TAM_MAX];
    printf("Inicio...\n");
    desc_socket = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&direc,sizeof(direc));
    direc.sin_family = AF_INET;
    direc.sin_port
}