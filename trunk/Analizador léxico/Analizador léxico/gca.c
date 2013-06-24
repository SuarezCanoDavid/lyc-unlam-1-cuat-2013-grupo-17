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

	
	/*while(PilaVacia(&PilaDeEtiquetas)==0)
	{
		printf("%d ",popInt(&PilaDeEtiquetas));
	}*/
	

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
	
	printf("%d  %d \n", idTerceto,VerTope(&PilaDeEtiquetas));
	if(PilaVacia(&PilaDeEtiquetas)==0 && idTerceto==VerTope(&PilaDeEtiquetas))
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
		TercetoTokes(listaDeTercetos[idTerceto].x,  idTerceto);
	}

	//VAMOS CON LOS SALTOS!!!
	if(listaDeTercetos[idTerceto].tipoDeX==JMP || listaDeTercetos[idTerceto].tipoDeX==JZ
			|| listaDeTercetos[idTerceto].tipoDeX==JNZ || listaDeTercetos[idTerceto].tipoDeX==JG
			|| listaDeTercetos[idTerceto].tipoDeX==JGE || listaDeTercetos[idTerceto].tipoDeX==JL
			|| listaDeTercetos[idTerceto].tipoDeX==JLE || listaDeTercetos[idTerceto].tipoDeX==JE
			|| listaDeTercetos[idTerceto].tipoDeX==JNE

			)
	{
		asmSalto(idTerceto);
	}



	}

void asmSalto(int idTerceto)
{
	
	switch(listaDeTercetos[idTerceto].tipoDeX)
	{
		case JMP:			fprintf(fileAssembler, "JMP etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JZ:			fprintf(fileAssembler,"JZ etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JNZ:			fprintf(fileAssembler,"JNZ etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JG:			fprintf(fileAssembler,"JG etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JGE:			fprintf(fileAssembler,"JGE etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JL:			fprintf(fileAssembler,"JL etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JLE:			fprintf(fileAssembler,"JLE etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JE:			fprintf(fileAssembler,"JE etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
		case JNE:			fprintf(fileAssembler,"JNE etiqueta_%d \n",listaDeTercetos[idTerceto].y);
							break;
	}
}
void TercetoTokes(int token, int idTerceto)
{
	switch(token)
		{
	case OP_ASIGNACION:
		asmAsignacion(idTerceto);
		break;
	case OP_CONCATENACION:

		break;
	case OP_DISTINTO:
		asmDistinto();
		break;

	case OP_DIVISION:
		asmDividir();
		break;
	case  OP_IGUAL:
		asmIgual();
		break;
	case OP_MAYOR:
		asmMayor();
		break;
	case OP_MAYOR_IGUAL:
		asmMayorIgual();
		break;
	case OP_MENOR:
		asmMenor();
		break;

	case OP_MENOR_IGUAL:
		asmMenorIgual();
		break;
	case OP_MULTIPLICACION:
		asmMultiplicar();
		break;
	case OP_RESTA:
		asmResta();
		break;

	case OP_SUMA:
		asmSuma();
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

void asmSuma()
{
	fprintf(fileAssembler,"FADDP \n");
	//fprintf(fileAssembler, "FST AUX%d\n", idTerceto);
	//fprintf(fileAssembler, "ffree st(0)\n");
 //   fprintf(fileAssembler, "ffree st(1)\n");

}

void asmResta()
{
	fprintf(fileAssembler,"FSUBP \n");
}

void asmMultiplicar()
{
	fprintf(fileAssembler,"FMULP \n");
}

void asmDividir()
{
	fprintf(fileAssembler,"FDIVP \n");
}

void asmAsignacion(int idTerceto)
{

	//printf("%d - %d - %d   ",listaDeTercetos[idTerceto].tipoDeY,listaDeTercetos[idTerceto].tipoDeZ,listaDeTercetos[idTerceto].z);
	if(listaDeTercetos[idTerceto].tipoDeY==BH
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==1)
	{
		fprintf(fileAssembler,"MOV BH ffh\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==BH
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BH 00h\n");
	}


	if(listaDeTercetos[idTerceto].tipoDeY==BL
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==1)
	{
		fprintf(fileAssembler,"MOV BL ffh\n");
	}
	
	if(listaDeTercetos[idTerceto].tipoDeY==BL
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BL 00h\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==BL
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BL 00h\n");
	}
	if(listaDeTercetos[idTerceto].tipoDeY==INDICE_TS)
	{
		fprintf(fileAssembler,"FSTP %s \n", TS[listaDeTercetos[idTerceto].y].nombre);
	}

	
	
	
}

void asmDistinto()
{
	fprintf(fileAssembler,"FCOMPP \n");
}

void asmIgual()
{
	fprintf(fileAssembler,"FCOMPP \n");
}

void asmMayor()
{
	fprintf(fileAssembler,"FXCH \n");
	fprintf(fileAssembler,"FCOMPP \n");
}

void asmMayorIgual()
{
	fprintf(fileAssembler,"FXCH \n");
	fprintf(fileAssembler,"FCOMPP \n");
}

void asmMenor()
{
	fprintf(fileAssembler,"FXCH \n");
	fprintf(fileAssembler,"FCOMPP \n");
}

void asmMenorIgual()
{
	fprintf(fileAssembler,"FXCH \n");
	fprintf(fileAssembler,"FCOMPP \n");
}

