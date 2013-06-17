#ifndef GCI_H
#define GCI_H

#define MAX_TERCETOS	1003

#define TOKEN		0
#define NRO_TERCETO	1
#define IGNORAR		2
#define INDICE_TS	3
#define JMP			4
#define	JZ			5
#define JNZ			6
#define JG			7
#define JGE			8
#define JL			9
#define JLE			10
#define	JE			11
#define JNE			12
#define BH			13
#define BL			14
#define VALOR		15

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
void borrarTerceto(Terceto *t);

#endif