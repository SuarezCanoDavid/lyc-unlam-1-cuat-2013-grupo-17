#include "mensaje.h"
#include "operacion.h"
#include <stdio.h>

void escribirMensaje(int a, int b)
{
    printf("La suma de %d y %d es: %d\n",a,b,suma(a,b));
}
