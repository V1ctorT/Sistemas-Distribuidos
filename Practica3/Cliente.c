#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Puerto a utilizar
#define PUERTO 3000

// Local host, la computadora local
#define DIR_SERV "192.168.10.1"

#define TAM_MAX 1024

int main()
{
    int desc_socket;
    socklen_t tam_estruc;
    struct sockaddr_in dest;
    char buffer[TAM_MAX];
    char *men = "Iniciando...";

    // Abre socket tipo datagrama (UDP)
    desc_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (desc_socket < 0)
    {
        perror("Error al inicializar socket");
        exit(EXIT_FAILURE);
    }

    // Inicio de puerto y direccion de servidor
    memset(&dest, 0, sizeof(dest));
    //bzero(&dest, sizeof());

    dest.sin_family = AF_INET;
    dest.sin_port = htons(PUERTO);

    // Convierte la direccion IP
    if (inet_pton(AF_INET, DIR_SERV, &dest.sin_addr) <= 0)
    {
        perror("Error con la direccion IP");
        close(desc_socket);
        exit(EXIT_FAILURE);
    }

    // Envia mensaje de inicio
    if (sendto(desc_socket, men, strlen(men) + 1, 0,
        (struct sockaddr *)&dest, sizeof(dest)) < 0)
    {
        perror("Error al enviar mensaje");
        close(desc_socket);
        exit(EXIT_FAILURE);
    }

    // Recibe mensaje
    memset(buffer, 0, TAM_MAX);

    tam_estruc = sizeof(dest);

    int byts_recibidos = recvfrom(desc_socket, buffer, TAM_MAX - 1, 0,
        (struct sockaddr *)&dest, &tam_estruc);

    if (byts_recibidos < 0)
    {
        perror("Error al recibir mensaje");
        close(desc_socket);
        exit(EXIT_FAILURE);
    }

    buffer[byts_recibidos] = '\0';

    printf("Respuesta del servidor: %s\n", buffer);

    // Cierra socket
    close(desc_socket);

    return 0;
}
