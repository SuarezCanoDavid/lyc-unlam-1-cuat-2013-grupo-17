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
	 GenerarCodigoString();
	 fprintf(fileAssembler,"\nEND;");
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

	fprintf(fileAssembler,"MAXTEXTSIZE equ 61\n");

	for(i=0;i<cantTokensEnTS;i++)
	{
		switch(TS[i].tipo)
		{
			case PR_INT:
			case PR_FLOAT: fprintf(fileAssembler, "%s dd ?\n",TS[i].nombre);
						   break;

			case PR_STRING:	fprintf(fileAssembler, "%s db '$',60 dup(?)\n", TS[i].nombre);    
							fprintf(fileAssembler, "%s_long dd 1\n", TS[i].nombre); 
							break;

			case  CTE_REAL:	
			case  CTE_ENTERA: fprintf(fileAssembler, "%s dd %s\n", TS[i].nombre, TS[i].valor);
							  break;

			case CTE_STRING: fprintf(fileAssembler, "%s db \"%s\",'$',%d dup(?)\n", TS[i].nombre,  TS[i].valor, MAX_LONG_CTE_STRING-TS[i].longitud-1);
							 fprintf(fileAssembler, "%s_long dd %d\n", TS[i].nombre,  TS[i].longitud+1);
							 break;
		}
	}
	
	//fprintf(fileAssembler,"__MENSAJE     db '$', %d dup (?)\n",MAX_LONG_CTE_STRING);
	fprintf(fileAssembler,"__ENTER       db MAXTEXTSIZE dup(?)\n");
	fprintf(fileAssembler,"__NEW_LINE    db 0Dh,0Ah,'$'\n");
	//fprintf(fileAssembler,"_AUXPrintReal db 14 dup (?), '$'\n");
	fprintf(fileAssembler,"_parteFrac    dd ?\n");
	fprintf(fileAssembler,"_parteEntera  dd ?\n");
	fprintf(fileAssembler,"_mul_10       dd 10.0\n");
	fprintf(fileAssembler,"_x87_round    dw 077fh\n");
}

void GeneracionCodigo()
{
	
	int i,r;
	 
	//Inicio de la generacion del codigo
	fprintf(fileAssembler,"\n.CODE\n");
	fprintf(fileAssembler,"\tmov AX, @DATA\n");
	fprintf(fileAssembler,"\tmov DS, AX\n");
	fprintf(fileAssembler,"\tmov ES, AX\n");
	fprintf(fileAssembler,"\tfinit\n\n");
	
	
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
		if(GenerarAssemblerByTerceto(i)==1)
			i++;

	}

	fprintf(fileAssembler, "etiqueta_%d:\n",cantTercetos);

	//Finalizar
	fprintf(fileAssembler,"ERROR:\n");
	fprintf(fileAssembler,"\tmov AX, 4C00h\n");
	fprintf(fileAssembler,"\tint 21h\n");

}

int GenerarAssemblerByTerceto(int idTerceto)
{
	int valorDevuelto=0;
	//Creacion de las etiquetas
	
	//printf("%d  %d \n", idTerceto,VerTope(&PilaDeEtiquetas));
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
		fprintf(fileAssembler,"\tfld %s\n", TS[listaDeTercetos[idTerceto].x].nombre);
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

	//VAMOS CON EL QUERIADO FILTERC
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
		if(listaDeTercetos[idTerceto].tipoDeY = VAR_FILTERC)
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

	}

	return valorDevuelto;

}

