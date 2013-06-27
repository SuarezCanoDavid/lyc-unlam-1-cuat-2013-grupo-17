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
				fprintf(fileAssembler, "_%s_long dd 1 \n", TS[i].nombre); 
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
		GenerarAssemblerByTerceto(i);
	}

	fprintf(fileAssembler, "etiqueta_%d:\n",cantTercetos);




	//Finalizar
	fprintf(fileAssembler,"mov AX, 4C00h\n");
	fprintf(fileAssembler,"int 21h\n");

}

void GenerarAssemblerByTerceto(int idTerceto)
{
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

	//VAMOS CON EL QUERIADO FILTERC
	if(listaDeTercetos[idTerceto].tipoDeX==VAR_FILTERC)
	{
		fprintf(fileAssembler,"_VAR_FILTERC \n");
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

}


void asmWprint(int idTerceto)
{

	fprintf(fileAssembler, "lea di, _AUXPrintReal\n");
	fprintf(fileAssembler, "lea si, _%s\n", TS[listaDeTercetos[idTerceto].y].valor); 
	fprintf(fileAssembler, "call formatReal\n");
	fprintf(fileAssembler, "lea DX, _AUXPrintReal\n");
	fprintf(fileAssembler, "call IMPRIMIR\n\n");

	//fprintf(fileAssembler, "mov eax,  %s_long\n",TS[listaDeTercetos[idTerceto].y].nombre);
	//fprintf(fileAssembler, "cld\n");
	//fprintf(fileAssembler, "mov esi , OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
	//fprintf(fileAssembler, "mov edi , OFFSET __ENTER\n");
	//fprintf(fileAssembler, " mov ecx, eax \n");
	//fprintf(fileAssembler, " rep movsb \n");
	//fprintf(fileAssembler, "call IMPRIMIR\n\n");
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
		fprintf(fileAssembler,"MOV BH , 0ffh\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==BH
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BH , 00h\n");
	}


	if(listaDeTercetos[idTerceto].tipoDeY==BL
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==1)
	{
		fprintf(fileAssembler,"MOV BL , 0ffh\n");
	}
	
	if(listaDeTercetos[idTerceto].tipoDeY==BL
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BL , 00h\n");
	}

	if(listaDeTercetos[idTerceto].tipoDeY==BL
		&& listaDeTercetos[idTerceto].tipoDeZ==VALOR
		&& listaDeTercetos[idTerceto].z==0)
	{
		fprintf(fileAssembler,"MOV BL , 00h\n");
	}
	if(listaDeTercetos[idTerceto].tipoDeY==INDICE_TS)
	{
		fprintf(fileAssembler,"FSTP %s \n", TS[listaDeTercetos[idTerceto].y].nombre);
	}

	//Asignacion de cadenas
	if(listaDeTercetos[idTerceto].tipoDeY==INDICE_TS
		&& listaDeTercetos[idTerceto].tipoDeZ==INDICE_TS)
	{
		fprintf(fileAssembler, "mov si, OFFSET %s\n",TS[listaDeTercetos[idTerceto].y].nombre);
		fprintf(fileAssembler, "mov di, OFFSET %s\n", TS[listaDeTercetos[idTerceto].z].nombre);
		fprintf(fileAssembler, "call COPIAR\n\n");
	
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
    fprintf(fileAssembler, "IMPRIMIR PROC\n");
    //fprintf(fileAssembler, "	mov AH, 9\n");
    //fprintf(fileAssembler, "	int 21h\n");
    fprintf(fileAssembler, "	mov DX, OFFSET __ENTER\n");
    fprintf(fileAssembler, "	mov AH, 9\n");
    fprintf(fileAssembler, "	int 21h\n");
    fprintf(fileAssembler, "	ret\n");
    fprintf(fileAssembler, "IMPRIMIR ENDP\n\n");
     
	    //Rutina para contar cantidad de caracteres de una cadena
    fprintf(fileAssembler, "STRLEN PROC\n");
    fprintf(fileAssembler, "	mov bx,0\n");
    fprintf(fileAssembler, "STRL01:\n");
    fprintf(fileAssembler, "	cmp BYTE PTR [SI+BX],'$'\n");
    fprintf(fileAssembler, "	je STREND\n");
    fprintf(fileAssembler, "	inc BX\n");
    fprintf(fileAssembler, "	cmp BX, MAXTEXTSIZE\n");
    fprintf(fileAssembler, "	jl STRL01\n");
    fprintf(fileAssembler, "STREND:\n");
    fprintf(fileAssembler, "	ret\n");
    fprintf(fileAssembler, "STRLEN ENDP\n\n");


	    //Rutina para copiar cadenas de caracteres
    fprintf(fileAssembler, "COPIAR PROC\n");
    fprintf(fileAssembler, "	call STRLEN ; busco la cantidad de caracteres\n");
    fprintf(fileAssembler, "	cmp bx,MAXTEXTSIZE\n");
    fprintf(fileAssembler, "	jle COPIARSIZEOK\n");
    fprintf(fileAssembler, "	mov bx,MAXTEXTSIZE\n");
    fprintf(fileAssembler, "COPIARSIZEOK:\n");
    fprintf(fileAssembler, "	mov cx,bx					; la copia se hace de 'CX' caracteres\n");
    fprintf(fileAssembler, "	cld							; cld es para que la copia se realice hacia adelante\n");
    fprintf(fileAssembler, "	rep movsb					; copia la cadea\n");
    fprintf(fileAssembler, "	mov al,'$'					; carácter terminador\n");
    fprintf(fileAssembler, "	mov BYTE PTR [DI],al		; el registro DI quedo apuntando al final\n");
    fprintf(fileAssembler, "	ret\n");
    fprintf(fileAssembler, "COPIAR ENDP\n\n");



	   fprintf(fileAssembler, "FRBUFFLEN equ 13\n");
    fprintf(fileAssembler, "FRBUFFPTO equ 9\n");


	fprintf(fileAssembler, "formatReal PROC\n");
    fprintf(fileAssembler, "    push di\n");
    fprintf(fileAssembler, "    mov cx,FRBUFFLEN\n");
    fprintf(fileAssembler, "    mov al,byte ptr ' '\n");
    fprintf(fileAssembler, "    rep stosb\n");
    fprintf(fileAssembler, "    mov [di],byte ptr '$'\n");
    fprintf(fileAssembler, "    call getSigno\n");
    fprintf(fileAssembler, "    pop di\n");
    fprintf(fileAssembler, "    push di\n");
    fprintf(fileAssembler, "    mov byte ptr [di],al\n");
    fprintf(fileAssembler, "    mov byte ptr [di+FRBUFFPTO],byte ptr '.'\n");
    fprintf(fileAssembler, "    call getExponente\n");
    fprintf(fileAssembler, "    mov bx,ax\n");
    fprintf(fileAssembler, "    call getMantisa\n");
    fprintf(fileAssembler, "    push eax\n");
    fprintf(fileAssembler, "    mov cl,10d\n");
    fprintf(fileAssembler, "    shr eax,cl\n");
    fprintf(fileAssembler, "    add di,FRBUFFPTO-1\n");
    fprintf(fileAssembler, "    call format9\n");
    fprintf(fileAssembler, "    pop eax\n");
    fprintf(fileAssembler, "    and eax,003FFFFh\n");
    fprintf(fileAssembler, "    call convDecimal\n");
    fprintf(fileAssembler, "    pop di\n");
    fprintf(fileAssembler, "    add di,FRBUFFPTO+3\n");
    fprintf(fileAssembler, "    mov cx,3\n");
    fprintf(fileAssembler, "    call formatX\n");
    fprintf(fileAssembler, "    ret\n");
    fprintf(fileAssembler, "formatReal ENDP\n");


	
	fprintf(fileAssembler, "getSigno:\n");
    fprintf(fileAssembler, "    mov word ptr ax,[si+2]\n");
    fprintf(fileAssembler, "    test ax,08000h\n");
    fprintf(fileAssembler, "    je csPos\n");
    fprintf(fileAssembler, "    mov al,'-'\n");
    fprintf(fileAssembler, "    jmp csEnd\n");
    fprintf(fileAssembler, "cSPos:\n");
    fprintf(fileAssembler, "    mov al,'+'\n");
    fprintf(fileAssembler, "csEnd:\n");
    fprintf(fileAssembler, "    ret\n");
    fprintf(fileAssembler, "getExponente:\n");
    fprintf(fileAssembler, "    mov ax,[si+2]\n");
    fprintf(fileAssembler, "    shl ax,1\n");
    fprintf(fileAssembler, "    xchg al,ah\n");
    fprintf(fileAssembler, "    and ax,000FFh\n");
    fprintf(fileAssembler, "    add eax,0FF81h ; bug del TASM, no genera sub ax,07Fh!!!\n");
    fprintf(fileAssembler, "    ret\n");
    fprintf(fileAssembler, "getMantisa:\n");
    fprintf(fileAssembler, "    mov eax,[si]\n");
    fprintf(fileAssembler, "    and eax,007FFFFFh\n");
    fprintf(fileAssembler, "    or eax,00800000h\n");
    fprintf(fileAssembler, "    mov cx,13d\n");
    fprintf(fileAssembler, "    sub cx,bx\n");
    fprintf(fileAssembler, "    jns gM01\n");
    fprintf(fileAssembler, "    neg cx\n");
    fprintf(fileAssembler, "    shl eax,cl\n");
    fprintf(fileAssembler, "    jmp gMend\n");
    fprintf(fileAssembler, "gM01:\n");
    fprintf(fileAssembler, "    clc\n");
    fprintf(fileAssembler, "    shr eax,cl\n");
    fprintf(fileAssembler, "gMend:\n");
    fprintf(fileAssembler, "    ret\n");
    fprintf(fileAssembler, "formatear:\n");
    fprintf(fileAssembler, "format9:\n");
    fprintf(fileAssembler, "    mov edx,eax\n");
    fprintf(fileAssembler, "    mov cl,16\n");
    fprintf(fileAssembler, "    shr edx,cl ; genero DX:AX\n");
    fprintf(fileAssembler, "    mov bx,10000\n");
    fprintf(fileAssembler, "    div bx ; ax resultado, dx, reminder\n");
    fprintf(fileAssembler, "    push ax\n");
    fprintf(fileAssembler, "    mov bx,10\n");
    fprintf(fileAssembler, "    mov ax,dx\n");
    fprintf(fileAssembler, "    mov cl,4\n");
    fprintf(fileAssembler, "    fCiclo01:\n");
    fprintf(fileAssembler, "    xor dx,dx\n");
    fprintf(fileAssembler, "    idiv bx\n");
    fprintf(fileAssembler, "    add dl,'0'\n");
    fprintf(fileAssembler, "    mov es:[di],dl\n");
    fprintf(fileAssembler, "    sub di,1\n");
    fprintf(fileAssembler, "    sub cl,1\n");
    fprintf(fileAssembler, "    jne fCiclo01\n");
    fprintf(fileAssembler, "    pop ax\n");
    fprintf(fileAssembler, "    mov cl,3\n");
    fprintf(fileAssembler, "formatX:\n");
    fprintf(fileAssembler, "    mov bx,10\n");
    fprintf(fileAssembler, "fCiclo02:\n");
    fprintf(fileAssembler, "    xor dx,dx\n");
    fprintf(fileAssembler, "    idiv bx\n");
    fprintf(fileAssembler, "    add dl,'0'\n");
    fprintf(fileAssembler, "    mov es:[di],dl\n");
    fprintf(fileAssembler, "    sub di,1\n");
    fprintf(fileAssembler, "    sub cl,1\n");
    fprintf(fileAssembler, "    jnz fCiclo02\n");
    fprintf(fileAssembler, "    ret\n");
    fprintf(fileAssembler, "numeros dw 500,250,125,62,31,15,8,4,2,1\n");
    fprintf(fileAssembler, "convDecimal:\n");
    fprintf(fileAssembler, "    mov cx,09\n");
    fprintf(fileAssembler, "    xor bx,bx\n");
    fprintf(fileAssembler, "    xor dx,dx\n");
    fprintf(fileAssembler, "    clc\n");
    fprintf(fileAssembler, "cvCiclo:\n");
    fprintf(fileAssembler, "    shr ax,1\n");
    fprintf(fileAssembler, "    jnc cvCiclo01\n");
    fprintf(fileAssembler, "    mov bx,cx\n");
    fprintf(fileAssembler, "    shl bx,1\n");
    fprintf(fileAssembler, "    add bx,offset numeros\n");
    fprintf(fileAssembler, "    mov bx,cs:[bx]\n");
    fprintf(fileAssembler, "    add bx,dx\n");
    fprintf(fileAssembler, "    mov dx,bx\n");
    fprintf(fileAssembler, "cvCiclo01:\n");
    fprintf(fileAssembler, "    sub cx,1\n");
    fprintf(fileAssembler, "    jns cvCiclo\n");
    fprintf(fileAssembler, "    mov ax,dx\n");
    fprintf(fileAssembler, "    add ax,1\n");
    fprintf(fileAssembler, "	ret\n");

}