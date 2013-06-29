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
	for(i=0;i<cantTokensEnTS;i++)
	{
		switch(TS[i].tipo)
		{
		case PR_INT:
		case PR_FLOAT:
			fprintf(fileAssembler, "%s dd ?\n",TS[i].nombre);
			break;
			case PR_STRING:
				fprintf(fileAssembler, "%s db '$' , 60 dup (?)\n", TS[i].nombre);    
				fprintf(fileAssembler, "%s_long dd 1 \n", TS[i].nombre); 
			break;
			case  CTE_ENTERA:
			case  CTE_REAL:
					fprintf(fileAssembler, "%s dd %s\n", TS[i].nombre, TS[i].valor);

			break;
			case CTE_STRING:
				fprintf(fileAssembler, "%s db \"%s\",'$', %d dup (?)\n", TS[i].nombre,  TS[i].valor, MAX_LONG_CTE_STRING-TS[i].longitud-1);
				fprintf(fileAssembler, "%s_long dd %d \n", TS[i].nombre,  TS[i].longitud+1);
			break;

		}
	}
	
	fprintf(fileAssembler, "_VAR_FILTERC dd 0.0 \n");
	fprintf(fileAssembler,"MAXTEXTSIZE equ 50\n");
	fprintf(fileAssembler,  "__MENSAJE db  '$', %d dup (?)\n",MAX_LONG_CTE_STRING);
	fprintf(fileAssembler,"__ENTER	db \"HOLA MUNDO\" ,'$'\n");
	fprintf(fileAssembler,"_AUXPrintReal db 14 dup (?), '$'\n");
	//
	fprintf(fileAssembler,"_PARTEFRAC dd  ?                   ;Parte Fraccionaria del numero a imprimir. Almacena un float\n");
	fprintf(fileAssembler,"_parteEntera    dd  ?                   ;Parte Entera del numero a imprimir. Almacena un int\n");
	fprintf(fileAssembler,"_mul_10         dd  10.0                ;Valor necesario para separar cada dígito de la parte fraccionaria\n");
fprintf(fileAssembler,"_x87_round      dw  077fh               ;Palabra de estado del coprocesador para redondear hacia abajo\n");


}

