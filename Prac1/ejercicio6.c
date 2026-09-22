#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

/*------------------------------------------
Red de Anillo Unidireccional
Linea de comando dice el numero de procesos sobre el anillo

*/

int main(){

    typedef struct Token{
        int destino;
        int hid1;
        int pid2;
        int viajes;
        int fin;
        char *mensaje;
    }Token;

    int no_Procesos = 0;
    printf("Ingrese numero de procesos: ");
    scanf("%d", &no_Procesos);

    //Creacion de los pipes para la comunicacion entre procesos
    int pipes[no_Procesos][2];
    for(int i = 0; i < no_Procesos; i++){
        pipe(pipes[i]);
    }

    //Creacion de n procesos
    int noP = 0;
    int idp;    //PID del proceso
    int idpp;   //PID del proceso padre
    pid_t pid;
    for(int i = 0; i < no_Procesos; i++){
        pid = fork();
        if(pid == 0){
            noP = i;
            idp = getpid();
            idpp = getppid();
            printf("Proceso %d creado| PID:%d |PPID:%d\n", i, idp, idpp);
            break;
        }
    }

    Token token;
    //Inicializamos Token para que el primer proceso lo envie
    if(noP == 0)
    {
        token.destino = no_Procesos -1;
        token.hid1 = idp;
        token.pid2 = idpp;
        token.viajes = 0;
        token.mensaje = "Holaaa, es un mensaje para el ultimo proceso :<)";
        token.fin = 0;
        write(pipes[0][1],&token,sizeof(token));
    }

    //Calculamos entrada
    int entrada = (noP - 1 + no_Procesos) % no_Procesos;
    //Ciclo para mandar mensaje
    while (1)
    {
        sleep(1);
        // Recibir token
        read(pipes[entrada][0], &token, sizeof(token));

        //printf("(%d) Leyendo... mensaje a %d\n",noP, token.destino);

        // Si es token de finalización
        if (token.fin == 1)
        {
            printf("(%d) Terminando proceso...\n", noP);

            write(pipes[noP][1], &token, sizeof(token));

            exit(0);
        }

        // Revisamos si el mensaje es para este proceso
        if (token.destino == noP)
        {
            printf("(%d) Recibido el mensaje: %s\n",noP, token.mensaje);
        }
        else
        {
            //printf("Proceso(%d): El mensaje no es para mi...\n", noP);
        }

        // P0 cuenta las vueltas completas
        if (noP == 0)
        {
            token.viajes++;

            printf("Total de ciclos: %d\n", token.viajes);

            if (token.viajes >= 3)
            {
                token.fin = 1;

                printf("(%d) Se completaron 3 ciclos... finalizando...\n",noP);
            }
        }

        // Todos pasan el token al siguiente
        printf("(%d) Escribiendo hacia pipa: %d\n",noP, noP);

        write(pipes[noP][1], &token, sizeof(token));

        // Si P0 acaba de activar fin, termina
        if (noP == 0 && token.fin == 1)
        {
            exit(0);
        }
    }
    exit (0);
}