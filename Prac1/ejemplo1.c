#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
    pid_t pid;
    char *message;
    int n;

    printf("Fork Program starting\n");
    pid = fork();
    switch (pid)
    {
    case -1:
        /* code */
        perror("Fork Failed\n");
    case 0:
        message = "This is the child";
        printf("PID Child: %d\n",pid);
        n = 5;
        break;
    default:
        message = "This is the parent";
        printf("PID parent: %d\n",pid);
        n = 3;
        break;
    }

    for (; n > 0; n --)
    {
        puts(message);
        sleep(1);
    }
    exit(0);
}