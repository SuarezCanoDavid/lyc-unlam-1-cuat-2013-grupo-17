#include "GCI.h"
#include "AnalizadorLexico.h"
#include <stdio.h>
#include <string.h>

int cantTercetos = 0;

extern tablaDeSimbolos TS[LONG_TS];

Terceto listaDeTercetos[MAX_TERCETOS];


int crearTerceto(Terceto *t)
{
	listaDeTercetos[cantTercetos].x = t->x;
	listaDeTercetos[cantTercetos].tipoDeX = t->tipoDeX;
	listaDeTercetos[cantTercetos].y = t->y;
	listaDeTercetos[cantTercetos].tipoDeY = t->tipoDeY;
	listaDeTercetos[cantTercetos].z = t->z;
	listaDeTercetos[cantTercetos].tipoDeZ = t->tipoDeZ;
	
	return cantTercetos++; 
}

void imprimirTercetos()
{
	int i;
	FILE *archivoDeTercetos;

	fopen_s(&archivoDeTercetos,"Tercetos.txt","w");

	for(i = 0; i < cantTercetos; ++i)
	{
		fprintf(archivoDeTercetos,"[%d] (",i);

		switch(listaDeTercetos[i].tipoDeX)
		{
			case INDICE_TS:		fprintf(archivoDeTercetos,"%s,",TS[listaDeTercetos[i].x].nombre);
								break;
			case TOKEN:			fprintf(archivoDeTercetos,"%s,",identificarTipoToken(listaDeTercetos[i].x));
								break;
			case NRO_TERCETO:	fprintf(archivoDeTercetos,"[%d],",listaDeTercetos[i].x);
								break;
			case IGNORAR:		fprintf(archivoDeTercetos,"_,");
								break;
			case JMP:			fprintf(archivoDeTercetos,"JMP,");
								break;
			case JZ:			fprintf(archivoDeTercetos,"JZ,");
								break;
			case JNZ:			fprintf(archivoDeTercetos,"JNZ,");
								break;
			case JG:			fprintf(archivoDeTercetos,"JG,");
								break;
			case JGE:			fprintf(archivoDeTercetos,"JGE,");
								break;
			case JL:			fprintf(archivoDeTercetos,"JL,");
								break;
			case JLE:			fprintf(archivoDeTercetos,"JLE,");
								break;
			case JE:			fprintf(archivoDeTercetos,"JE,");
								break;
			case JNE:			fprintf(archivoDeTercetos,"JNE,");
								break;
			case BH:			fprintf(archivoDeTercetos,"BH,");
								break;
			case BL:			fprintf(archivoDeTercetos,"BL,");
								break;
			case VALOR:			fprintf(archivoDeTercetos,"%xh,",listaDeTercetos[i].x);
								break;
		}

		switch(listaDeTercetos[i].tipoDeY)
		{
			case INDICE_TS:		fprintf(archivoDeTercetos,"%s,",TS[listaDeTercetos[i].y].nombre);
								break;
			case TOKEN:			fprintf(archivoDeTercetos,"%s,",identificarTipoToken(listaDeTercetos[i].y));
								break;
			case NRO_TERCETO:	fprintf(archivoDeTercetos,"[%d],",listaDeTercetos[i].y);
								break;
			case IGNORAR:		fprintf(archivoDeTercetos,"_,");
								break;
			case JMP:			fprintf(archivoDeTercetos,"JMP,");
								break;
			case JZ:			fprintf(archivoDeTercetos,"JZ,");
								break;
			case JNZ:			fprintf(archivoDeTercetos,"JNZ,");
								break;
			case JG:			fprintf(archivoDeTercetos,"JG,");
								break;
			case JGE:			fprintf(archivoDeTercetos,"JGE,");
								break;
			case JL:			fprintf(archivoDeTercetos,"JL,");
								break;
			case JLE:			fprintf(archivoDeTercetos,"JLE,");
								break;
			case JE:			fprintf(archivoDeTercetos,"JE,");
								break;
			case JNE:			fprintf(archivoDeTercetos,"JNE,");
								break;
			case BH:			fprintf(archivoDeTercetos,"BH,");
								break;
			case BL:			fprintf(archivoDeTercetos,"BL,");
								break;
			case VALOR:			fprintf(archivoDeTercetos,"%xh,",listaDeTercetos[i].y);
								break;
		}

		switch(listaDeTercetos[i].tipoDeZ)
		{
			case INDICE_TS:		fprintf(archivoDeTercetos,"%s",TS[listaDeTercetos[i].z].nombre);
								break;
			case TOKEN:			fprintf(archivoDeTercetos,"%s",identificarTipoToken(listaDeTercetos[i].z));
								break;
			case NRO_TERCETO:	fprintf(archivoDeTercetos,"[%d]",listaDeTercetos[i].z);
								break;
			case IGNORAR:		fprintf(archivoDeTercetos,"_");
								break;
			case JMP:			fprintf(archivoDeTercetos,"JMP");
								break;
			case JZ:			fprintf(archivoDeTercetos,"JZ");
								break;
			case JNZ:			fprintf(archivoDeTercetos,"JNZ");
								break;
			case JG:			fprintf(archivoDeTercetos,"JG");
								break;
			case JGE:			fprintf(archivoDeTercetos,"JGE");
								break;
			case JL:			fprintf(archivoDeTercetos,"JL");
								break;
			case JLE:			fprintf(archivoDeTercetos,"JLE");
								break;
			case JE:			fprintf(archivoDeTercetos,"JE");
								break;
			case JNE:			fprintf(archivoDeTercetos,"JNE");
								break;
			case BH:			fprintf(archivoDeTercetos,"BH");
								break;
			case BL:			fprintf(archivoDeTercetos,"BL");
								break;
			case VALOR:			fprintf(archivoDeTercetos,"%xh",listaDeTercetos[i].z);
								break;
		}

		fprintf(archivoDeTercetos,")\n");
	}

	fclose(archivoDeTercetos);
}

void borrarTerceto(Terceto *t)
{
	t->tipoDeX = IGNORAR;
	t-> tipoDeY = IGNORAR;
	t->tipoDeZ = IGNORAR;
}