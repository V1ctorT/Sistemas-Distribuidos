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
// #define DIR_SERV "192.168.10.1"

#define DIR_SERV "192.168.100.51"

#define TAM_MAX 1024

int main(int argc, char *argv[])
{
    if (argc < 2) { //Validar que se ingreso un archivo
        printf("Uso: %s nombre_archivo\n", argv[0]);
        return 1;
    }

    int desc_socket;
    socklen_t tam_estruc;
    struct sockaddr_in dest;
    //Memoria para el mensaje recibido
    char buffer[TAM_MAX];
    //El nombre del archivo
    char *men = argv[1];
    char solicitud[TAM_MAX];
    
    strcpy(solicitud, men);

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

    // Envia mensaje de solicitud-----------------------------------------------------------
    printf("Enviando peticion al servidor-> %s\n", solicitud );
    int bytes_enviados = sendto(desc_socket, solicitud, strlen(solicitud) + 1, 0,
        (struct sockaddr *)&dest, sizeof(dest));
    printf("Bytes enviados:%d\n",bytes_enviados);
    if (bytes_enviados < 0)
    {
        perror("Error al enviar peticion");
        close(desc_socket);
        exit(EXIT_FAILURE);
    }

    // Recibe mensaje
    memset(buffer, 0, TAM_MAX);

    tam_estruc = sizeof(dest);

    //Recibe mensaje de consulta permisos para escribir----------------------------------
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

    if(strcmp(buffer,"noEncontrado:Terminar") == 0){
        perror("Error No se encontro el archivo...");
        close(desc_socket);
        exit(EXIT_FAILURE);
    }

    char validacion[1024];
    printf("Comprobando permisos: \n");
    int permiso = access("./Cliente_Files", W_OK);
    printf("Permiso -> %d\n",permiso);
    if(permiso == 0)
    {
        strcpy(buffer, "ACEPTAR");
    }else
    {
        strcpy(buffer, "RECHAZAR");
    }
    
    printf("Validacion -> %s\n", buffer);

    //Manda el mensaje para validacion de permisos---------------------------------------
    if(sendto(desc_socket, buffer, strlen(buffer) + 1, 0,
        (struct sockaddr *)&dest, sizeof(dest)) < 0)
        {
        perror("Error al enviar validacion");
        close(desc_socket);
        exit(EXIT_FAILURE);
        }
    printf("Recibiendo archivo...\n");
    
    //Recibe el archivo
    FILE *archivo;
    char bloque[1024];
    //Armar la ruta con el nombre del archivo
    char ruta[256];
    snprintf(ruta,sizeof(ruta), "./Cliente_Files/%s", argv[1]);
    archivo = fopen(ruta, "wb");
    if(archivo == NULL){
        perror("Error al abrir el archivo");
        close(desc_socket);
        exit(EXIT_FAILURE);
    }

    int bytes;
    while ((bytes = recvfrom(desc_socket, bloque,sizeof(bloque),0,NULL,NULL)) > 0)
    {
        if (bytes == 4 && memcmp(bloque, "FIN", 4) == 0)
        {
            printf("Transferencia finalizada.\n");
            break;
        }
    fwrite(bloque, 1, bytes, archivo);
    }

    fclose(archivo);
    

    // Cierra socket
    close(desc_socket);

    return 0;
}
