/*
Comunicacion UDP - Programa servidor

El cliente envia
    tipo de operacion 
        Suma, Resta, Multiplicacion y Division
        Serie de numeros que se operan
El servidor envia
    El calculo de dicha operacion
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PUERTO 3000
#define TAM_MAX 1024
const char *p;

//Funciones para procesar la entrada-----------------------------------------------------
//Numero
double numero()
{
    while(isspace(*p))
        p++;
        
    double n = strtod(p, (char**)&p);
    return n;
    
}
//Termino
double termino()
{
    double resultado = numero();

    while (1)
    {
        while (isspace(*p))
            p++;

        if(*p == '*')
        {
            p++;
            resultado *= numero();
        }
        else if(*p == '/')
        {
            p++;
            resultado /= numero();
        }else break;
        
    }
    return resultado;
}

//Expresion
double expresion()
{
    double resultado = termino();
    while (1)
    {
        while (isspace(*p))
            p++;

        if (*p == '+')
        {
            p++;
            resultado += termino();
        }
        else if (*p == '-')
        {
            p++;
            resultado -= termino();
        }
        else
            break;
    }

    return resultado;
}


int main()
{
    int desc_socket, tam_dest;
    struct sockaddr_in direc, dest;
    char buffer[TAM_MAX];
    char men[TAM_MAX];
    printf("Inicio...\n");

    //Apertura del Canal-----------------------------------------------------------------
    desc_socket = socket(AF_INET,SOCK_DGRAM,0);
    
    //Asignar 0s a las direcciones
    bzero(&direc,sizeof(direc));
    
    //Propiedades de la direccion
    direc.sin_family = AF_INET;
    direc.sin_port = htons(PUERTO);
    direc.sin_addr.s_addr = INADDR_ANY;

    //Bind - Publicidad de la conexion, asocia el socket al puerto ----------------------
    bind(desc_socket,(struct sockaddr*)&direc, sizeof(direc));
    tam_dest = sizeof(dest);

    while (1)
    {
    //Esperar mensaje del cliente--------------------------------------------------------
    recvfrom(desc_socket, men, strlen(men)+1,0,(struct sockaddr *)&dest, &tam_dest);
    printf("Conectando... Calculando Operacion -> %s\n",men);

    printf("Servicio: Calculando operacion -> %s\n",men);
    p = men;
    double resultado = expresion();

    //Conversion a texto
    snprintf(buffer, sizeof(buffer), "%.2f", resultado);
    printf("Resultado obtenido char = %s | double = %.2f\n",buffer, resultado);

    //Responder al cliente---------------------------------------------------------------
    sendto(desc_socket, buffer, strlen(buffer)+1,0,(struct sockaddr *)&dest, sizeof(dest));
    }
    

    close(desc_socket);
    return 0;
}