void GeneracionCodigo()
{
	
	int i,r;
	 
	//Inicio de la generacion del codigo
	fprintf(fileAssembler,"\n.CODE\n");
	fprintf(fileAssembler,"mov AX, @DATA\n");
	fprintf(fileAssembler,"mov DS, AX\n");
	fprintf(fileAssembler,"mov ES, AX\n");
	fprintf(fileAssembler,"finit\n\n");
	
	
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
	fprintf(fileAssembler, "ERROR:\n");
	fprintf(fileAssembler,"mov AX, 4C00h\n");
	fprintf(fileAssembler,"int 21h\n");

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
		fprintf(fileAssembler,"FLD %s \n", TS[listaDeTercetos[idTerceto].x].nombre);
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
		fprintf(fileAssembler,"_VAR_FILTERC \n");
	}





	return valorDevuelto;
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
	fprintf(fileAssembler, "mov eax,  %s_long\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler, "cld\n");
	fprintf(fileAssembler, "mov esi , OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
	fprintf(fileAssembler, "mov edi , OFFSET __ENTER\n");
	fprintf(fileAssembler, " mov ecx, eax \n");
	fprintf(fileAssembler, " rep movsb \n");
	fprintf(fileAssembler, "call IMPRIMIR\n\n");
	}
	else
	{
		fprintf(fileAssembler, "mov eax, %s \n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, " mov _parteFrac, eax \n");
    fprintf(fileAssembler, "call imprimirNumero\n");
	

	}


 //

}

void asmNot(int idTerceto)
{
	if(listaDeTercetos[idTerceto].tipoDeY==BH)
		fprintf(fileAssembler,"NOT BH \n");

	if(listaDeTercetos[idTerceto].tipoDeY==CH)
		fprintf(fileAssembler,"NOT CH \n");
}

void asmOr(int idTerceto)
{
	if(listaDeTercetos[idTerceto].tipoDeY==BH
		&& listaDeTercetos[idTerceto].tipoDeZ==BL
		)
	{
		fprintf(fileAssembler,"OR BH , BL \n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==CH
		&& listaDeTercetos[idTerceto].tipoDeZ==CL
		)
	{
		fprintf(fileAssembler,"OR CH , CL \n");
	}
}

void asmAnd(int idTerceto)
{
	if(listaDeTercetos[idTerceto].tipoDeY==BH
		&& listaDeTercetos[idTerceto].tipoDeZ==BL
		)
	{
		fprintf(fileAssembler,"ADD BH , BL \n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==CH
		&& listaDeTercetos[idTerceto].tipoDeZ==CL
		)
	{
		fprintf(fileAssembler,"ADD CH , CL \n");
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

	if(listaDeTercetos[idTerceto].tipoDeY==BH 	&& listaDeTercetos[idTerceto].tipoDeZ==VALOR && listaDeTercetos[idTerceto].z==1)
	{
		fprintf(fileAssembler,"MOV BH , 0ffh\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==BH 	&& listaDeTercetos[idTerceto].tipoDeZ==VALOR && listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BH , 00h\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==BL && listaDeTercetos[idTerceto].tipoDeZ==VALOR 	&& listaDeTercetos[idTerceto].z==1)
	{
		fprintf(fileAssembler,"MOV BL , 0ffh\n");
	}
	
	if(listaDeTercetos[idTerceto].tipoDeY==BL  && listaDeTercetos[idTerceto].tipoDeZ==VALOR && listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BL , 00h\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==BL && listaDeTercetos[idTerceto].tipoDeZ==VALOR 	&& listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BL , 00h\n");
	}
	if(   TS[listaDeTercetos[idTerceto].y].tipo==PR_FLOAT 	|| TS[listaDeTercetos[idTerceto].y].tipo==PR_INT || TS[listaDeTercetos[idTerceto].y].tipo==CTE_ENTERA || TS[listaDeTercetos[idTerceto].y].tipo==CTE_REAL)
	{
		fprintf(fileAssembler,"FSTP %s \n", TS[listaDeTercetos[idTerceto].y].nombre);
	}

	//Asignacion de cadenas
	if(TS[listaDeTercetos[idTerceto].y].tipo==PR_STRING
		|| TS[listaDeTercetos[idTerceto].y].tipo==CTE_STRING)
	{

		//;asignacion
	
		fprintf(fileAssembler,"mov eax, %s_long \n",TS[listaDeTercetos[idTerceto].z].nombre);
		fprintf(fileAssembler,"mov %s_long, eax \n", TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler,"cld \n");
		fprintf(fileAssembler,"mov esi,OFFSET %s \n",TS[listaDeTercetos[idTerceto].z].nombre);
		fprintf(fileAssembler,"mov edi,OFFSET %s \n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler,"mov ecx, eax \n");
		fprintf(fileAssembler,"rep movsb	\n");

	
	
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

void GenerarCodigoString()
{
	
	//Rutina de salida a pantalla
    fprintf(fileAssembler, "IMPRIMIR : \n");
   
    fprintf(fileAssembler, "	mov DX, OFFSET __ENTER\n");
    fprintf(fileAssembler, "	mov AH, 9\n");
    fprintf(fileAssembler, "	int 21h\n");
    fprintf(fileAssembler, "	ret\n");
     
	
fprintf(fileAssembler, "imprimirNumero:\n");
    fprintf(fileAssembler, "	fclex                       ;Limpio los flags\n");
    fprintf(fileAssembler, "	fldcw _x87_round            ;Le indico al coprocesador que redondee para abajo\n");
    fprintf(fileAssembler, "	fld _parteFrac              ;Cargo la variable a imprimir. ST(0) = _parteFrac = idReal\n");
    fprintf(fileAssembler, "	fistp _parteEntera          ;Convierto a entero, redondendo hacia abajo, la guardo en _parteEntera y pop ST(0)   \n");
    fprintf(fileAssembler, "	fld _parteFrac              ;Vuelvo a cargar la variable a imprimir. ST(0) = _parteFrac = idReal\n");
    fprintf(fileAssembler, "	fild _parteEntera           ;Cargo la parte entera. ST(0) = _parteEntera, ST(1) = _parteFrac = idReal\n");
    fprintf(fileAssembler, "	fsubp                       ;Obtengo la parte fraccionaria. ST(1) = ST(1) - ST(0) = idReal - _parteEntera. Pop ST(0)\n");
    fprintf(fileAssembler, "	fstp _parteFrac             ;Guardo la parte fraccionaria y pop ST(0)\n");


    fprintf(fileAssembler, "	mov ecx, 0                  ;Inicializo ecx para que sirva como contador de digitos\n");
    fprintf(fileAssembler, "	mov ebx, 10                 ;Cargo el divisor en ebx\n");
    fprintf(fileAssembler, "	mov eax, _parteEntera       ;Cargo la parte baja del dividendo\n");
fprintf(fileAssembler, "et1:\n");
    fprintf(fileAssembler, "	mov edx, 0                  ;Cargo la parte alta del dividendo\n");
    fprintf(fileAssembler, "	div ebx                     ;Divido la parte entera (luego será el cociente) por 10\n");
    fprintf(fileAssembler, "	push dx                     ;Guardo en la pila la parte baja del resto\n");
    fprintf(fileAssembler, "	inc ecx                     ;Incremento ecx\n");
    fprintf(fileAssembler, "	cmp eax, 0                  ;Comparo el cociente con 0\n");
    fprintf(fileAssembler, "	jne et1                     ;Si el cociente no es 0 debo seguir sacando digitos\n");


    fprintf(fileAssembler, "	mov bx, 0                   ;Inicializo bx (indice de __ENTER)\n");
fprintf(fileAssembler, "et2:\n");
    fprintf(fileAssembler, "	pop dx                      ;Saco de la pila el primer numero de la parte entera\n");
    fprintf(fileAssembler, "	add dl, 030h                ;Ajusto para que sea un caracter ascii\n");
    fprintf(fileAssembler, "	mov [__ENTER+bx], dl        ;Almaceno el caracter ascii en __ENTER[bx]\n");
    fprintf(fileAssembler, "	inc bx                      ;Incremento bx (indice de __ENTER)\n");
    fprintf(fileAssembler, "	loop et2                    ;Itero hasta que ecx == 0 (con cada pasada se autodecrementa)\n");

    
    fprintf(fileAssembler, "	fld _parteFrac              ;Cargo _parteFrac en ST(0)\n");
    fprintf(fileAssembler, "	fldz                        ;Cargo 0.0 en ST(0), ST(1) = _parteFrac\n");
    fprintf(fileAssembler, "	fcompp                      ;Comparo y pop ST(0) y ST(1)\n");
    fprintf(fileAssembler, "	fstsw ax                    ;Almaceno el registro de estado del coprocesador en ax\n");
    fprintf(fileAssembler, "	sahf                        ;Cargo el registro de estado de la cpu\n");
    fprintf(fileAssembler, "	je fin                      ;Finalizo porque se trata de un numero entero\n");

    fprintf(fileAssembler, "	mov [__ENTER+bx], '.'       ;Como se trata de un numero real le agrego el punto\n");
    fprintf(fileAssembler, "	inc bx                      ;Incremento bx (indice de __ENTER)\n");
    

fprintf(fileAssembler, "et3:\n");
    fprintf(fileAssembler, "	fld _parteFrac              ;Cargo la parte fraccionaria en ST(0)\n");
    fprintf(fileAssembler, "	fmul _mul_10                ;Multipilo ST(0) * _mul_10 y guardo en ST(0)\n");
    fprintf(fileAssembler, "	fst _parteFrac              ;Guardo la multiplicación anterior. _parteFrac quedaría con un número mayor a 1.0\n");
    fprintf(fileAssembler, "	fistp _parteEntera          ;Trunco la parte entera, la guardo en _parteEntera y pop ST(0)\n");
    fprintf(fileAssembler, "	mov edx, _parteEntera       ;Almaceno _parteEntera en edx\n");
    fprintf(fileAssembler, "	add dl, 030h                ;Ajusto para que sea un caracter ascii\n");
    fprintf(fileAssembler, "	mov [__ENTER+bx], dl        ;Almaceno el caracter ascii en __ENTER[bx]\n");
    fprintf(fileAssembler, "	inc bx                      ;Incremento bx (indice de __ENTER)\n");
    fprintf(fileAssembler, "	fld _parteFrac              ;Cargo la parte fraccionaria en ST(0)\n");
    fprintf(fileAssembler, "	fild _parteEntera           ;Cargo la parte entera en ST(0). ST(1) = _parteFrac\n");
    fprintf(fileAssembler, "	fsubp                       ;Resto _parteFrac menos _parteEntera y pop. Sería, por ejemlo, 1.53-1.0. ST(1) = 0.53\n");
    fprintf(fileAssembler, "	fst _parteFrac              ;Guardo la parte fraccionaria\n");
    fprintf(fileAssembler, "	fldz                        ;Cargo 0.0 en ST(0), ST(1) = _parteFrac\n");
    fprintf(fileAssembler, "	fcompp                      ;Comparo y pop ST(0) y ST(1)\n");
    fprintf(fileAssembler, "	fstsw ax                    ;Almaceno el registro de estado del coprocesador en ax\n");
    fprintf(fileAssembler, "	sahf                        ;Cargo el registro de estado de la cpu\n");
    fprintf(fileAssembler, "	je fin                      ;Finalizo porque ya no quedan más dígitos\n");
    fprintf(fileAssembler, "	jmp et3                     ;Salto a et3\n");


fprintf(fileAssembler, "fin:\n");
    fprintf(fileAssembler, "	mov [__ENTER+bx], '$'       ;Termino la cadena\n");
    fprintf(fileAssembler, "	call IMPRIMIR                 ;Llamo a imprimir el número\n");
    fprintf(fileAssembler, "	ret                         ;Retorno\n");



}