#include "Pila.h"

void push(int elemento,Pila *pila)
{
	pila->contenedor[pila->tope] = elemento;

	++pila->tope;
}

int pop(Pila *pila)
{
	--pila->tope;

	return pila->contenedor[pila->tope];
}

void vaciar(Pila *pila)
{
	pila->tope = 0;
}