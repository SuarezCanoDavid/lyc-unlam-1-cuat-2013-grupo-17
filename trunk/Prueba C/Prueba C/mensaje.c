#include "mensaje.h"
#include "operaciones.h"
#include <stdio.h>

void escribirMensaje(int a, int b)
{
	printf("La suma de %d y %d es: %d\n",a,b,sumar(a,b));
}