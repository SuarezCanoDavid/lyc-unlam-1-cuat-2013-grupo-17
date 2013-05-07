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

//CTE_NUMERICA 


void iniciarCteNum(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';

}

void continuarCteNum(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0';

}

void finalizarCteNum(tokenAAnalizar *tokenActual, char caracter)
{
	//insertarEnTS(tokenActual,CTE_NUMERICA) ;

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
tokenActual->token[tokenActual->longitudToken] = caracter;
tokenActual->longitudToken++;
tokenActual->token[tokenActual->longitudToken] = '\0';
//insertarEnTS(tokenActual,CTE_STRING);
//descartar = false; 

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

