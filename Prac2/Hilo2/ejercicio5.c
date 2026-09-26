#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

void *ocurrencia(void *args);
void *copyDirectory(void *args);
void *report(void *args);

struct report
{
    int conc;
    int copias;
    char *cadena;
};

void main()
{
    pthread_t hilo1;
    pthread_t hilo2;
    pthread_t hilo3;

    //ocurrencia
    char *cadena = "queso";
    int *concurrencia;
    int *arch;

    pthread_create(&hilo1,NULL,ocurrencia,cadena);
    printf("Esperando el conteo del hilo 1...\n");

    //directorio
    pthread_create(&hilo2,NULL,copyDirectory,&arch);
    printf("Esperando tarea del hilo 2...\n");

    //Finalizacion
    pthread_join(hilo1,(void**)&concurrencia);
    printf("Hilo 1 Finalizado, se obtuvo: %d '%s'\n",*concurrencia,cadena);    

    pthread_join(hilo2,(void**)&arch);
    printf("Hilo 2 Finalizado, se copiaron: %d archivos\n",*arch);

    struct report datos;
    datos.conc = *concurrencia;
    datos.copias = *arch;
    datos.cadena = "queso";

    //Reporte
    pthread_create(&hilo3,NULL,report,&datos);
    printf("Esperando tarea del hilo 3...\n");
    pthread_join(hilo3,NULL);
    printf("Hilo 3 Finalizado\n");

}

void *ocurrencia(void *args)
{
    char *pal = (char*)args;
    int *oc = malloc(sizeof(int));
    *oc = 0;
    char palabra_texto[100];
    //Abrir el archivo
    const char *arch = "Texto.txt";
    FILE *archivo = fopen(arch,"r");
    if (archivo == NULL){
        perror("Error al abrir el archivo...\n");
        pthread_exit(0);
    }
    while(fscanf(archivo, "%99s", palabra_texto) == 1)
    {
        if(strcmp(palabra_texto, pal) == 0) *oc += 1;
    }
    fclose(archivo);
    printf("\tLa palabra '%s' aparece: %d veces\n",pal,*oc);
    pthread_exit(oc);
}

void *copyDirectory(void *args)
{
    int *copias = malloc(sizeof(int));
    *copias = 0;
    const char *origen = ".";
    const char *destino = "./Hilo2";
    mkdir(destino, 0700);

    DIR *dir = opendir(origen);
    struct dirent *entrada;

    if(dir == NULL){
        perror("Error al abrir el directorio de origen...\n");
        pthread_exit(0);
    }

    while ((entrada = readdir(dir)) != NULL)
    {
        if(strcmp(entrada->d_name, ".")==0 ||
         strcmp(entrada->d_name,"..")==0) continue;


        if (entrada->d_type != DT_REG) continue;
        
        
        char ruta_origen[1024];
        char ruta_destino[1024];

        snprintf(ruta_origen, sizeof(ruta_origen),"%s/%s", origen, entrada->d_name);
        snprintf(ruta_destino, sizeof(ruta_destino), "%s/%s", destino, entrada->d_name);

        printf("\tCopiando Archivos: %s -> %s\n", ruta_origen,ruta_destino);

        //Abrir origen
        int fd_origen = open(ruta_origen,O_RDONLY);
        if(fd_origen == -1){
            perror("Error al abrir el archivo...\n");
            continue;
        }
        // Crear destino
        int fd_destino = open(ruta_destino,
                              O_WRONLY | O_CREAT | O_TRUNC,
                              0600);

        if (fd_destino == -1)
        {
            perror("Error al crear archivo");
            close(fd_origen);
            continue;
        }

        // Copiar
        char buffer[4096];
        ssize_t bytes;

        while ((bytes = read(fd_origen,
                             buffer,
                             sizeof(buffer))) > 0)
        {
            write(fd_destino, buffer, bytes);
        }

        close(fd_origen);
        close(fd_destino);
        *copias += 1;
    }

    closedir(dir);
    pthread_exit(copias);
}

void *report(void *args)
{
    struct report *datos = (struct report *)args;
    printf("\tHilo3: Creando reporte... \n");
    printf("\tConcurrencia de la palabra '%s': %d\n",datos->cadena, datos->conc);
    printf("\tArchivos Copiados: %d\n", datos->copias);

    FILE *reporte = fopen("Reporte.txt","w");
    if(reporte == NULL){
        perror("No se pudo abrir o crear el archivo 'Reporte.txt...\n'");
        pthread_exit(NULL);
    }
    fprintf(reporte,"-------------------------------------------------------------------------\n");
    fprintf(reporte,"Reporte de las tareas de los Hilos:\n");
    fprintf(reporte,"Hilo 1: Conteo de concurrencia de la palabra '%s': %d veces\n",
        datos->cadena,datos->conc);
    fprintf(reporte,"Hilo 2: Archivos copiados: %d\n",datos->copias);
    fprintf(reporte,"Hilo 3: Creado archivo reporte en 'Reporte.txt'\n");
    fprintf(reporte,"-------------------------------------------------------------------------\n");
    fclose(reporte);
    pthread_exit(NULL);
}