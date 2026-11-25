/*
Suma de dos numeros decimales------------------------------------------------------------
*/

#include<stdio.h>
#include<stdlib.h>
#include "operaciones.h"

int main()
{
    double a,b,resultado;
    int ope;
    //Pedida al usuario
    printf("Ingrese numero 1\n");
    scanf("%lf",&a);
    printf("Ingrese numero 2\n");
    scanf("%lf",&b);
    printf("Tipo de operacion\n1 - Suma\n");
    printf("2 - Resta\n3 - Multiplicacion");
    printf("4 - Division\n");
    scanf("%d",&ope);
    
    resultado = operacion(a,b,ope);
    printf("Resultado -> %lf\n",resultado);

    return (0);
}