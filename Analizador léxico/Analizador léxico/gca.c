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
	fopen_s(&fileAssembler,"final.asm", "w");
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
	int i,r;
	//Inicio de la generacion del codigo
	fprintf(fileAssembler,"\n.CODE\n");
	fprintf(fileAssembler,"mov AX, @DATA\n");
	/*fprintf(fileAssembler,"mov DS, AX\n");
	fprintf(fileAssembler,"mov ES, AX\n");
	fprintf(fileAssembler,"finit\n\n");*/
	

	//Inicio del codigo assembles del programa fuente
	
	vaciarPilaDeInt(&PilaDeEtiquetas);
	//LLeno la Pila de Eriquetas
	LlenarPilaEtiquetas();
	
	//Ordeno la Pila de etiquetas
	OrdenarPila(&PilaDeEtiquetas);

	



	//recorro todos los tercetos
	for(i=0;i<cantTercetos;i++)
	{
		GenerarAssemblerByTerceto(i);
	}

	//while(PilaVacia(&PilaDeEtiquetas)==0)
	//{
	//	printf("%d ",popInt(&PilaDeEtiquetas));
	//}


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

		while(idTerceto==VerTope(&PilaDeEtiquetas))
		{
			popInt(&PilaDeEtiquetas);
		}
	}



	if(listaDeTercetos[idTerceto].tipoDeX==INDICE_TS)
	{
		//ej. [2](a,_,_) -> subo el valor a el coprocesador
		fprintf(fileAssembler,"FLD %s \n", TS[listaDeTercetos[idTerceto].x].nombre);
	}
	//VAMOS CON LOS TOKENS!!!!!
	if(listaDeTercetos[idTerceto].tipoDeX==TOKEN)
	{
		TercetoTokes(listaDeTercetos[idTerceto].x,idTerceto);
	}
	



	}


void TercetoTokes(int token, int idTerceto)
{
	switch(token)
		{
	case OP_ASIGNACION:

		break;
	case OP_CONCATENACION:

		break;
	case OP_DISTINTO:

		break;

	case OP_DIVISION:

		break;
	case  OP_IGUAL:

		break;
	case OP_MAYOR:

		break;
	case OP_MAYOR_IGUAL:

		break;
	case OP_MENOR:

		break;

	case OP_MENOR_IGUAL:

		break;
	case OP_MULTIPLICACION:

		break;
	case OP_RESTA:

		break;

	case OP_SUMA:
		asmSuma(idTerceto);
		break;

	}

}

void LlenarPilaEtiquetas()
{
	int i;
	for(i=0;i<cantTercetos;i++)
	{
		if(listaDeTercetos[i].tipoDeX==JMP || listaDeTercetos[i].tipoDeX==JZ
			|| listaDeTercetos[i].tipoDeX==JNZ || listaDeTercetos[i].tipoDeX==JG
			|| listaDeTercetos[i].tipoDeX==JGE || listaDeTercetos[i].tipoDeX==JL
			|| listaDeTercetos[i].tipoDeX==JLE || listaDeTercetos[i].tipoDeX==JE
			|| listaDeTercetos[i].tipoDeX==JNE

			)
		{
			pushInt(listaDeTercetos[i].y,&PilaDeEtiquetas);
			
		}
	}

}
////////////////////////////
//OPERACIONES///////////////
////////////////////////////

//SUMA

void asmSuma(int idTerceto)
{
	fprintf(fileAssembler,"FADD \n");
	fprintf(fileAssembler, "FST AUX%d\n", idTerceto);
	fprintf(fileAssembler, "ffree st(0)\n");
    fprintf(fileAssembler, "ffree st(1)\n");
}