void asmConcatenacion(int idTerceto)
{
	//;verifico que se pueda realizar la concatenacion
fprintf(fileAssembler, "mov eax, %s_long \n",TS[listaDeTercetos[idTerceto].z].nombre);//           ;Cargo la longitud de cadena1 en ax
fprintf(fileAssembler, "add eax, %s_long \n",TS[listaDeTercetos[idTerceto].y].nombre);//            ;Le sumo a ax la longitud de cadena2
fprintf(fileAssembler, "sub ax, 1 \n");//                       ;Le resto 1 porque antes contabilicé dos veces el símbolo $
fprintf(fileAssembler, "cmp ax, MAXTEXTSIZE  \n"); //            ;Comparo
fprintf(fileAssembler, "jg  ERROR  \n");//                      ;Si (_cadena1_long+_cadena2_long-1) > MAXTEXTSIZE, salto a ERROR o termino

//;comienzo la concatenacion
fprintf(fileAssembler, "mov %s_long , eax  \n",TS[listaDeTercetos[idTerceto+1].y].nombre);//			;Guardo el tamaño final que tendra cadenaAux
fprintf(fileAssembler, "mov eax, %s_long  \n",TS[listaDeTercetos[idTerceto].y].nombre);// ;Guardo el tamaño de cadena1 en ax
fprintf(fileAssembler, "sub ax, 1 \n");//   ;Resto 1 de ax (no voy a guardar el simbolo $ de cadena1)
fprintf(fileAssembler, "cld     \n");//     ;Indico que el incremento sea positivo
fprintf(fileAssembler, "mov esi,OFFSET %s \n",TS[listaDeTercetos[idTerceto].y].nombre);//;apunto esi al inicio de cadena1
fprintf(fileAssembler, "mov edi, OFFSET %s \n",TS[listaDeTercetos[idTerceto+1].y].nombre);//;apunto edi al inicio de cadenaAux
fprintf(fileAssembler, "mov ecx, eax \n");//   ;Indico que va a tener que repetir ax veces
fprintf(fileAssembler, "rep movsb \n");//  ;copio los caracteres de cadena1 a cadenaAux (byte a byte)
fprintf(fileAssembler, "mov esi,OFFSET %s \n",TS[listaDeTercetos[idTerceto].z].nombre);//;apunto esi al inicio de cadena2
fprintf(fileAssembler, "mov ecx, %s_long \n", TS[listaDeTercetos[idTerceto].y].nombre);// ;Indico que va a tener que repetir _cadena2_long veces
fprintf(fileAssembler, "rep movsb   \n"); //                    ;concateno los caracteres de cadena2 a cadenaAux (byte a byte)
	
}

void asmWprint(int idTerceto)
{
	if(TS[listaDeTercetos[idTerceto].y].tipo== PR_STRING ||TS[listaDeTercetos[idTerceto].y].tipo== CTE_STRING )
	{
		fprintf(fileAssembler, "\tcld\n");
		fprintf(fileAssembler, "\tmov esi, OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, "\tmov edi, OFFSET __ENTER\n");
		fprintf(fileAssembler, "\tmov ecx, %s_long\n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, "\trep movsb\n");
		fprintf(fileAssembler, "\tcall IMPRIMIR\n");
	}
	else
	{
		fprintf(fileAssembler, "\tmov eax, %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, "\tmov _parteFrac, eax\n");
		fprintf(fileAssembler, "\tcall IMPRIMIRNUMERO\n");
	}
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
							case PR_INT:
							case CTE_REAL:
							case PR_FLOAT:
							case CTE_ENTERA: fprintf(fileAssembler,"\tfstp %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
											 break;
							case PR_STRING:
							case CTE_STRING: fprintf(fileAssembler,"\tmov eax, %s_long\n",TS[listaDeTercetos[idTerceto].z].nombre);
											 fprintf(fileAssembler,"\tmov %s_long, eax\n", TS[listaDeTercetos[idTerceto].y].nombre);
											 fprintf(fileAssembler,"\tcld\n");
											 fprintf(fileAssembler,"\tmov esi,OFFSET %s\n",TS[listaDeTercetos[idTerceto].z].nombre);
											 fprintf(fileAssembler,"\tmov edi,OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
											 fprintf(fileAssembler,"\tmov ecx, eax\n");
											 fprintf(fileAssembler,"\trep movsb\n");
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
    fprintf(fileAssembler, "\tfclex\n");
    fprintf(fileAssembler, "\tfldcw _x87_round\n");
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
    

	fprintf(fileAssembler, "\net3:\n");
    fprintf(fileAssembler, "\tfld _parteFrac\n");
    fprintf(fileAssembler, "\tfmul _mul_10\n");
    fprintf(fileAssembler, "\tfst _parteFrac\n");
    fprintf(fileAssembler, "\tfistp _parteEntera\n");
    fprintf(fileAssembler, "\tmov edx, _parteEntera\n");
    fprintf(fileAssembler, "\tadd dl, 030h\n");
    fprintf(fileAssembler, "\tmov [__ENTER+bx], dl\n");
    fprintf(fileAssembler, "\tinc bx\n");
    fprintf(fileAssembler, "\tfld _parteFrac\n");
    fprintf(fileAssembler, "\tfild _parteEntera\n");
    fprintf(fileAssembler, "\tfsubp\n");
    fprintf(fileAssembler, "\tfst _parteFrac\n");
    fprintf(fileAssembler, "\tfldz\n");
    fprintf(fileAssembler, "\tfcompp\n");
    fprintf(fileAssembler, "\tfstsw ax\n");
    fprintf(fileAssembler, "\tsahf\n");
    fprintf(fileAssembler, "\tje fin\n");
    fprintf(fileAssembler, "\tjmp et3\n");


	fprintf(fileAssembler, "\nfin:\n");
    fprintf(fileAssembler, "\tmov [__ENTER+bx], '$'\n");
    fprintf(fileAssembler, "\tcall IMPRIMIR\n");
    fprintf(fileAssembler, "\tret\n");
}