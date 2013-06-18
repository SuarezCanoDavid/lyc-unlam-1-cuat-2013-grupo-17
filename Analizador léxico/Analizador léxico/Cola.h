#include "GCI.h"

#ifndef COLA_H
#define COLA_H

#define TAM_COLA	20

struct ColaDeTercetos
{
	Terceto contenedor[TAM_COLA];
	int tercetoInicial;
	int cantTercetos;
};
typedef struct ColaDeTercetos ColaDeTercetos;

void vaciarColaDeTercetos(ColaDeTercetos *c);
void encolarTerceto(Terceto *t,ColaDeTercetos *c);
Terceto *desencolarTerceto(ColaDeTercetos *c);
int colaDeTercetosEstaVacia(ColaDeTercetos *c);

#endif