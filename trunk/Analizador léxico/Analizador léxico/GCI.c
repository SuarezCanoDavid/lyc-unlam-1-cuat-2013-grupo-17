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
		}

		fprintf(archivoDeTercetos,")\n");
	}

	fclose(archivoDeTercetos);
}