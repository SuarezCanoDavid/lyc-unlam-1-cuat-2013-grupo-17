#include "gca.h"
#include "AnalizadorLexico.h"
#include "tokens.h"
#include "GCI.h"
#include "Pila.h"
#include <string.h>


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
	adecuarTS();
	GenerarEncabezado();
	DeclararVariables();
	
	GeneracionCodigo();
	GenerarCodigoString();
	//GenerarCodigoInput();
	//GenerarConversiones();
	fprintf(fileAssembler,"\nEND");
}

void GenerarEncabezado()
{
	
	fopen_s(&fileAssembler,"FINAL.ASM", "w");
	
	fprintf(fileAssembler,"include  number.asm\n");
	fprintf(fileAssembler,"include  macros2.asm\n");
	//fprintf(fileAssembler,"include  numbers.asm\n");
	//fprintf(fileAssembler,"include  numbers.asm\n");
	 
	fprintf(fileAssembler,".MODEL SMALL\n");
	fprintf(fileAssembler,".386\n");
	fprintf(fileAssembler,".STACK 200h\n\n");
}

void DeclararVariables()
{
	int i;
	fprintf(fileAssembler,".DATA\n");

	fprintf(fileAssembler,"MAXTEXTSIZE equ 61\n");

	for(i=0;i<cantTokensEnTS;i++)
	{
		switch(TS[i].tipo)
		{
			case PR_INT+PR_FUNCTION:
			case PR_INT:
			case PR_FLOAT+PR_FUNCTION:
			case PR_FLOAT: fprintf(fileAssembler, "%s dd ?\n",TS[i].nombre);
						   break;
			case PR_STRING+PR_FUNCTION:
			case PR_STRING:	fprintf(fileAssembler, "%s db '$',60 dup(?)\n", TS[i].nombre);    
							fprintf(fileAssembler, "%s_long dd 1\n", TS[i].nombre); 
							break;

			case  CTE_REAL:	
			case  CTE_ENTERA: fprintf(fileAssembler, "%s dd %s\n", TS[i].nombre, TS[i].valor);
							  break;

			case CTE_STRING: fprintf(fileAssembler, "%s db \"%s\",'$',%d dup(?)\n", TS[i].nombre,  TS[i].valor, MAX_LONG_CTE_STRING-TS[i].longitud);
							 fprintf(fileAssembler, "%s_long dd %d\n", TS[i].nombre,  TS[i].longitud+1);
							 break;
			
							
		}
	}
	
	//fprintf(fileAssembler,"__MENSAJE     db '$', %d dup (?)\n",MAX_LONG_CTE_STRING);
	fprintf(fileAssembler,"__ENTER          db MAXTEXTSIZE dup(?)\n");
	fprintf(fileAssembler,"__NEW_LINE       db 0Dh,0Ah,'$'\n");
	//fprintf(fileAssembler,"_AUXPrintReal db 14 dup (?), '$'\n");
	fprintf(fileAssembler,"_parteFrac       dd ?\n");
	fprintf(fileAssembler,"_parteEntera     dd ?\n");
	fprintf(fileAssembler,"_mul_10          dd 10.0\n");
	fprintf(fileAssembler,"_x87_round       dw 077fh\n");
	fprintf(fileAssembler,"_x87_env			db 28 dup(?)\n");
	fprintf(fileAssembler,"_x87_stack0		dd ?\n");
	fprintf(fileAssembler,"_x87_stack1		dd ?\n");
	fprintf(fileAssembler,"_x87_stack2		dd ?\n");
	fprintf(fileAssembler,"_x87_stack3		dd ?\n");
	fprintf(fileAssembler,"_x87_stack4		dd ?\n");
	fprintf(fileAssembler,"_x87_stack5		dd ?\n");
	fprintf(fileAssembler,"_x87_stack6		dd ?\n");
	fprintf(fileAssembler,"_x87_stack7		dd ?\n");
	fprintf(fileAssembler,"_cadenaAux		db MAXTEXTSIZE dup(?)\n");
	fprintf(fileAssembler,"_cadenaAux_long  dd 0\n");

	fprintf(fileAssembler,"realAltura_MAIN  dd 0\n");
	
	 
	fprintf(fileAssembler,"strReadString		db MAXTEXTSIZE dup(?)\n");
	 
}

void GeneracionCodigo()
{
	
	int i;
	 
	//Inicio de la generacion del codigo
	fprintf(fileAssembler,"\n.CODE\n");
	fprintf(fileAssembler,"\tmov ax, @DATA\n");
	fprintf(fileAssembler,"\tmov ds, ax\n");
	fprintf(fileAssembler,"\tmov es, ax\n");
    fprintf(fileAssembler,"\tfinit\n");
    fprintf(fileAssembler,"\tfldcw _x87_round\n\n");
	
	
	//Inicio del codigo assembles del programa fuente
	
	vaciarPilaDeInt(&PilaDeEtiquetas);
	//LLeno la Pila de Eriquetas
	LlenarPilaEtiquetas();
	
	for(i = 0; listaDeTercetos[i].x != PR_BEGINPROG; ++i);

	//recorro todos los tercetos
	for(;i<cantTercetos;i++)
	{
		if(GenerarAssemblerByTerceto(i)==1)
			i++;

	}

	//Finalizar
	fprintf(fileAssembler,"\nERROR:\n");
	fprintf(fileAssembler,"\tmov ax, 4C00h\n");
	fprintf(fileAssembler,"\tint 21h\n\n");

	for(i = 0; listaDeTercetos[i].x != PR_BEGINPROG; ++i)
	{
		if(GenerarAssemblerByTerceto(i)==1)
			i++;
	}
}

