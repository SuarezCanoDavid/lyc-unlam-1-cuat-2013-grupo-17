#include "Cola.h"


#ifndef PILA_H
#define PILA_H

#define TAM_PILA	50
#define TRUE	1
#define FALSE	0

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
void OrdenarPila(PilaDeInt *pila);
int comparar (const void* p1, const void* p2);
int PilaVacia(PilaDeInt *);
int contiene(PilaDeInt *,int);
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