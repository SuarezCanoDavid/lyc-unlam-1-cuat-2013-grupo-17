#include "AnalizadorLexico.h"

int main()
{

	return 0;
}


void iniciarId(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken]=caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken]='\0';

}

void continuarId(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter ;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';

	
}
void finalizarId(tokenAAnalizar *tokenActual, char caracter)
{

//	insertarEnTS(tokenActual,ID);
}

//CTE_ENTERA o CTE_REAL 
 


void iniciarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';
}

void continuarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++ ;
	tokenActual->token[tokenActual->longitudToken] = '\0';
}

void finalizarCteEntera(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,CTE_ENTERA);

}

void iniciarCteReal(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';
}

void continuarCteReal(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';
}

void finalizarCteReal(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,CTE_REAL);

}


//CTE_STRING 

void iniciarCteStr(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0' ;

}
void continuarCteStr(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] =  '\0';

}

void finalizarCteStr(tokenAAnalizar *tokenActual, char caracter)
{

	//insertarEnTS(tokenActual,CTE_STRING);
	//cteStringAbierta = FALSE; 
}

//OP_SUMA u OP_CONCATENACION 
void iniciarOpSumaOConcat(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';


}
void finalizarOpSuma(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_SUMA) ;

}
void continuarOpConcat(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';

}
void finalizarOpConcat(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_CONCATENACION);

}

//OP_RESTA o comentario 

void iniciarOpRestaOComent(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';
}

void finalizarOpResta(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_RESTA);
}


void continuarComentario(tokenAAnalizar *tokenActual, char caracter)
{
	//comentarioAbierto = TRUE;
}

void finalizarComentario(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->longitudToken = 0;
	//comentarioAbierto = FALSE;

}

//OP_MULTIPLICACION 

void iniciarOpMulti(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';

}
void finalizarOpMulti(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_MULTIPLICACION); 

}

//OP_DIVISION 
void iniciarOpDivi(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 

}
void finalizarOpDivi(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_DIVISION);

}

//PARENTESIS_ABRE 
void iniciarParAbre(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';
}

void finalizarParAbre(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,PARENTESIS_ABRE) ;
}

//PARENTESIS_CIERRA 
void iniciarParCierra(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';

}
void finalizarParCierra(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,PARENTESIS_CIERRA) ;
}

//CORCHETES_ABRE 
void iniciarCorAbre(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarCorAbre(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,CORCHETES_ABRE);
}

//CORCHETES_CIERRA 
void iniciarCorCierra(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';

}

void finalizarCorCierra(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,CORCHETES_CIERRA);
}

//PUNTO_COMA 
void iniciarPuntoComa(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarPuntoComa(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,PUNTO_COMA) ;
}

//DOS_PUNTOS 

void iniciarDosPuntos(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarDosPuntos(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,DOS_PUNTOS) ;
}

//GUION_BAJO 
void iniciarGuionBajo(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';


}

void finalizarGuionBajo(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,GUION_BAJO);

}

//COMA 
void iniciarComa(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarComa(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,COMA);
}

//OP_MENOR u OP_MENOR_IGUAL 
void iniciarOpMenorOMenorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarOpMenor(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_MENOR);
}

void continuarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_MENOR_IGUAL);
}

//OP_MAYOR u OP_MAYOR_IGUAL 

void iniciarOpMayorOMayorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}
void finalizarOpMayor(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_MAYOR);
}

void continuarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_MAYOR_IGUAL) ;
}

//OP_ASIGNACION u OP_IGUAL 
void iniciarOpAsigOIgual(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarOpAsig(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_ASIGNACION); 
}

void continuarOpIgual(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 

}
void finalizarOpIgual(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_IGUAL);

}

//OP_DISTINTO 

void iniciarOpDistinto(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void continuarOpDistinto(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}

void finalizarOpDistinto(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,OP_DISTINTO); 

}

//Espacios vacíos (ENTER, TAB, ESPACIO) 
void ignorarEspacios(tokenAAnalizar * (tokenActual), char caracter)
{

}

//Error de sintáxis 
void syntaxError(tokenAAnalizar * (tokenActual), char caracter)
{
}