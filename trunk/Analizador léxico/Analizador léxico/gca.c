#include "gca.h"
#include "AnalizadorLexico.h"
#include "tokens.h"
#include "GCI.h"
#include "Pila.h"


/*Tabla de símbolos*/
extern tablaDeSimbolos TS[LONG_TS];
extern int cantTokensEnTS;
extern int cantTercetos;
extern Terceto listaDeTercetos[MAX_TERCETOS];
PilaDeInt PilaDeEtiquetas;


/*Archivo Assembler*/
FILE *fileAssembler;


void GenerarAssembler()
{
	GenerarEncabezado();
	 DeclararVariables();
	 GeneracionCodigo();
}

void GenerarEncabezado()
{
	fopen_s(&fileAssembler,"fileAssembler.asm", "w");
	//generar encabezado (ver carpeta)
	fprintf(fileAssembler,".MODEL SMALL\n");
	fprintf(fileAssembler,".386\n");
	fprintf(fileAssembler,".STACK 200h\n\n");
}

void DeclararVariables()
{
	int i;
	fprintf(fileAssembler,".DATA\n");
	for(i=0;i<cantTokensEnTS;i++)
	{
		switch(TS[i].tipo)
		{
		case PR_INT:
		case PR_FLOAT:
			fprintf(fileAssembler, "%s dd ?\n",TS[i].nombre);
			break;
			case PR_STRING:
				fprintf(fileAssembler, "%s db MAXTEXTSIZE dup (?),'$'\n", TS[i].nombre);      
			break;
			case  CTE_ENTERA:
			case  CTE_REAL:
					fprintf(fileAssembler, "%s dd %s\n", TS[i].nombre, TS[i].valor);

			break;
			case CTE_STRING:
				fprintf(fileAssembler, "%s db \"%s\",'$', %d dup (?)\n", TS[i].nombre,  TS[i].valor, MAX_LONG_CTE_STRING);
			break;

		}
	}
	



}

void GeneracionCodigo()
{
	int i;
	//Inicio de la generacion del codigo
	fprintf(fileAssembler,"\n.CODE\n");
	fprintf(fileAssembler,"mov AX, @DATA\n");
	/*fprintf(fileAssembler,"mov DS, AX\n");
	fprintf(fileAssembler,"mov ES, AX\n");
	fprintf(fileAssembler,"finit\n\n");*/
	

	//Inicio del codigo assembles del programa fuente
	
	//Ordeno la Pila de etiquetas
	OrdenarPila(&PilaDeEtiquetas);

	//recorro todos los tercetos
	for(i=0;i<cantTercetos;i++)
	{
		GenerarAssemblerByTerceto(i);
	}



	//Finalizar
	fprintf(fileAssembler,"mov AX, 4C00h\n");
	fprintf(fileAssembler,"int 21h\n");

}

void GenerarAssemblerByTerceto(int idTerceto)
{
	//Creacion de las etiquetas
	if(idTerceto==VerTope(&PilaDeEtiquetas))
	{
		fprintf(fileAssembler, "etiqueta_%d:\n", idTerceto);
		popInt(&PilaDeEtiquetas);
	}



	if(listaDeTercetos[idTerceto].tipoDeX==INDICE_TS)
	{
		//ej. [2](a,_,_) -> subo el valor a el coprocesador
		fprintf(fileAssembler,"FLD %s", TS[listaDeTercetos[idTerceto].x].nombre);
	}
	//VAMOS CON LOS TOKENS!!!!!
	if(listaDeTercetos[idTerceto].tipoDeX==TOKEN)
	{
		//switch(listaDeTercetos[idTerceto].x
	}
	



	}
