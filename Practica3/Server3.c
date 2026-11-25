#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 3000
#define TAM_MAX 1024


int buscarArchivo(const char *directorio, const char *nombre, char *rutaEncontrada, size_t tamRuta)
{
    DIR *dir;
    struct dirent *entrada;
    struct stat info;

    char ruta[1024];

    dir = opendir(directorio);

    if (dir == NULL)
        return 0;

    while ((entrada = readdir(dir)) != NULL)
    {
        if (strcmp(entrada->d_name, ".") == 0 ||
            strcmp(entrada->d_name, "..") == 0)
            continue;

        snprintf(ruta, sizeof(ruta), "%s/%s",
                 directorio, entrada->d_name);

        if (stat(ruta, &info) == -1)
            continue;

        if (S_ISREG(info.st_mode))
        {
            if (strcmp(entrada->d_name, nombre) == 0)
            {
                snprintf(rutaEncontrada, tamRuta, "%s", ruta);
                closedir(dir);
                return 1;
            }
        }
        else if (S_ISDIR(info.st_mode))
        {
            if (buscarArchivo(ruta, nombre,
                              rutaEncontrada, tamRuta))
            {
                closedir(dir);
                return 1;
            }
        }
    }

    closedir(dir);
    return 0;
}



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
    
    //Propiedades de la direccion
    direc.sin_family = AF_INET;
    direc.sin_port = htons(PUERTO);
    direc.sin_addr.s_addr = INADDR_ANY;
    //Publicidad de la conexion, asocia el socket al puerto
    bind(desc_socket,(struct sockaddr*)&direc, sizeof(direc));
    tam_dest = sizeof(dest);

    //Esperar la solicitud del archivo del cliente---------------------------------------
    printf("Esperando Solicitud de Archivo...\n");
    int bytes = recvfrom(desc_socket, men, TAM_MAX+1,0,(struct sockaddr *)&dest, &tam_dest);
    men[bytes] = '\0';
    printf("Conectando %d , Archivo peticion: %s\n",bytes, men);

    char rutaEncontrada[1024];
    printf("Buscando Archivo: %s\n",men);
    int enc = 0;
    if(buscarArchivo("Server3_Files", men, rutaEncontrada, sizeof(rutaEncontrada)))
    { 
        printf("Archivo encontrado y disponible (%s), validando permisos...\n",men);
        printf("Ruta: %s\n", rutaEncontrada);
        strcpy(men, "Encontrado:Validar");
        enc = 1;
    }
    else { 
        printf("Archivo no encontrado\n"); 
        strcpy(men, "noEncontrado:Terminar");
    }


    //Responder al cliente que si esta el archivo, esperando validacion de escritura
    sendto(desc_socket, men, strlen(men)+1,0,(struct sockaddr *)&dest, sizeof(dest));

    if(enc == 0)
    {
            perror("Verifique el nombre del archivo...\n");
            close(desc_socket);
            exit(EXIT_FAILURE);
    }

    //Esperar la validacion del archivo del cliente--------------------------------------
    bytes = recvfrom(desc_socket, men, TAM_MAX+1,0,(struct sockaddr *)&dest, &tam_dest);
    men[bytes] = '\0';
    printf("Validacion -> %s\n",men);

    //Si se validaron los permisos
    if(strcmp(men, "ACEPTAR") == 0)
    {
        printf("Enviando archivos...\n");
        //Empezamos a enviar los datos
        char bloque[1024];
        FILE *archivo = fopen(rutaEncontrada, "rb");
        if(archivo == NULL)
        {
            perror("Error al abrir el archivo");
            close(desc_socket);
            exit(EXIT_FAILURE);
        }
        size_t bytes;
        while ((bytes = fread(bloque,1,sizeof(bloque),archivo))>0)
        {
            sendto(desc_socket, bloque, bytes, 0, (struct sockaddr *)&dest, sizeof(dest));
            printf("Enviando > %zu\n",bytes);
        }
        sendto(desc_socket, "FIN", 4, 0, (struct sockaddr *)&dest, sizeof(dest));
    }

    close(desc_socket);
    return 0;
}