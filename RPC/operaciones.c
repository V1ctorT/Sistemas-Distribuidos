/*
    En una libreria debe de haber:
        Codigo de funciones - Procedimientos
        Codigo de definicion de la libreria
    Todo en un mismo directorio
    Principio de modularidad porque empieza a crecer
*/

#include "operaciones.h"

//Declaramos funciones
double operacion(double a, double b, int op)
{
    switch(op)
    {
        case 1:
            return (a+b);
        case 2:
            return (a-b);
        case 3:
            return (a*b);
        case 4:
            return (a/b);
        default:
            return 0.0;

    }

}
