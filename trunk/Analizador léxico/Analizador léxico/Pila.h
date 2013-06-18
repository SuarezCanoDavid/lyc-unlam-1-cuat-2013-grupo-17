#include "Cola.h"

#ifndef PILA_H
#define PILA_H

#define TAM_PILA	20

struct PilaDeInt
{
	int contenedor[TAM_PILA];
	int tope;
};
typedef struct PilaDeInt PilaDeInt;

void pushInt(int elemento,PilaDeInt *pila);
int popInt(PilaDeInt *pila);
void vaciarPilaDeInt(PilaDeInt *pila);
int VerTope(PilaDeInt *pila);

struct PilaDeColas
{
	ColaDeTercetos *contenedor[TAM_PILA];
	int tope;
};
typedef struct PilaDeColas PilaDeColas;

void pushCola(ColaDeTercetos *c,PilaDeColas *pila);
ColaDeTercetos *popCola(PilaDeColas *pila);
void vaciarPilaDeCola(PilaDeColas *pila);

#endif