#ifndef GCI_H
#define GCI_H

#define MAX_TERCETOS	1003

#define TOKEN		0
#define NRO_TERCETO	1
#define IGNORAR		2
#define INDICE_TS	3

struct Terceto
{
	int x;
	int tipoDeX;
	int y;
	int tipoDeY;
	int z;
	int tipoDeZ;
};
typedef struct Terceto Terceto;

int crearTerceto(Terceto *t);
void imprimirTercetos();

#endif