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
    //mem
    bzero(&direc,sizeof(direc));
    direc.sin_family = AF_INET;
    direc.sin_port = htons(PUERTO);
    direc.sin_addr.s_addr = INADDR_ANY;
    //Publicidad de la conexion, asocia el socket al puerto
    bind(desc_socket,(struct sockaddr*)&direc, sizeof(direc));
    tam_dest = sizeof(dest);

    //Esperar mensaje del cliente...
    recvfrom(desc_socket, men, strlen(men)+1,0,(struct sockaddr *)&dest, &tam_dest);
    printf("Conectando...\n");
    
    //Responder al cliente
    sendto(desc_socket, buffer, strlen(buffer)+1,0,(struct sockaddr *)&dest, sizeof(dest));
    close(desc_socket);
    return 0;
}