int GenerarAssemblerByTerceto(int idTerceto)
{
	int valorDevuelto=0;
	
	//Creacion de las etiquetas
	if(contiene(&PilaDeEtiquetas,idTerceto) == TRUE)
	{
		fprintf(fileAssembler, "etiqueta_%d:\n", idTerceto);
	}

	if(listaDeTercetos[idTerceto].tipoDeX==CALL)
	{
		asmCall(idTerceto);

	}
	if(listaDeTercetos[idTerceto].tipoDeX==INDICE_TS)
	{
		//ej. [2](a,_,_) -> subo el valor a el coprocesador
		if(TS[listaDeTercetos[idTerceto].x].tipo != PR_STRING+PR_FUNCTION && TS[listaDeTercetos[idTerceto].x].tipo != PR_STRING)
		{
			fprintf(fileAssembler,"\tfld %s\n", TS[listaDeTercetos[idTerceto].x].nombre);
		}
	}
	//VAMOS CON LOS TOKENS!!!!!
	if(listaDeTercetos[idTerceto].tipoDeX==TOKEN)
	{
		valorDevuelto= TercetoTokes(listaDeTercetos[idTerceto].x,  idTerceto);
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

	//VAMOS CON EL QUERIDO FILTERC
	if(listaDeTercetos[idTerceto].tipoDeX==VAR_FILTERC)
	{
		fprintf(fileAssembler,"\tfldz\n");
	}

	return valorDevuelto;
}

void asmSalto(int idTerceto)
{
	
	switch(listaDeTercetos[idTerceto].tipoDeX)
	{
		case JMP:			fprintf(fileAssembler,"\tjmp etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JZ:			fprintf(fileAssembler,"\tjz etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JNZ:			fprintf(fileAssembler,"\tjnz etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JG:			fprintf(fileAssembler,"\tja etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JGE:			fprintf(fileAssembler,"\tjae etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JL:			fprintf(fileAssembler,"\tjb etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JLE:			fprintf(fileAssembler,"\tjbe etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JE:			fprintf(fileAssembler,"\tje etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
		case JNE:			fprintf(fileAssembler,"\tjne etiqueta_%d\n",listaDeTercetos[idTerceto].y);
							break;
	}
}

int TercetoTokes(int token, int idTerceto)
{
	int valorDevuelto=0;
	switch(token)
		{
	case OP_ASIGNACION:
		asmAsignacion(idTerceto);
		break;
	case OP_CONCATENACION:
			asmConcatenacion(idTerceto);
			valorDevuelto=1;
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
		if(listaDeTercetos[idTerceto].tipoDeY == VAR_FILTERC)
		{
			fprintf(fileAssembler,"\tfld1\n");
		}
		asmSuma();
		break;
	
	case PR_AND:
		asmAnd(idTerceto);
		break;

	case PR_OR:
		asmOr(idTerceto);
		break;
	
	case PR_NOT:
		asmNot(idTerceto);
		break;

	case PR_WPRINT:
		asmWprint(idTerceto);
		break;

	case CALL:
		asmCall(idTerceto);
		break;
	case PR_FUNCTION:
		asmEtiquetaFuncion(idTerceto);
		break;
	case PR_RETURN:
		asmEtiquetaReturn(idTerceto);
		break;
	case PR_SCANF:
		asmScanf(idTerceto);
		break;
	}

	return valorDevuelto;

}

void asmEtiquetaFuncion(int idTerceto)
{
	char aux[MAX_LONG_TOKEN+1]="";
	int i;
	for(i=1; TS[listaDeTercetos[idTerceto].y].nombre[i]!='_';i++)
	{
		aux[i-1]=TS[listaDeTercetos[idTerceto].y].nombre[i];
	}
	aux[i-1]='\0';

	fprintf(fileAssembler, "%s:\n",aux);
	fprintf(fileAssembler, "\tpush ebx\n");
	fprintf(fileAssembler, "\tpush ecx\n");
	fprintf(fileAssembler, "\tfstenv _x87_env\n");
	fprintf(fileAssembler, "\tfstp _x87_stack0\n");
	fprintf(fileAssembler, "\tfstp _x87_stack1\n");
	fprintf(fileAssembler, "\tfstp _x87_stack2\n");
	fprintf(fileAssembler, "\tfstp _x87_stack3\n");
	fprintf(fileAssembler, "\tfstp _x87_stack4\n");
	fprintf(fileAssembler, "\tfstp _x87_stack5\n");
	fprintf(fileAssembler, "\tfstp _x87_stack6\n");
	fprintf(fileAssembler, "\tfstp _x87_stack7\n\n");
}

void asmEtiquetaReturn(int idTerceto)
{
	fprintf(fileAssembler, "\n\tfld _x87_stack7\n");
	fprintf(fileAssembler, "\tfld _x87_stack6\n");
	fprintf(fileAssembler, "\tfld _x87_stack5\n");
	fprintf(fileAssembler, "\tfld _x87_stack4\n");
	fprintf(fileAssembler, "\tfld _x87_stack3\n");
	fprintf(fileAssembler, "\tfld _x87_stack2\n");
	fprintf(fileAssembler, "\tfld _x87_stack1\n");
	fprintf(fileAssembler, "\tfld _x87_stack0\n");
	fprintf(fileAssembler, "\tfldenv _x87_env\n");
	fprintf(fileAssembler, "\tpop ebx\n");
	fprintf(fileAssembler, "\tpop ecx\n");
	fprintf(fileAssembler, "\tret\n\n");
}

void asmCall(int idTerceto)
{
	
	char aux[MAX_LONG_TOKEN+1]="";
	int i;
	for(i=1; TS[listaDeTercetos[idTerceto].y].nombre[i]!='_';i++)
	{
		aux[i-1]=TS[listaDeTercetos[idTerceto].y].nombre[i];
	}
	aux[i-1]='\0';
		
		fprintf(fileAssembler, "\tcall %s\n",aux);

}
void asmConcatenacion(int idTerceto)
{
	//verifico que se pueda realizar la concatenacion
	fprintf(fileAssembler, "\tmov eax, %s_long\n",TS[listaDeTercetos[idTerceto].z].nombre);
	fprintf(fileAssembler, "\tadd eax, %s_long\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler, "\tsub eax, 1\n");
	fprintf(fileAssembler, "\tcmp eax, MAXTEXTSIZE\n");
	fprintf(fileAssembler, "\tjg  ERROR\n");

	//comienzo la concatenacion
	fprintf(fileAssembler, "\tmov _cadenaAux_long , eax\n");
	fprintf(fileAssembler, "\tcld\n");
	fprintf(fileAssembler, "\tmov esi, OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler, "\tmov edi, OFFSET _cadenaAux\n");
	fprintf(fileAssembler, "\tmov ecx, %s_long\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler, "\tsub ecx, 1\n");
	fprintf(fileAssembler, "\trep movsb\n");
	fprintf(fileAssembler, "\tmov esi, OFFSET %s\n",TS[listaDeTercetos[idTerceto].z].nombre);
	fprintf(fileAssembler, "\tmov ecx, %s_long\n", TS[listaDeTercetos[idTerceto].z].nombre);
	fprintf(fileAssembler, "\trep movsb\n");

	fprintf(fileAssembler,"\tmov eax, _cadenaAux_long\n");
	fprintf(fileAssembler,"\tmov %s_long, eax\n",TS[listaDeTercetos[idTerceto+1].y].nombre);
	fprintf(fileAssembler,"\tcld\n");
	fprintf(fileAssembler,"\tmov esi, OFFSET _cadenaAux\n");
	fprintf(fileAssembler,"\tmov edi, OFFSET %s\n",TS[listaDeTercetos[idTerceto+1].y].nombre);
	fprintf(fileAssembler,"\tmov ecx, eax\n");
	fprintf(fileAssembler,"\trep movsb\n");
}

void asmWprint(int idTerceto)
{
	if(TS[listaDeTercetos[idTerceto].y].tipo== PR_STRING ||TS[listaDeTercetos[idTerceto].y].tipo== CTE_STRING ||TS[listaDeTercetos[idTerceto].y].tipo== PR_STRING+PR_FUNCTION)
	{ 	 
		/*
		fprintf(fileAssembler, "\tcld\n");
		fprintf(fileAssembler, "\tmov esi, OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, "\tmov edi, OFFSET __ENTER\n");
		fprintf(fileAssembler, "\tmov ecx, %s_long\n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, "\trep movsb\n");
		fprintf(fileAssembler, "\tcall IMPRIMIR\n");
		
			fprintf(fileAssembler, "\tnewLine 2\n");
		fprintf(fileAssembler, "\tdisplayString  %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
		 */
		
		  fprintf(fileAssembler, "\tmov dx, OFFSET __NEW_LINE\n");
	  fprintf(fileAssembler, "\tmov ah, 9\n");
	  fprintf(fileAssembler, "\tint 21h\n");
		  fprintf(fileAssembler, "\t MOV DX,OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
		 fprintf(fileAssembler, "\t MOV Ah,9 \n");
		fprintf(fileAssembler, "\t INT 21h\n");

		  fprintf(fileAssembler, "\tmov dx, OFFSET __NEW_LINE\n");
	  fprintf(fileAssembler, "\tmov ah, 9\n");
	  fprintf(fileAssembler, "\tint 21h\n");
		 

	}
	else
	{/*
		fprintf(fileAssembler, "\tmov eax, %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, "\tmov _parteFrac, eax\n");
		fprintf(fileAssembler, "\tcall IMPRIMIRNUMERO\n");
		*/
		if(TS[listaDeTercetos[idTerceto].y].tipo== PR_INT ||TS[listaDeTercetos[idTerceto].y].tipo== CTE_ENTERA ||TS[listaDeTercetos[idTerceto].y].tipo== PR_INT+PR_FUNCTION)
		{  
			
		  fprintf(fileAssembler, "\tmov dx, OFFSET __NEW_LINE\n");
	  fprintf(fileAssembler, "\tmov ah, 9\n");
	  fprintf(fileAssembler, "\tint 21h\n");
		 
		fprintf(fileAssembler, "\tdisplayFloat  %s , 0\n",TS[listaDeTercetos[idTerceto].y].nombre);
		
		  fprintf(fileAssembler, "\tmov dx, OFFSET __NEW_LINE\n");
	  fprintf(fileAssembler, "\tmov ah, 9\n");
	  fprintf(fileAssembler, "\tint 21h\n");
		}
		else
		{
			
		  fprintf(fileAssembler, "\tmov dx, OFFSET __NEW_LINE\n");
	  fprintf(fileAssembler, "\tmov ah, 9\n");
	  fprintf(fileAssembler, "\tint 21h\n");
		 
		fprintf(fileAssembler, "\tdisplayFloat  %s , 2\n",TS[listaDeTercetos[idTerceto].y].nombre);
		
		  fprintf(fileAssembler, "\tmov dx, OFFSET __NEW_LINE\n");
	  fprintf(fileAssembler, "\tmov ah, 9\n");
	  fprintf(fileAssembler, "\tint 21h\n");
		}
		
	}
}
void asmScanf(int idTerceto)
{
	/*
	fprintf(fileAssembler, "\tcld\n");
	call readString  
    mov dx,offset strReadString
	*/
	//comienzo la concatenacion-------
	/*fprintf(fileAssembler, "\tmov _cadenaAux_long , eax\n");
	fprintf(fileAssembler, "\tcld\n");
	fprintf(fileAssembler, "\tmov esi, OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler, "\tmov edi, OFFSET _cadenaAux\n");
	fprintf(fileAssembler, "\tmov ecx, %s_long\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler, "\tsub ecx, 1\n");
	fprintf(fileAssembler, "\trep movsb\n");
	fprintf(fileAssembler, "\tmov esi, OFFSET %s\n",TS[listaDeTercetos[idTerceto].z].nombre);
	fprintf(fileAssembler, "\tmov ecx, %s_long\n", TS[listaDeTercetos[idTerceto].z].nombre);
	fprintf(fileAssembler, "\trep movsb\n");
	*/


	/* usar este
	fprintf(fileAssembler,"\t CALL readString\n");
	fprintf(fileAssembler,"\tmov eax, _cadenaAux_long\n");
	fprintf(fileAssembler,"\tmov eax, _cadenaAux_long\n");
	fprintf(fileAssembler,"\tmov %s_long, eax\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler,"\tcld\n");
	fprintf(fileAssembler,"\tmov esi, OFFSET _cadenaAux\n");
	fprintf(fileAssembler,"\tmov edi, OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler,"\tmov ecx, eax\n");
	fprintf(fileAssembler,"\trep movsb\n");
	*/
//	fprintf(fileAssembler,"\nMOV Ah,9 \n");
	//	 fprintf(fileAssembler,"\tINT 21h \n");
	if(TS[listaDeTercetos[idTerceto].y].tipo== PR_STRING ||TS[listaDeTercetos[idTerceto].y].tipo== CTE_STRING ||TS[listaDeTercetos[idTerceto].y].tipo== PR_STRING+PR_FUNCTION)
	{
		fprintf(fileAssembler,"\tmov dx, OFFSET __NEW_LINE\n");
		fprintf(fileAssembler,"\tmov ah, 9\n");
		fprintf(fileAssembler,"\tint 21h\n");
		fprintf(fileAssembler,"\t getString %s \n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler,"\tmov dx, OFFSET __NEW_LINE\n");
		fprintf(fileAssembler,"\tmov ah, 9\n");
		fprintf(fileAssembler,"\tint 21h\n");
	}
	else
	{
			fprintf(fileAssembler,"\tmov dx, OFFSET __NEW_LINE\n");
		fprintf(fileAssembler,"\tmov ah, 9\n");
		fprintf(fileAssembler,"\tint 21h\n");
		fprintf(fileAssembler,"\t getFloat %s \n",TS[listaDeTercetos[idTerceto].y].nombre);
			fprintf(fileAssembler,"\tmov dx, OFFSET __NEW_LINE\n");
		fprintf(fileAssembler,"\tmov ah, 9\n");
		fprintf(fileAssembler,"\tint 21h\n");
	//	fprintf(fileAssembler,"\t CALL read  \n");
	//	fprintf(fileAssembler,"\tmov eax,  _cadenaAux\n"); 
	//		fprintf(fileAssembler,"\tmov   %s ,  eax \n",TS[listaDeTercetos[idTerceto].y].nombre); 
		//fprintf(fileAssembler, "\t mov _integerAux,al\n");
	}
	//fprintf(fileAssembler,"\t getString  \n");
	


}

void asmNot(int idTerceto)
{
	if(listaDeTercetos[idTerceto].tipoDeY==BH)
	{
		fprintf(fileAssembler,"\tnot bh\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==CH)
	{
		fprintf(fileAssembler,"\tnot ch\n");
	}
}	

void asmOr(int idTerceto)
{
	if(listaDeTercetos[idTerceto].tipoDeY==BH)
	{
		fprintf(fileAssembler,"\tor bh, bl\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==CH)
	{
		fprintf(fileAssembler,"\tor ch, cl\n");
	}
}

void asmAnd(int idTerceto)
{
	if(listaDeTercetos[idTerceto].tipoDeY==BH)
	{
		fprintf(fileAssembler,"\tand bh, bl\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==CH)
	{
		fprintf(fileAssembler,"\tand ch, cl\n");
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
	fprintf(fileAssembler,"\tfaddp\n");
}

void asmResta()
{
	fprintf(fileAssembler,"\tfsubp\n");
}

void asmMultiplicar()
{
	fprintf(fileAssembler,"\tfmulp\n");
}

void asmDividir()
{
	fprintf(fileAssembler,"\tfdivp\n");
}

void asmAsignacion(int idTerceto)
{
	switch(listaDeTercetos[idTerceto].tipoDeY)
	{
		case BH: switch(listaDeTercetos[idTerceto].z)
				 {
					case 0: fprintf(fileAssembler,"\tmov bh, 000h\n");
							break;
					case 1: fprintf(fileAssembler,"\tmov bh, 0ffh\n");
							break;
				 }
				 break;
		case BL: switch(listaDeTercetos[idTerceto].z)
				 {
					case 0: fprintf(fileAssembler,"\tmov bl, 000h\n");
							break;
					case 1: fprintf(fileAssembler,"\tmov bl, 0ffh\n");
							break;
				 }
				 break;
		case CH: switch(listaDeTercetos[idTerceto].z)
				 {
					case 0: fprintf(fileAssembler,"\tmov ch, 000h\n");
							break;
					case 1: fprintf(fileAssembler,"\tmov ch, 0ffh\n");
							break;
				 }
				 break;
		case CL: switch(listaDeTercetos[idTerceto].z)
				 {
					case 0: fprintf(fileAssembler,"\tmov cl, 000h\n");
							break;
					case 1: fprintf(fileAssembler,"\tmov cl, 0ffh\n");
							break;
				 }
				 break;
		case INDICE_TS: switch(TS[listaDeTercetos[idTerceto].y].tipo)
						{
							case PR_INT+PR_FUNCTION:
							case PR_INT:	 if(listaDeTercetos[idTerceto].tipoDeZ == INDICE_TS)
											 {
											 	 fprintf(fileAssembler,"\tfld %s\n",TS[listaDeTercetos[idTerceto].z].nombre);
											 }
											 fprintf(fileAssembler,"\tfistp %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
											 fprintf(fileAssembler,"\tfild %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
											 fprintf(fileAssembler,"\tfstp %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
											 break; 
							//case CTE_REAL:
							//case CTE_ENTERA:
							case PR_FLOAT+PR_FUNCTION:
							case PR_FLOAT:	 if(listaDeTercetos[idTerceto].tipoDeZ == INDICE_TS)
											 {
												 fprintf(fileAssembler,"\tfld %s\n",TS[listaDeTercetos[idTerceto].z].nombre);
											 }
											 fprintf(fileAssembler,"\tfstp %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
											 break;
							//case CTE_STRING:
							case PR_STRING+PR_FUNCTION:
							case PR_STRING:  if(listaDeTercetos[idTerceto].tipoDeZ == INDICE_TS)
											 {
												 fprintf(fileAssembler,"\tmov eax, %s_long\n",TS[listaDeTercetos[idTerceto].z].nombre);
												 fprintf(fileAssembler,"\tmov %s_long, eax\n", TS[listaDeTercetos[idTerceto].y].nombre);
												 fprintf(fileAssembler,"\tcld\n");
												 fprintf(fileAssembler,"\tmov esi,OFFSET %s\n",TS[listaDeTercetos[idTerceto].z].nombre);
												 fprintf(fileAssembler,"\tmov edi,OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
												 fprintf(fileAssembler,"\tmov ecx, eax\n");
												 fprintf(fileAssembler,"\trep movsb\n");
											 }

											 if(listaDeTercetos[idTerceto].tipoDeZ == NRO_TERCETO && listaDeTercetos[listaDeTercetos[idTerceto].z].tipoDeX == INDICE_TS)
											 {
												 fprintf(fileAssembler,"\tmov eax, %s_long\n",TS[listaDeTercetos[listaDeTercetos[idTerceto].z].x].nombre);
												 fprintf(fileAssembler,"\tmov %s_long, eax\n", TS[listaDeTercetos[idTerceto].y].nombre);
												 fprintf(fileAssembler,"\tcld\n");
												 fprintf(fileAssembler,"\tmov esi,OFFSET %s\n",TS[listaDeTercetos[listaDeTercetos[idTerceto].z].x].nombre);
												 fprintf(fileAssembler,"\tmov edi,OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
												 fprintf(fileAssembler,"\tmov ecx, eax\n");
												 fprintf(fileAssembler,"\trep movsb\n");
											 }
											 break;
						}
						break;
	}
}

void asmDistinto()
{
	fprintf(fileAssembler,"\tfcompp\n");
	fprintf(fileAssembler,"\tfstsw ax\n");
	fprintf(fileAssembler,"\tsahf\n");
}

void asmIgual()
{
	fprintf(fileAssembler,"\tfcompp\n");
	fprintf(fileAssembler,"\tfstsw ax\n");
	fprintf(fileAssembler,"\tsahf\n");
}

void asmMayor()
{
	fprintf(fileAssembler,"\tfxch\n");
	fprintf(fileAssembler,"\tfcompp\n");
	fprintf(fileAssembler,"\tfstsw ax\n");
	fprintf(fileAssembler,"\tsahf\n");
}

void asmMayorIgual()
{
	fprintf(fileAssembler,"\tfxch\n");
	fprintf(fileAssembler,"\tfcompp\n");
	fprintf(fileAssembler,"\tfstsw ax\n");
	fprintf(fileAssembler,"\tsahf\n");
}

void asmMenor()
{
	fprintf(fileAssembler,"\tfxch\n");
	fprintf(fileAssembler,"\tfcompp\n");
	fprintf(fileAssembler,"\tfstsw ax\n");
	fprintf(fileAssembler,"\tsahf\n");
}

void asmMenorIgual()
{
	fprintf(fileAssembler,"\tfxch\n");
	fprintf(fileAssembler,"\tfcompp\n");
	fprintf(fileAssembler,"\tfstsw ax\n");
	fprintf(fileAssembler,"\tsahf\n");
}

void GenerarCodigoInput()
{
	//Usar este
	 fprintf(fileAssembler, "\nproc read\n");
            fprintf(fileAssembler, "\tmov dx,offset _cadenaAux\n");
            //fprintf(fileAssembler, "\tmov ah,09h\n");
			fprintf(fileAssembler, "\tmov ah,0Ah \n");
            fprintf(fileAssembler, "\tint 21h\n");
			/*
            fprintf(fileAssembler, "\tmov ah,01h\n");
            fprintf(fileAssembler, "\tint 21h\n");
            fprintf(fileAssembler, "\tsub eax,48\n");
            fprintf(fileAssembler, "\t mov _integerAux, eax\n");
			*/
            fprintf(fileAssembler, "\t ret\n");
        fprintf(fileAssembler, "\t endp\n");




	///fin valido
	//Rutina de entrada de pantalla
		/*
	 fprintf(fileAssembler, "\nreadString proc near\n");
    fprintf(fileAssembler, "\t mov si,offset _cadenaAux \n");
    fprintf(fileAssembler, "\tloop_Read_String:\n");
	 fprintf(fileAssembler, "\ncall readCharacter\n");
    fprintf(fileAssembler, "\t mov [si],al \n");
    fprintf(fileAssembler, "\t inc si\n");
	 fprintf(fileAssembler, "\n cmp al,0dh\n");
    fprintf(fileAssembler, "\t jne loop_Read_String\n");
    fprintf(fileAssembler, "\tmov [si],'$\n");
	 fprintf(fileAssembler, "\tret\n");
    fprintf(fileAssembler, "\tendp\n");

	/*
	readString proc near 
    
    mov si,offset strReadString    
loop_Read_String:
    call readCharacter      
    mov [si],al
    inc si
    cmp al,0dh
    jne loop_Read_String
    mov [si],'$'
    ret
endp


	//Leer caracter

	 fprintf(fileAssembler, "\nreadCharacter proc near\n");
    fprintf(fileAssembler, "\tmov ah,00 \n");
    fprintf(fileAssembler, "\tint 16h\n");
	 fprintf(fileAssembler, "\n ret\n");
    fprintf(fileAssembler, "\tendp\n");
	/*
	readCharacter proc near
    mov ah,00
    int 16h
    ret
endp  
*/
	
	
}
void GenerarCodigoString()
{
	
	//Rutina de salida a pantalla
    fprintf(fileAssembler, "\nIMPRIMIR:\n");
    fprintf(fileAssembler, "\tmov dx, OFFSET __ENTER\n");
    fprintf(fileAssembler, "\tmov ah, 9\n");
    fprintf(fileAssembler, "\tint 21h\n");
	fprintf(fileAssembler, "\tmov dx, OFFSET __NEW_LINE\n");
    fprintf(fileAssembler, "\tmov ah, 9\n");
    fprintf(fileAssembler, "\tint 21h\n");
    fprintf(fileAssembler, "\tret\n");
     
	
	fprintf(fileAssembler, "\nIMPRIMIRNUMERO:\n");
    fprintf(fileAssembler, "\tfld _parteFrac\n");
    fprintf(fileAssembler, "\tfistp _parteEntera\n");
    fprintf(fileAssembler, "\tfld _parteFrac\n");
    fprintf(fileAssembler, "\tfild _parteEntera\n");
    fprintf(fileAssembler, "\tfsubp\n");
    fprintf(fileAssembler, "\tfstp _parteFrac\n");


    fprintf(fileAssembler, "\n\tmov ecx, 0\n");
    fprintf(fileAssembler, "\tmov ebx, 10\n");
    fprintf(fileAssembler, "\tmov eax, _parteEntera\n");
	fprintf(fileAssembler, "et1:\n");
    fprintf(fileAssembler, "\tmov edx, 0\n");
    fprintf(fileAssembler, "\tdiv ebx\n");
    fprintf(fileAssembler, "\tpush dx\n");
    fprintf(fileAssembler, "\tinc ecx\n");
    fprintf(fileAssembler, "\tcmp eax, 0\n");
    fprintf(fileAssembler, "\tjne et1\n");


    fprintf(fileAssembler, "\n\tmov bx, 0\n");
	fprintf(fileAssembler, "et2:\n");
    fprintf(fileAssembler, "\tpop dx\n");
    fprintf(fileAssembler, "\tadd dl, 030h\n");
    fprintf(fileAssembler, "\tmov [__ENTER+bx], dl\n");
    fprintf(fileAssembler, "\tinc bx\n");
    fprintf(fileAssembler, "\tloop et2\n");

    
    fprintf(fileAssembler, "\n\tfld _parteFrac\n");
    fprintf(fileAssembler, "\tfldz\n");
    fprintf(fileAssembler, "\tfcompp\n");
    fprintf(fileAssembler, "\tfstsw ax\n");
    fprintf(fileAssembler, "\tsahf\n");
    fprintf(fileAssembler, "\tje fin\n");

    fprintf(fileAssembler, "\n\tmov [__ENTER+bx], '.'\n");
    fprintf(fileAssembler, "\tinc bx\n");
    
	fprintf(fileAssembler, "\n\tmov cl, 60\n");
	fprintf(fileAssembler, "\tsub cl, bl\n");

	fprintf(fileAssembler, "\tmov ch, 0ffh\n");
	fprintf(fileAssembler, "\net3:\n");
    fprintf(fileAssembler, "\tfld _parteFrac\n");
    fprintf(fileAssembler, "\tfmul _mul_10\n");
    fprintf(fileAssembler, "\tfst _parteFrac\n");
    fprintf(fileAssembler, "\tfistp _parteEntera\n");
    fprintf(fileAssembler, "\tmov edx, _parteEntera\n");
    fprintf(fileAssembler, "\tadd dl, 030h\n");
    fprintf(fileAssembler, "\tmov [__ENTER+bx], dl\n");
    fprintf(fileAssembler, "\tinc bx\n");
	fprintf(fileAssembler, "\tdec cl\n");
    fprintf(fileAssembler, "\tfld _parteFrac\n");
    fprintf(fileAssembler, "\tfild _parteEntera\n");
    fprintf(fileAssembler, "\tfsubp\n");
    fprintf(fileAssembler, "\tfst _parteFrac\n");
    fprintf(fileAssembler, "\tfldz\n");
    fprintf(fileAssembler, "\tfcompp\n");
    fprintf(fileAssembler, "\tfstsw ax\n");
    fprintf(fileAssembler, "\tsahf\n");
    fprintf(fileAssembler, "\tjne et4\n");
	fprintf(fileAssembler, "\tmov ch, 000h\n");
	fprintf(fileAssembler, "et4:\n");
	fprintf(fileAssembler, "\tand ch, cl\n");
	fprintf(fileAssembler, "\tjz fin\n");
    fprintf(fileAssembler, "\tjmp et3\n");


	fprintf(fileAssembler, "\nfin:\n");
    fprintf(fileAssembler, "\tmov [__ENTER+bx], '$'\n");
    fprintf(fileAssembler, "\tcall IMPRIMIR\n");
    fprintf(fileAssembler, "\tret\n");
}
void GenerarConversiones()
{
	fprintf(fileAssembler, "\n;*******************************************************\n");
 fprintf(fileAssembler, "\t;=============== Convertir numero a cadena ===============\n");
 fprintf(fileAssembler, "\t; Parametros\n");
 fprintf(fileAssembler, "\t; ax: valor\n");
 fprintf(fileAssembler, "\t; bx: donde guardar la cadena final\n");
 fprintf(fileAssembler, "\t; Retorna\n");
 fprintf(fileAssembler, "\t; cadena\n");
 fprintf(fileAssembler, "\titoa proc near\n");
   fprintf(fileAssembler, "\txor cx,cx  ;CX = 0\n");
 
  fprintf(fileAssembler, "\t itoa_1:\n");
   fprintf(fileAssembler, "\tcmp ax,0   ; El ciclo itoa_1 extrae los digitos del\n");
   fprintf(fileAssembler, "\tje itoa_2  ; menos al mas significativo de AX y los\n");
              fprintf(fileAssembler, "\t; guarda en el stack. Al finalizar el \n");
   fprintf(fileAssembler, "\txor dx,dx  ; ciclo el digito mas significativo esta\n");
   fprintf(fileAssembler, "\tpush bx    ; arriba del stack.\n");
   fprintf(fileAssembler, "\tmov bx,10  ; CX contiene el numero de digitos\n");
   fprintf(fileAssembler, "\tdiv bx\n");
   fprintf(fileAssembler, "\tpop bx\n");
   fprintf(fileAssembler, "\tpush dx\n");
   fprintf(fileAssembler, "\tinc cx\n");
   fprintf(fileAssembler, "\tjmp itoa_1\n");

   fprintf(fileAssembler, "\titoa_2:\n");
   fprintf(fileAssembler, "\tcmp cx,0    ; Esta seccion maneja el caso cuando\n");
   fprintf(fileAssembler, "\tja itoa_3   ; el numero a convertir (AX) es 0.\n");
   fprintf(fileAssembler, "\tmov ax,'0'  ; En este caso, el ciclo anterior\n");
   fprintf(fileAssembler, "\tmov [bx],ax ; no guarda valores en el stack y\n");
   fprintf(fileAssembler, "\tinc bx      ; CX tiene el valor 0\n");
   fprintf(fileAssembler, "\tjmp itoa_4\n");

   fprintf(fileAssembler, "\titoa_3:\n");
   fprintf(fileAssembler, "\tpop ax      ; Extraemos los numero del stack\n");
   fprintf(fileAssembler, "\tadd ax,30h  ; lo pasamos a su valor ascii\n");
   fprintf(fileAssembler, "\tmov [bx],ax ; lo guardamos en la cadena final\n");
   fprintf(fileAssembler, "\tinc bx\n");
   fprintf(fileAssembler, "\tloop itoa_3\n");

   fprintf(fileAssembler, "\titoa_4:\n");
   fprintf(fileAssembler, "\tmov ax,'$'  ; terminar cadena con '$' para \n");
   fprintf(fileAssembler, "\tmov [bx],ax ; imprimirla con la INT21h/AH=9\n");
   fprintf(fileAssembler, "\tret\n");
 fprintf(fileAssembler, "\titoa endp \n");
 fprintf(fileAssembler, "\t;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

 fprintf(fileAssembler, "\tatoi    proc\n");
 fprintf(fileAssembler, "\tlocals\n");

 fprintf(fileAssembler, "\t;This procedure converts a string to a double word sized integer\n");
 fprintf(fileAssembler, "\t;memory variable. It works using the following methods:\n");
 fprintf(fileAssembler, "\t;1.    If a negative sign has been entered, a value of -1 is stored\n");
 fprintf(fileAssembler, "\t;      for later multiplication with the result.\n");
 fprintf(fileAssembler, "\t;2.    By a process of subtracting 30h from each digit, then adding this\n");
 fprintf(fileAssembler, "\t;      value to the previous accumulation and multiplying the accumulation\n");
 fprintf(fileAssembler, "\t;      by 10, the number is converted from ascii to integer.\n");

                         fprintf(fileAssembler, "\tpushad\n");
                         fprintf(fileAssembler, "\tpush    edi\n");
                         fprintf(fileAssembler, "\tpush    esi\n");

                         fprintf(fileAssembler, "\tpush    bx\n");

                        fprintf(fileAssembler, "\t;getIntegerString      TempStr\n");
                        fprintf(fileAssembler, "\t;lea    si, tempstr\n");

                         fprintf(fileAssembler, "\tmov    edi, 0\n");
                         fprintf(fileAssembler, "\tmov    edx, 0\n");
                         fprintf(fileAssembler, "\tmov    eax, 0\n");
                         fprintf(fileAssembler, "\tmov    ebx, 0\n");

 fprintf(fileAssembler, "\t@@cycle:                cmp    byte ptr [si][bx], '$'\n");
                         fprintf(fileAssembler, "\tje      @@done10\n");

                         fprintf(fileAssembler, "\tcmp    byte ptr[si][bx], '-'\n");
                         fprintf(fileAssembler, "\tjne    @@skip10\n");
                         fprintf(fileAssembler, "\tor      edi, -1\n");
                         fprintf(fileAssembler, "\tjmp    @@notAScii\n");

 fprintf(fileAssembler, "\t@@skip10:              or      edi, 1\n");
 fprintf(fileAssembler, "\t@@skip11:              mov    edx, 0\n");
   fprintf(fileAssembler, "\tImul    IntegerTen\n");

                         fprintf(fileAssembler, "\tcmp    edx, 0\n");
                         fprintf(fileAssembler, "\tje     @@InRange\n");
                         fprintf(fileAssembler, "\tMOV    DX,OFFSET TooLarge\n");
                         fprintf(fileAssembler, "\tMOV    Ah,9 \n");
                         fprintf(fileAssembler, "\tINT    21h       \n");                 
                         fprintf(fileAssembler, "\tmov    eax, 0\n");
                         fprintf(fileAssembler, "\tjmp    @@done10\n");

 fprintf(fileAssembler, "\t@@InRange:              movsx  ecx, byte ptr[si][bx]\n");
                         fprintf(fileAssembler, "\tadd    eax, ecx\n");
                         fprintf(fileAssembler, "\tcmp    ecx, 30h\n");
                         fprintf(fileAssembler, "\tjl      @@notAscii\n");
                         fprintf(fileAssembler, "\tsub    eax, 30h\n");
 fprintf(fileAssembler, "\t@@notAscii:            inc    bx\n");
                         fprintf(fileAssembler, "\tjmp    @@cycle\n");

 fprintf(fileAssembler, "\t@@done10:              mov    edx, 0\n");
                         fprintf(fileAssembler, "\timul    edi\n");

 fprintf(fileAssembler, "\tpop    bx\n");
                         fprintf(fileAssembler, "\tmov    [bx], eax\n");

                         fprintf(fileAssembler, "\tpop    esi\n");
                         fprintf(fileAssembler, "\tpop    edi\n");
                         fprintf(fileAssembler, "\tpopad\n");
                         fprintf(fileAssembler, "\tret\n");

 fprintf(fileAssembler, "\tatoi    endp\n");


 fprintf(fileAssembler, "\t;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

 fprintf(fileAssembler, "\t;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

 fprintf(fileAssembler, "\tAtoF    proc\n");
 fprintf(fileAssembler, "\tlocals\n");

 fprintf(fileAssembler, "\t;This procedure converts a string to a double word sized floating point\n");
 fprintf(fileAssembler, "\t;memory variable. It works using the following methods:\n");
 fprintf(fileAssembler, "\t;l.    First the string is examined for the number of places after the\n");
 fprintf(fileAssembler, "\t;      decimal.  This number is stored in the variable places.  The decimal\n");
 fprintf(fileAssembler, "\t;      point is deleted from the string so an integer remains.\n");
 fprintf(fileAssembler, "\t;2.    If a negative sign has been entered, a value of -1 is stored\n");
 fprintf(fileAssembler, "\t;      for later multiplication with the result.\n");
 fprintf(fileAssembler, "\t;3.    By a process of subtracting 30h from each digit, then adding this\n");
 fprintf(fileAssembler, "\t;      value to the previous accumulation and multiplying the accumulation\n");
 fprintf(fileAssembler, "\t;      by 10, the number is converted from ascii to integer.\n");
 fprintf(fileAssembler, "\t;4.    The integer value from step 3 above is loaded into the floating\n");
 fprintf(fileAssembler, "\t;      point processor and divided by 10 as many times as there were\n");
 fprintf(fileAssembler, "\t;      places entered by the user.\n");


                         fprintf(fileAssembler, "\tpushad\n");
                         fprintf(fileAssembler, "\tpush    edi\n");
                         fprintf(fileAssembler, "\tpush    esi\n");

                         fprintf(fileAssembler, "\tpush    bx\n");
                
                         fprintf(fileAssembler, "\t;getFloatString      tempStr\n");
                         fprintf(fileAssembler, "\t;lea    si, tempStr\n");

                         fprintf(fileAssembler, "\tmov    bx, 0\n");

 fprintf(fileAssembler, "\t@@cycle12:              cmp    byte ptr [bx][si], '$'\n");
                         fprintf(fileAssembler, "\tjne    @@around2\n");
                         fprintf(fileAssembler, "\tmov    places, 0\n");
                         fprintf(fileAssembler, "\tjmp    @@skip2\n");
 fprintf(fileAssembler, "\t@@around2:              cmp    byte ptr [bx][si], '.'\n");
                         fprintf(fileAssembler, "\tje      @@done30\n");
                         fprintf(fileAssembler, "\tinc    bx\n");
                         fprintf(fileAssembler, "\tjmp    @@cycle12\n");

 fprintf(fileAssembler, "\t@@done30:              mov    places, 0\n");
 fprintf(fileAssembler, "\t@@skip:                inc    bx\n");
                         fprintf(fileAssembler, "\tmov    al, byte ptr [bx][si]\n");
                         fprintf(fileAssembler, "\tmov    byte ptr [bx-1][si], al\n");
                         fprintf(fileAssembler, "\tcmp    byte ptr [bx][si], '$'\n");
                         fprintf(fileAssembler, "\tje      @@skip2\n");
                         fprintf(fileAssembler, "\tinc    places\n");
                         fprintf(fileAssembler, "\tjmp    @@skip\n");

 fprintf(fileAssembler, "\t@@skip2:                mov    edi, 0\n");
                         fprintf(fileAssembler, "\tmov    edx, 0\n");
                         fprintf(fileAssembler, "\tmov    eax, 0\n");
                         fprintf(fileAssembler, "\tmov    bx, 0\n");

 fprintf(fileAssembler, "\t@@cycle:                cmp    byte ptr [si][bx], '$'\n");
                         fprintf(fileAssembler, "\tje      @@done10\n");

 fprintf(fileAssembler, "\tcmp    byte ptr [si][bx], '-'\n");
                         fprintf(fileAssembler, "\tjne    @@skip10\n");
                         fprintf(fileAssembler, "\tor      edi, -1\n");

                         fprintf(fileAssembler, "\tjmp    @@notAscii\n");

 fprintf(fileAssembler, "\t@@skip10:              or      edi, 1\n");
 fprintf(fileAssembler, "\t@@skip11:              mov    edx, 0\n");
                         fprintf(fileAssembler, "\tImul    IntegerTen\n");
                         fprintf(fileAssembler, "\tcmp    edx, 0\n");
                         fprintf(fileAssembler, "\tje      @@InRange\n");
                         fprintf(fileAssembler, "\tMOV    DX,OFFSET TooLarge\n");
                         fprintf(fileAssembler, "\tMOV    Ah,9 \n");
                         fprintf(fileAssembler, "\tINT    21h    \n");
                         fprintf(fileAssembler, "\tmov    eax, 0\n");
                         fprintf(fileAssembler, "\tjmp    @@done10\n");
 fprintf(fileAssembler, "\t@@InRange:              movsx  ecx, byte ptr[si][bx]\n");
                         fprintf(fileAssembler, "\tadd    eax, ecx\n");
                         fprintf(fileAssembler, "\tcmp    ecx, 30h\n");
                       fprintf(fileAssembler, "\tjl      @@notAscii\n");
                         fprintf(fileAssembler, "\tsub    eax, 30h\n");
 fprintf(fileAssembler, "\t@@notASCii:            inc    bx\n");
                         fprintf(fileAssembler, "\tjmp    @@cycle\n");

 fprintf(fileAssembler, "\t@@done10:              mov    edx, 0\n");
                         fprintf(fileAssembler, "\timul    edi\n");
                         fprintf(fileAssembler, "\tmov    int1, eax\n");
                         fprintf(fileAssembler, "\tfinit\n");
                         fprintf(fileAssembler, "\tfild    int1\n");

                         fprintf(fileAssembler, "\tmov    cx, 0\n");
 fprintf(fileAssembler, "\t@@cycle2:cmp            cx,    places\n");
                         fprintf(fileAssembler, "\tje      @@done\n");
                         fprintf(fileAssembler, "\tfld    realTen\n");
                       fprintf(fileAssembler, "\tfdivp\n");
                         fprintf(fileAssembler, "\tinc    cx\n");
                         fprintf(fileAssembler, "\tjmp    @@cycle2\n");

 fprintf(fileAssembler, "\t@@done:                pop    bx\n");
                         fprintf(fileAssembler, "\tfstp    dword ptr[bx]\n");

                         fprintf(fileAssembler, "\tfwait\n");

                         fprintf(fileAssembler, "\tpop    esi\n");
                         fprintf(fileAssembler, "\tpop    edi\n");
                         fprintf(fileAssembler, "\tpopad\n");
                         fprintf(fileAssembler, "\tret\n");

 fprintf(fileAssembler, "\tAtoF    endp\n");
}

void adecuarTS()
{
	int contadorCte = 0;
	int i;

	for(i = 0; i < cantTokensEnTS; ++i)
	{
		if(TS[i].tipo == CTE_ENTERA || TS[i].tipo == CTE_REAL || TS[i].tipo == CTE_STRING)
		{
			sprintf_s(TS[i].nombre,MAX_LONG_TOKEN+1,"_cte%d",contadorCte++);
		}
		
		if(TS[i].tipo==PR_INT ||TS[i].tipo==PR_STRING ||TS[i].tipo==PR_FLOAT)
		{
			char aux[MAX_LONG_TOKEN+1]="_";
			strcat_s(aux,MAX_LONG_TOKEN,TS[i].ambito);
			strcat_s(aux,MAX_LONG_TOKEN,"_");
			strcat_s(aux,MAX_LONG_TOKEN,TS[i].nombre);

			sprintf_s(TS[i].nombre,MAX_LONG_TOKEN+1,"%s",aux);

		}
		if(TS[i].tipo-PR_INT == PR_FUNCTION || TS[i].tipo-PR_FLOAT == PR_FUNCTION || TS[i].tipo-PR_STRING == PR_FUNCTION)
		{
			char aux1[MAX_LONG_TOKEN+1]="_";
			strcat_s(aux1,MAX_LONG_TOKEN,TS[i].nombre);
			strcat_s(aux1,MAX_LONG_TOKEN,"_");
			strcat_s(aux1,MAX_LONG_TOKEN,"RETURN");

			sprintf_s(TS[i].nombre,MAX_LONG_TOKEN+1,"%s",aux1);
		}

	

	}
}