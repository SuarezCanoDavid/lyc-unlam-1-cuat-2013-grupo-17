#include "Pila.h"

void pushInt(int elemento,PilaDeInt *pila)
{
	pila->contenedor[pila->tope] = elemento;

	++pila->tope;
}

int popInt(PilaDeInt *pila)
{
	--pila->tope;

	return pila->contenedor[pila->tope];
}

void vaciarPilaDeInt(PilaDeInt *pila)
{
	pila->tope = 0;
}

int VerTope(PilaDeInt *pila)
{
	return pila->contenedor[pila->tope-1];

}

void pushCola(ColaDeTercetos *c,PilaDeColas *pila)
{
	pila->contenedor[pila->tope] = c;

	++pila->tope;
}

ColaDeTercetos *popCola(PilaDeColas *pila)
{
	--pila->tope;

	return pila->contenedor[pila->tope];
}

void vaciarPilaDeCola(PilaDeColas *pila)
{
	pila->tope = 0;
}