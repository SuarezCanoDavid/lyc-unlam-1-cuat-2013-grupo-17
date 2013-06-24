#include "AnalizadorLexico.h"
#include "tokens.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*Archivo de código fuente*/
FILE *archivoFuente;

/*Archivo de tokens identificados*/
FILE *archivoDeTokens;

/*Archivo de salida del analizador sintactico*/
extern FILE *salidaAS;

/*Matriz de Nuevo Estado*/
int nuevoEstado[ESTADO_FINAL][CANT_COLUMNAS];

/*Matriz de proceso*/
void (*proceso[ESTADO_FINAL][CANT_COLUMNAS])(tokenAAnalizar *, char);

/*Indica el tipo de token identificado*/
int tipoToken;

/*Tabla de símbolos*/
tablaDeSimbolos TS[LONG_TS];

/*Contador de la cantidad total de tokens identificados*/
int cantTokensIdentificados = 0;

/*Contador de la cantidad de tokens en al TS*/
int cantTokensEnTS = 0;

/*Vector de palabras reservadas*/
char palabrasReservadas[CANT_PR][10] = { "FOR", "ROF", "IF", "ELSE", "FI", "WPRINT", "FILTERC", "DO", 
										"WHILE", "AND", "OR", "NOT", "VAR", "ENDVAR", "INT", "FLOAT", "STRING" ,
										 "for", "rof", "if", "else", "fi", "wprint", "filterc", "do", 
										"while", "and", "or", "not", "var", "endvar", "int", "float", "string"}; 
// Vector para las palabras reservadas
int vPalabrasReservadasBis[CANT_PR] = { PR_FOR,PR_ROF,PR_IF,PR_ELSE,PR_FI,PR_WPRINT,PR_FILTERC,PR_DO,
										PR_WHILE,PR_AND,PR_OR,PR_NOT,PR_VAR,PR_ENDVAR,PR_INT,PR_FLOAT,PR_STRING,
										PR_FOR,PR_ROF,PR_IF,PR_ELSE,PR_FI,PR_WPRINT,PR_FILTERC,PR_DO,
										PR_WHILE,PR_AND,PR_OR,PR_NOT,PR_VAR,PR_ENDVAR,PR_INT,PR_FLOAT,PR_STRING};

/*Manejador de errores*/
manejadorDeErrores error[CANT_ERRORES];

char tipoTokenSalida[LONG_TIPO_TOKEN];

/*Numero de linea actual*/
int lineaActual = 1;

void inicializarAL(FILE *fuente)
{
	int i;

	/*Vinculo el archivo de código para que lo utilice el Analizador Léxico*/
	archivoFuente = fuente;

	/*Creo y abro el archivo donde se guardarán los todos los tokens identificados*/
	fopen_s(&archivoDeTokens,"tokens_identificados.txt","w");

	/*Inicializo las matrices de nuevoEstado y proceso*/
	inicializarMatrices();

	/*Inicializo la TS*/
	for(i = 0; i < LONG_TS; ++i)
	{
		TS[i].nombre[0] = '\0';
		TS[i].tipo = 0;
		TS[i].valor[0] = '\0';
		TS[i].longitud = 0;
	}

	error[ERROR_CTE_STRING_ABIERTA].estado = FALSE;
	strcpy_s(error[ERROR_CTE_STRING_ABIERTA].descripcion,LONG_DESC_ERROR,"CONSTANTE STRING ABIERTA");
	
	error[ERROR_COMENTARIO_ABIERTO].estado = FALSE;
	strcpy_s(error[ERROR_COMENTARIO_ABIERTO].descripcion,LONG_DESC_ERROR,"COMENTARIO ABIERTO");

	error[ERROR_CTE_STRING_SUPERA_30].estado = FALSE;
	strcpy_s(error[ERROR_CTE_STRING_SUPERA_30].descripcion,LONG_DESC_ERROR,"CONSTANTE STRING SUPERA LOS 30 CARACTERES");
	
	error[ERROR_CTE_ENTERA_FUERA_DE_RANGO].estado = FALSE;
	strcpy_s(error[ERROR_CTE_ENTERA_FUERA_DE_RANGO].descripcion,LONG_DESC_ERROR,"CONSTANTE ENTERA FUERA DE RANGO DE REPRESENTACION");

	error[ERROR_CTE_REAL_FUERA_DE_RANGO].estado = FALSE;
	strcpy_s(error[ERROR_CTE_REAL_FUERA_DE_RANGO].descripcion,LONG_DESC_ERROR,"CONSTANTE REAL FUERA DE RANGO DE REPRESENTACION");

	error[ERROR_FORMATO_NUMERICO_INVALIDO].estado = FALSE;
	strcpy_s(error[ERROR_FORMATO_NUMERICO_INVALIDO].descripcion,LONG_DESC_ERROR,"FORMATO NUMERICO INVALIDO");

	error[ERROR_OP_DISTINTO_NO_FINALIZADO].estado = FALSE;
	strcpy_s(error[ERROR_OP_DISTINTO_NO_FINALIZADO].descripcion,LONG_DESC_ERROR,"OPERADOR DISTINTO NO FINALIZADO");

	error[ERROR_CARACTER_NO_VALIDO].estado = FALSE;
	strcpy_s(error[ERROR_CARACTER_NO_VALIDO].descripcion,LONG_DESC_ERROR,"CARACTER NO VALIDO");
}

int yylex()
{
	int estado = 0;
	int columna;
	int input;
	int i;
	int retrocederLectura = TRUE;
	char caracter;
	tokenAAnalizar tokenActual;
	tokenActual.token[0]='\0';
    tokenActual.longitudToken = 0;

	do
	{	
		input = fgetc(archivoFuente);
        
		if(input != EOF)
		{
			caracter = (char)input;
			
	        columna = determinarColumna(caracter);

			if(estado == 0 && columna == 21)
			{
				error[ERROR_CARACTER_NO_VALIDO].estado = TRUE;
			}

	        (*proceso[estado][columna])(&tokenActual,caracter);

	        estado = nuevoEstado[estado][columna];
		}
		else
		{
			if(estado != 0)
			{
				for(i = 0; i < CANT_ERRORES; ++i)
				{
					if(error[i].estado == TRUE)
					{
						syntaxError(&tokenActual,caracter);
					}
				}

				caracter = ' '; /*Fuezo la terminación del token actual*/

				columna = determinarColumna(caracter);

				(*proceso[estado][columna])(&tokenActual,caracter);
			}
			else
			{
				tipoToken = EOF;

				fclose(archivoDeTokens);

				imprimirTS();
			}

			estado = ESTADO_FINAL;

			retrocederLectura = FALSE;
		}

	}while(estado != ESTADO_FINAL);

	if(retrocederLectura == TRUE)
	{
		ungetc(caracter,archivoFuente);
	}

	return tipoToken;
}

int determinarColumna(char caracter)
{
	int columna;

	switch(caracter)
	{
		case '0':	columna = 0;	 
					break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':	columna = 1;
					break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z': 	columna = 2;
				  	break;	
		case '(':	columna = 3;
					break;
		case ')':	columna = 4;
					break;
		case '[':	columna = 5;
					break;
		case ']':	columna = 6;
					break;
		case ';':	columna = 7;
					break;
		case ':':	columna = 8;
					break;		
		case '"':	columna = 9;
					break;
		case '<':	columna = 10;
					break;
		case '>':	columna = 11;
					break;
		case '=':	columna = 12;
					break;
		case '!':	columna = 13;
					break;
		case '+':	columna = 14;
					break;
		case '*':	columna = 15;
					break;
		case '/':	columna = 16;
					break;
		case '-':	columna = 17;
					break;
		case ',':	columna = 18;
					break;
		case '_':	columna = 19;
					break;
		case '.':	columna = 20;
					break;
		case '#':
		case '$':
		case '%':
		case '&':
		case '?':
		case '¿':
		case '¡':
		case '@':
		case '\\':
		case '{':
		case '}':
		case '|':
		case '~':
		case '°':
		case '¬':
		case '½':
		case 'ñ':
		case 'Ñ':
		case '\'':
		case '^':
		case '·': 	columna = 21;
				  	break;
		case ' ':
		case '\t':
		case '\n': 	columna = 22;
				  	break;
	}

	return columna;
}

void insertarTokenEnTS(tokenAAnalizar *tokenActual, const int tipoDeToken)
{
	int i = 0;
	int j;
	int esPalabraReservada = FALSE;
	char nombreAux[MAX_LONG_TOKEN+1];

	yylval = -1;

	/*Compruebo si el token es una palabra reservada*/
	while(i < CANT_PR && esPalabraReservada == FALSE)
	{
		if(strcmp(palabrasReservadas[i],tokenActual->token) == 0)
		{
			esPalabraReservada = TRUE;
		}

		++i;
	}

	/*Si es una palabra reservada*/
	if(esPalabraReservada == TRUE)
	{
		tipoToken = vPalabrasReservadasBis[i-1];
	}
	else
	{
		/*En la TS solo figuran IDs y CTEs*/
		if(tipoDeToken == ID || tipoDeToken == CTE_STRING || tipoDeToken == CTE_ENTERA || tipoDeToken == CTE_REAL)
		{
			nombreAux[0] = '\0';

			if(tipoDeToken == CTE_ENTERA || tipoDeToken == CTE_REAL)
			{
				nombreAux[0] = '_';
				nombreAux[1] = '\0';
			}

			strcat_s(nombreAux,MAX_LONG_TOKEN+1,tokenActual->token);

			/*Busco el token en la TS*/
			for(i = 0; i < cantTokensEnTS && strcmp(TS[i].nombre,nombreAux) != 0; ++i);

			/*Si no se encontró el token (i se igualó a cantTokensEnTS), lo inserto en la TS*/
			if(i == cantTokensEnTS)
			{
				/*Si es alguna de las constantes necesita un tratamiento previo*/
				if(tipoDeToken == CTE_STRING || tipoDeToken == CTE_ENTERA || tipoDeToken == CTE_REAL)
				{
					/*Guardo el tipo de token*/
					TS[i].tipo = tipoDeToken;

					/*Si es una constante string*/
					if(tipoDeToken == CTE_STRING)
					{
						nombreAux[0] = '_';
						/*Guardo el valor del token sin las comillas*/
						for(j = 1; tokenActual->token[j] != '"'; ++j)
						{
							TS[i].valor[j-1] = tokenActual->token[j];
							nombreAux[j] = tokenActual->token[j];
							if(nombreAux[j] == ' ')
							{
								nombreAux[j] = '_';
							}
						}
						TS[i].longitud = j-1;
						TS[i].valor[TS[i].longitud] = '\0';
						nombreAux[j] = '\0';
					}
					else /*Si es una constante numerica*/
					{
						/*Guardo el valor del token*/
						strcpy_s(TS[i].valor,MAX_LONG_TOKEN,tokenActual->token);

						if(tipoDeToken == CTE_ENTERA)
						{
							strcat_s(TS[i].valor,MAX_LONG_TOKEN,".0");
						}
					}
				}

				/*Vuelvo a limpiar el nombre en la TS*/
				TS[i].nombre[0] = '\0';

				/*Guardo el nombre del token*/
				strcat_s(TS[i].nombre,MAX_LONG_TOKEN+1,nombreAux);

				/*Incremento la cantidad de tokens*/
				++cantTokensEnTS;
			}

			/*Devuelvo la posición en la TS en yylval*/
			yylval = i;
		}

		/*Asigno el tipo de token a la variable global tipoToken*/
		tipoToken = tipoDeToken;
	}

	/*Guardo el token identificado, su tipo e yylval en el archivo de tokens*/
	fprintf(archivoDeTokens,"TOKEN Nº%4d= %-50sTIPO_TOKEN= %-20sYYLVAL= %d\n",
		++cantTokensIdentificados,tokenActual->token,identificarTipoToken(tipoToken),yylval);
}

void imprimirTS()
{
	FILE *archivoDeTS;
	int i;

	fopen_s(&archivoDeTS,"Tabla_de_símbolos.txt","w");

	for(i = 0; i < cantTokensEnTS; ++i)
	{
		fprintf(archivoDeTS,"%-3d%-50s%-15s%-50s%-10d\n",
			i,TS[i].nombre,identificarTipoToken(TS[i].tipo),TS[i].valor,TS[i].longitud);
	}

	fclose(archivoDeTS);
}

char *identificarTipoToken(int tipo)
{
	switch(tipo)
	{
		case PAR_ABRE:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PAR_ABRE");
								break;
		case PAR_CIERRA:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PAR_CIERRA");
								break;
		case COR_ABRE:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"COR_ABRE");
								break;
		case COR_CIERRA:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"COR_CIERRA");
								break;
		case COMA:				strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"COMA");
								break;
		case PUNTO_COMA:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PUNTO_COMA");
								break;
		case DOS_PUNTOS:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"DOS_PUNTOS");
								break;
		case GUION_BAJO:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"GUION_BAJO");
								break;
		case ID:				strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"ID");
								break;
		case CTE_STRING:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"CTE_STRING");
								break;
		case CTE_ENTERA:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"CTE_ENTERA");
								break;
		case CTE_REAL:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"CTE_REAL");
								break;
		case OP_SUMA:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_SUMA");
								break;
		case OP_RESTA:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_RESTA");
								break;
		case OP_MULTIPLICACION:	strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_MULTIPLICACION");
								break;
		case OP_DIVISION:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_DIVISION");
								break;
		case OP_MAYOR:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_MAYOR");
								break;
		case OP_MAYOR_IGUAL:	strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_MAYOR_IGUAL");
								break;
		case OP_MENOR:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_MENOR");
								break;
		case OP_MENOR_IGUAL:	strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_MENOR_IGUAL");
								break;
		case OP_IGUAL:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_IGUAL");
								break;
		case OP_DISTINTO:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_DISTINTO");
								break;
		case OP_ASIGNACION:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_ASIGNACION");
								break;
		case OP_CONCATENACION:	strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"OP_CONCATENACION");
								break;
		case PR_IF	:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_IF");
								break;
		case PR_FI:				strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_FI");
								break;
		case PR_FOR:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_FOR");
								break;
		case PR_ROF:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_ROF");
								break;
		case PR_ELSE:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_ELSE");
								break;
		case PR_WPRINT:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_WPRINT");
								break;
		case PR_FILTERC:		strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_FILTERC");
								break;
		case PR_DO:				strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_DO");
								break;
		case PR_WHILE:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_WHILE");
								break;
		case PR_AND:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_AND");
								break;
		case PR_OR:				strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_OR");
								break;
		case PR_NOT:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_NOT");
								break;
		case PR_VAR:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_VAR");
								break;
		case PR_ENDVAR:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_ENDVAR");
								break;
		case PR_INT:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_INT");
								break;
		case PR_FLOAT:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_FLOAT");
								break;
		case PR_STRING:			strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"PR_STRING");
								break;
		case 0:					strcpy_s(tipoTokenSalida,sizeof(char)*LONG_TIPO_TOKEN,"-");
								break;
	}

	return tipoTokenSalida;
}


/*ID*/
void iniciarId(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void continuarId(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarId(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,ID);
}


/*CTE_ENTERA o CTE_REAL*/ 
void iniciarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);

	if(caracter == '0')
	{
		error[ERROR_FORMATO_NUMERICO_INVALIDO].estado = TRUE;
	}
}

void continuarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarCteEntera(tokenAAnalizar *tokenActual, char caracter)
{
	error[ERROR_FORMATO_NUMERICO_INVALIDO].estado = FALSE;

	if(atof(tokenActual->token) > MAX_VALOR_CTE_ENTERA)
	{
		error[ERROR_CTE_ENTERA_FUERA_DE_RANGO].estado = TRUE;

		syntaxError(tokenActual,caracter);
	}

	insertarTokenEnTS(tokenActual,CTE_ENTERA);
}

void iniciarCteReal(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void continuarCteReal(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);

	error[ERROR_FORMATO_NUMERICO_INVALIDO].estado = FALSE;
}

void finalizarCteReal(tokenAAnalizar *tokenActual, char caracter)
{
	if(atof(tokenActual->token) < pow(2,-127) || atof(tokenActual->token) > pow(2,129))
	{
		error[ERROR_CTE_REAL_FUERA_DE_RANGO].estado = TRUE;

		syntaxError(tokenActual,caracter);
	}

	insertarTokenEnTS(tokenActual,CTE_REAL);
}


/*CTE_STRING*/ 
void iniciarCteStr(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);

	error[ERROR_CTE_STRING_ABIERTA].estado = TRUE;
}

void continuarCteStr(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);

	if(tokenActual->longitudToken > MAX_LONG_CTE_STRING)
	{
		error[ERROR_CTE_STRING_SUPERA_30].estado = TRUE;

		error[ERROR_CTE_STRING_ABIERTA].estado = FALSE; 

		syntaxError(tokenActual,caracter);
	}
}

void finalizarCteStr(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,CTE_STRING);

	error[ERROR_CTE_STRING_ABIERTA].estado = FALSE; 
}


/*OP_SUMA u OP_CONCATENACION*/
void iniciarOpSumaOConcat(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpSuma(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_SUMA) ;
}

void continuarOpConcat(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpConcat(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_CONCATENACION);
}


/*OP_RESTA o comentario*/ 
void iniciarOpRestaOComent(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpResta(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_RESTA);
}

void continuarComentario(tokenAAnalizar *tokenActual, char caracter)
{
	error[ERROR_COMENTARIO_ABIERTO].estado = TRUE;

	if(caracter == '\n')
	{
		++lineaActual;
	}
}

void finalizarComentario(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->longitudToken = 0;

	error[ERROR_COMENTARIO_ABIERTO].estado = FALSE;
}


/*OP_MULTIPLICACION*/ 
void iniciarOpMulti(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpMulti(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_MULTIPLICACION); 
}


/*OP_DIVISION*/ 
void iniciarOpDivi(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpDivi(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_DIVISION);
}


/*PARENTESIS_ABRE*/ 
void iniciarParAbre(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);;
}

void finalizarParAbre(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,PAR_ABRE) ;
}


/*PARENTESIS_CIERRA*/ 
void iniciarParCierra(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarParCierra(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,PAR_CIERRA) ;
}


/*CORCHETES_ABRE*/ 
void iniciarCorAbre(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarCorAbre(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,COR_ABRE);
}


/*CORCHETES_CIERRA*/ 
void iniciarCorCierra(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarCorCierra(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,COR_CIERRA);
}


/*PUNTO_COMA*/ 
void iniciarPuntoComa(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter); 
}

void finalizarPuntoComa(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,PUNTO_COMA) ;
}


/*DOS_PUNTOS*/ 
void iniciarDosPuntos(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarDosPuntos(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,DOS_PUNTOS) ;
}


/*GUION_BAJO*/ 
void iniciarGuionBajo(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarGuionBajo(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,GUION_BAJO);
}


/*COMA*/ 
void iniciarComa(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarComa(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,COMA);
}


/*OP_MENOR u OP_MENOR_IGUAL*/ 
void iniciarOpMenorOMenorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpMenor(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_MENOR);
}

void continuarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_MENOR_IGUAL);
}


/*OP_MAYOR u OP_MAYOR_IGUAL*/ 
void iniciarOpMayorOMayorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpMayor(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_MAYOR);
}

void continuarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_MAYOR_IGUAL) ;
}


/*OP_ASIGNACION u OP_IGUAL*/ 
void iniciarOpAsigOIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpAsig(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_ASIGNACION); 
}

void continuarOpIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);
}

void finalizarOpIgual(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_IGUAL);
}


/*OP_DISTINTO*/ 
void iniciarOpDistinto(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);

	error[ERROR_OP_DISTINTO_NO_FINALIZADO].estado = TRUE;
}

void continuarOpDistinto(tokenAAnalizar *tokenActual, char caracter)
{
	insertarCaracterEnToken(tokenActual,caracter);

	error[ERROR_OP_DISTINTO_NO_FINALIZADO].estado = FALSE;
}

void finalizarOpDistinto(tokenAAnalizar *tokenActual, char caracter)
{
	insertarTokenEnTS(tokenActual,OP_DISTINTO); 
}


/*Espacios vacíos (ENTER, TAB, ESPACIO)*/ 
void ignorarEspacios(tokenAAnalizar *tokenActual, char caracter)
{
	if(caracter == '\n')
	{
		++lineaActual;
	}
}


/*Error de sintáxis*/ 
void syntaxError(tokenAAnalizar *tokenActual, char caracter)
{
	int i;

	/*Indico el error de sintáxis e imprimo el último token*/
	fprintf(archivoDeTokens,"\nSYNTAX ERROR\nTOKEN ACTUAL: %s\nDESCRIPCION: ",tokenActual->token);

	/*Busco cual es el error*/
	for(i = 0; i < CANT_ERRORES; ++i)
	{
		if(error[i].estado == TRUE)
		{
			/*Escribo el error en el archivo de tokens identificados*/
			fprintf(archivoDeTokens,"%s",error[i].descripcion);
		}
	}

	/*Cierro el arcivo fuente*/
	fclose(archivoFuente);
	
	/*Cierro el archivo de tokens identificados*/
	fclose(archivoDeTokens);

	/*Cierro el archivo de salida del analizador sintactico*/
	fclose(salidaAS);

	/*Termino el programa*/
	exit(1);
}


void insertarCaracterEnToken(tokenAAnalizar *tokenActual, char caracter)
{
	tokenActual->token[tokenActual->longitudToken] = caracter;
	tokenActual->longitudToken++;
	tokenActual->token[tokenActual->longitudToken] = '\0'; 
}


void inicializarMatrices()
{
    /*ESTADO 0*/
    /* 0 */nuevoEstado[0][0]  = 30;             proceso[0][0]  = iniciarCteEnteraOReal;
    /*1-9*/nuevoEstado[0][1]  = 21;             proceso[0][1]  = iniciarCteEnteraOReal;
    /*a-Z*/nuevoEstado[0][2]  = 7;              proceso[0][2]  = iniciarId;
    /* ( */nuevoEstado[0][3]  = 1;              proceso[0][3]  = iniciarParAbre;
    /* ) */nuevoEstado[0][4]  = 2;              proceso[0][4]  = iniciarParCierra;
    /* [ */nuevoEstado[0][5]  = 4;              proceso[0][5]  = iniciarCorAbre;
    /* ] */nuevoEstado[0][6]  = 3;              proceso[0][6]  = iniciarCorCierra;
    /* ; */nuevoEstado[0][7]  = 5;              proceso[0][7]  = iniciarPuntoComa;
    /* : */nuevoEstado[0][8]  = 6;              proceso[0][8]  = iniciarDosPuntos;
    /* " */nuevoEstado[0][9]  = 8;              proceso[0][9]  = iniciarCteStr;
    /* < */nuevoEstado[0][10] = 10;             proceso[0][10] = iniciarOpMenorOMenorIgual;
    /* > */nuevoEstado[0][11] = 12;             proceso[0][11] = iniciarOpMayorOMayorIgual;
    /* = */nuevoEstado[0][12] = 14;             proceso[0][12] = iniciarOpAsigOIgual;
    /* ! */nuevoEstado[0][13] = 16;             proceso[0][13] = iniciarOpDistinto;
    /* + */nuevoEstado[0][14] = 18;             proceso[0][14] = iniciarOpSumaOConcat;
    /* * */nuevoEstado[0][15] = 20;             proceso[0][15] = iniciarOpMulti;
    /* / */nuevoEstado[0][16] = 24;             proceso[0][16] = iniciarOpDivi;
    /* - */nuevoEstado[0][17] = 25;             proceso[0][17] = iniciarOpRestaOComent;
    /* , */nuevoEstado[0][18] = 28;             proceso[0][18] = iniciarComa;
    /* _ */nuevoEstado[0][19] = 29;             proceso[0][19] = iniciarGuionBajo;
    /* . */nuevoEstado[0][20] = 22;             proceso[0][20] = iniciarCteReal;
    /*$#@*/nuevoEstado[0][21] = SYNTAX_ERROR;   proceso[0][21] = syntaxError;
    /*esp*/nuevoEstado[0][22] = 0;              proceso[0][22] = ignorarEspacios;

    /*ESTADO 1*/
    /* 0 */nuevoEstado[1][0]  = ESTADO_FINAL;   proceso[1][0]  = finalizarParAbre;
    /*1-9*/nuevoEstado[1][1]  = ESTADO_FINAL;   proceso[1][1]  = finalizarParAbre;
    /*a-Z*/nuevoEstado[1][2]  = ESTADO_FINAL;   proceso[1][2]  = finalizarParAbre;
    /* ( */nuevoEstado[1][3]  = ESTADO_FINAL;   proceso[1][3]  = finalizarParAbre;
    /* ) */nuevoEstado[1][4]  = ESTADO_FINAL;   proceso[1][4]  = finalizarParAbre;
    /* [ */nuevoEstado[1][5]  = ESTADO_FINAL;   proceso[1][5]  = finalizarParAbre;
    /* ] */nuevoEstado[1][6]  = ESTADO_FINAL;   proceso[1][6]  = finalizarParAbre;
    /* ; */nuevoEstado[1][7]  = ESTADO_FINAL;   proceso[1][7]  = finalizarParAbre;
    /* : */nuevoEstado[1][8]  = ESTADO_FINAL;   proceso[1][8]  = finalizarParAbre;
    /* " */nuevoEstado[1][9]  = ESTADO_FINAL;   proceso[1][9]  = finalizarParAbre;
    /* < */nuevoEstado[1][10] = ESTADO_FINAL;   proceso[1][10] = finalizarParAbre;
    /* > */nuevoEstado[1][11] = ESTADO_FINAL;   proceso[1][11] = finalizarParAbre;
    /* = */nuevoEstado[1][12] = ESTADO_FINAL;   proceso[1][12] = finalizarParAbre;
    /* ! */nuevoEstado[1][13] = ESTADO_FINAL;   proceso[1][13] = finalizarParAbre;
    /* + */nuevoEstado[1][14] = ESTADO_FINAL;   proceso[1][14] = finalizarParAbre;
    /* * */nuevoEstado[1][15] = ESTADO_FINAL;   proceso[1][15] = finalizarParAbre;
    /* / */nuevoEstado[1][16] = ESTADO_FINAL;   proceso[1][16] = finalizarParAbre;
    /* - */nuevoEstado[1][17] = ESTADO_FINAL;   proceso[1][17] = finalizarParAbre;
    /* , */nuevoEstado[1][18] = ESTADO_FINAL;   proceso[1][18] = finalizarParAbre;
    /* _ */nuevoEstado[1][19] = ESTADO_FINAL;   proceso[1][19] = finalizarParAbre;
    /* . */nuevoEstado[1][20] = ESTADO_FINAL;   proceso[1][20] = finalizarParAbre;
    /*$#@*/nuevoEstado[1][21] = ESTADO_FINAL;   proceso[1][21] = finalizarParAbre;
    /*esp*/nuevoEstado[1][22] = ESTADO_FINAL;   proceso[1][22] = finalizarParAbre;   

    /*ESTADO 2*/
    /* 0 */nuevoEstado[2][0]  = ESTADO_FINAL;   proceso[2][0]  = finalizarParCierra;
    /*1-9*/nuevoEstado[2][1]  = ESTADO_FINAL;   proceso[2][1]  = finalizarParCierra;
    /*a-Z*/nuevoEstado[2][2]  = ESTADO_FINAL;   proceso[2][2]  = finalizarParCierra;
    /* ( */nuevoEstado[2][3]  = ESTADO_FINAL;   proceso[2][3]  = finalizarParCierra;
    /* ) */nuevoEstado[2][4]  = ESTADO_FINAL;   proceso[2][4]  = finalizarParCierra;
    /* [ */nuevoEstado[2][5]  = ESTADO_FINAL;   proceso[2][5]  = finalizarParCierra;
    /* ] */nuevoEstado[2][6]  = ESTADO_FINAL;   proceso[2][6]  = finalizarParCierra;
    /* ; */nuevoEstado[2][7]  = ESTADO_FINAL;   proceso[2][7]  = finalizarParCierra;
    /* : */nuevoEstado[2][8]  = ESTADO_FINAL;   proceso[2][8]  = finalizarParCierra;
    /* " */nuevoEstado[2][9]  = ESTADO_FINAL;   proceso[2][9]  = finalizarParCierra;
    /* < */nuevoEstado[2][10] = ESTADO_FINAL;   proceso[2][10] = finalizarParCierra;
    /* > */nuevoEstado[2][11] = ESTADO_FINAL;   proceso[2][11] = finalizarParCierra;
    /* = */nuevoEstado[2][12] = ESTADO_FINAL;   proceso[2][12] = finalizarParCierra;
    /* ! */nuevoEstado[2][13] = ESTADO_FINAL;   proceso[2][13] = finalizarParCierra;
    /* + */nuevoEstado[2][14] = ESTADO_FINAL;   proceso[2][14] = finalizarParCierra;
    /* * */nuevoEstado[2][15] = ESTADO_FINAL;   proceso[2][15] = finalizarParCierra;
    /* / */nuevoEstado[2][16] = ESTADO_FINAL;   proceso[2][16] = finalizarParCierra;
    /* - */nuevoEstado[2][17] = ESTADO_FINAL;   proceso[2][17] = finalizarParCierra;
    /* , */nuevoEstado[2][18] = ESTADO_FINAL;   proceso[2][18] = finalizarParCierra;
    /* _ */nuevoEstado[2][19] = ESTADO_FINAL;   proceso[2][19] = finalizarParCierra;
    /* . */nuevoEstado[2][20] = ESTADO_FINAL;   proceso[2][20] = finalizarParCierra;
    /*$#@*/nuevoEstado[2][21] = ESTADO_FINAL;   proceso[2][21] = finalizarParCierra;
    /*esp*/nuevoEstado[2][22] = ESTADO_FINAL;   proceso[2][22] = finalizarParCierra;   

    /*ESTADO 3*/
    /* 0 */nuevoEstado[3][0]  = ESTADO_FINAL;   proceso[3][0]  = finalizarCorCierra;
    /*1-9*/nuevoEstado[3][1]  = ESTADO_FINAL;   proceso[3][1]  = finalizarCorCierra;
    /*a-Z*/nuevoEstado[3][2]  = ESTADO_FINAL;   proceso[3][2]  = finalizarCorCierra;
    /* ( */nuevoEstado[3][3]  = ESTADO_FINAL;   proceso[3][3]  = finalizarCorCierra;
    /* ) */nuevoEstado[3][4]  = ESTADO_FINAL;   proceso[3][4]  = finalizarCorCierra;
    /* [ */nuevoEstado[3][5]  = ESTADO_FINAL;   proceso[3][5]  = finalizarCorCierra;
    /* ] */nuevoEstado[3][6]  = ESTADO_FINAL;   proceso[3][6]  = finalizarCorCierra;
    /* ; */nuevoEstado[3][7]  = ESTADO_FINAL;   proceso[3][7]  = finalizarCorCierra;
    /* : */nuevoEstado[3][8]  = ESTADO_FINAL;   proceso[3][8]  = finalizarCorCierra;
    /* " */nuevoEstado[3][9]  = ESTADO_FINAL;   proceso[3][9]  = finalizarCorCierra;
    /* < */nuevoEstado[3][10] = ESTADO_FINAL;   proceso[3][10] = finalizarCorCierra;
    /* > */nuevoEstado[3][11] = ESTADO_FINAL;   proceso[3][11] = finalizarCorCierra;
    /* = */nuevoEstado[3][12] = ESTADO_FINAL;   proceso[3][12] = finalizarCorCierra;
    /* ! */nuevoEstado[3][13] = ESTADO_FINAL;   proceso[3][13] = finalizarCorCierra;
    /* + */nuevoEstado[3][14] = ESTADO_FINAL;   proceso[3][14] = finalizarCorCierra;
    /* * */nuevoEstado[3][15] = ESTADO_FINAL;   proceso[3][15] = finalizarCorCierra;
    /* / */nuevoEstado[3][16] = ESTADO_FINAL;   proceso[3][16] = finalizarCorCierra;
    /* - */nuevoEstado[3][17] = ESTADO_FINAL;   proceso[3][17] = finalizarCorCierra;
    /* , */nuevoEstado[3][18] = ESTADO_FINAL;   proceso[3][18] = finalizarCorCierra;
    /* _ */nuevoEstado[3][19] = ESTADO_FINAL;   proceso[3][19] = finalizarCorCierra;
    /* . */nuevoEstado[3][20] = ESTADO_FINAL;   proceso[3][20] = finalizarCorCierra;
    /*$#@*/nuevoEstado[3][21] = ESTADO_FINAL;   proceso[3][21] = finalizarCorCierra;
    /*esp*/nuevoEstado[3][22] = ESTADO_FINAL;   proceso[3][22] = finalizarCorCierra;

    /*ESTADO 4*/
    /* 0 */nuevoEstado[4][0]  = ESTADO_FINAL;   proceso[4][0]  = finalizarCorAbre;
    /*1-9*/nuevoEstado[4][1]  = ESTADO_FINAL;   proceso[4][1]  = finalizarCorAbre;
    /*a-Z*/nuevoEstado[4][2]  = ESTADO_FINAL;   proceso[4][2]  = finalizarCorAbre;
    /* ( */nuevoEstado[4][3]  = ESTADO_FINAL;   proceso[4][3]  = finalizarCorAbre;
    /* ) */nuevoEstado[4][4]  = ESTADO_FINAL;   proceso[4][4]  = finalizarCorAbre;
    /* [ */nuevoEstado[4][5]  = ESTADO_FINAL;   proceso[4][5]  = finalizarCorAbre;
    /* ] */nuevoEstado[4][6]  = ESTADO_FINAL;   proceso[4][6]  = finalizarCorAbre;
    /* ; */nuevoEstado[4][7]  = ESTADO_FINAL;   proceso[4][7]  = finalizarCorAbre;
    /* : */nuevoEstado[4][8]  = ESTADO_FINAL;   proceso[4][8]  = finalizarCorAbre;
    /* " */nuevoEstado[4][9]  = ESTADO_FINAL;   proceso[4][9]  = finalizarCorAbre;
    /* < */nuevoEstado[4][10] = ESTADO_FINAL;   proceso[4][10] = finalizarCorAbre;
    /* > */nuevoEstado[4][11] = ESTADO_FINAL;   proceso[4][11] = finalizarCorAbre;
    /* = */nuevoEstado[4][12] = ESTADO_FINAL;   proceso[4][12] = finalizarCorAbre;
    /* ! */nuevoEstado[4][13] = ESTADO_FINAL;   proceso[4][13] = finalizarCorAbre;
    /* + */nuevoEstado[4][14] = ESTADO_FINAL;   proceso[4][14] = finalizarCorAbre;
    /* * */nuevoEstado[4][15] = ESTADO_FINAL;   proceso[4][15] = finalizarCorAbre;
    /* / */nuevoEstado[4][16] = ESTADO_FINAL;   proceso[4][16] = finalizarCorAbre;
    /* - */nuevoEstado[4][17] = ESTADO_FINAL;   proceso[4][17] = finalizarCorAbre;
    /* , */nuevoEstado[4][18] = ESTADO_FINAL;   proceso[4][18] = finalizarCorAbre;
    /* _ */nuevoEstado[4][19] = ESTADO_FINAL;   proceso[4][19] = finalizarCorAbre;
    /* . */nuevoEstado[4][20] = ESTADO_FINAL;   proceso[4][20] = finalizarCorAbre;
    /*$#@*/nuevoEstado[4][21] = ESTADO_FINAL;   proceso[4][21] = finalizarCorAbre;
    /*esp*/nuevoEstado[4][22] = ESTADO_FINAL;   proceso[4][22] = finalizarCorAbre;    

    /*ESTADO 5*/
    /* 0 */nuevoEstado[5][0]  = ESTADO_FINAL;   proceso[5][0]  = finalizarPuntoComa;
    /*1-9*/nuevoEstado[5][1]  = ESTADO_FINAL;   proceso[5][1]  = finalizarPuntoComa;
    /*a-Z*/nuevoEstado[5][2]  = ESTADO_FINAL;   proceso[5][2]  = finalizarPuntoComa;
    /* ( */nuevoEstado[5][3]  = ESTADO_FINAL;   proceso[5][3]  = finalizarPuntoComa;
    /* ) */nuevoEstado[5][4]  = ESTADO_FINAL;   proceso[5][4]  = finalizarPuntoComa;
    /* [ */nuevoEstado[5][5]  = ESTADO_FINAL;   proceso[5][5]  = finalizarPuntoComa;
    /* ] */nuevoEstado[5][6]  = ESTADO_FINAL;   proceso[5][6]  = finalizarPuntoComa;
    /* ; */nuevoEstado[5][7]  = ESTADO_FINAL;   proceso[5][7]  = finalizarPuntoComa;
    /* : */nuevoEstado[5][8]  = ESTADO_FINAL;   proceso[5][8]  = finalizarPuntoComa;
    /* " */nuevoEstado[5][9]  = ESTADO_FINAL;   proceso[5][9]  = finalizarPuntoComa;
    /* < */nuevoEstado[5][10] = ESTADO_FINAL;   proceso[5][10] = finalizarPuntoComa;
    /* > */nuevoEstado[5][11] = ESTADO_FINAL;   proceso[5][11] = finalizarPuntoComa;
    /* = */nuevoEstado[5][12] = ESTADO_FINAL;   proceso[5][12] = finalizarPuntoComa;
    /* ! */nuevoEstado[5][13] = ESTADO_FINAL;   proceso[5][13] = finalizarPuntoComa;
    /* + */nuevoEstado[5][14] = ESTADO_FINAL;   proceso[5][14] = finalizarPuntoComa;
    /* * */nuevoEstado[5][15] = ESTADO_FINAL;   proceso[5][15] = finalizarPuntoComa;
    /* / */nuevoEstado[5][16] = ESTADO_FINAL;   proceso[5][16] = finalizarPuntoComa;
    /* - */nuevoEstado[5][17] = ESTADO_FINAL;   proceso[5][17] = finalizarPuntoComa;
    /* , */nuevoEstado[5][18] = ESTADO_FINAL;   proceso[5][18] = finalizarPuntoComa;
    /* _ */nuevoEstado[5][19] = ESTADO_FINAL;   proceso[5][19] = finalizarPuntoComa;
    /* . */nuevoEstado[5][20] = ESTADO_FINAL;   proceso[5][20] = finalizarPuntoComa;
    /*$#@*/nuevoEstado[5][21] = ESTADO_FINAL;   proceso[5][21] = finalizarPuntoComa;
    /*esp*/nuevoEstado[5][22] = ESTADO_FINAL;   proceso[5][22] = finalizarPuntoComa;

    /*ESTADO 6*/
    /* 0 */nuevoEstado[6][0]  = ESTADO_FINAL;   proceso[6][0]  = finalizarDosPuntos;
    /*1-9*/nuevoEstado[6][1]  = ESTADO_FINAL;   proceso[6][1]  = finalizarDosPuntos;
    /*a-Z*/nuevoEstado[6][2]  = ESTADO_FINAL;   proceso[6][2]  = finalizarDosPuntos;
    /* ( */nuevoEstado[6][3]  = ESTADO_FINAL;   proceso[6][3]  = finalizarDosPuntos;
    /* ) */nuevoEstado[6][4]  = ESTADO_FINAL;   proceso[6][4]  = finalizarDosPuntos;
    /* [ */nuevoEstado[6][5]  = ESTADO_FINAL;   proceso[6][5]  = finalizarDosPuntos;
    /* ] */nuevoEstado[6][6]  = ESTADO_FINAL;   proceso[6][6]  = finalizarDosPuntos;
    /* ; */nuevoEstado[6][7]  = ESTADO_FINAL;   proceso[6][7]  = finalizarDosPuntos;
    /* : */nuevoEstado[6][8]  = ESTADO_FINAL;   proceso[6][8]  = finalizarDosPuntos;
    /* " */nuevoEstado[6][9]  = ESTADO_FINAL;   proceso[6][9]  = finalizarDosPuntos;
    /* < */nuevoEstado[6][10] = ESTADO_FINAL;   proceso[6][10] = finalizarDosPuntos;
    /* > */nuevoEstado[6][11] = ESTADO_FINAL;   proceso[6][11] = finalizarDosPuntos;
    /* = */nuevoEstado[6][12] = ESTADO_FINAL;   proceso[6][12] = finalizarDosPuntos;
    /* ! */nuevoEstado[6][13] = ESTADO_FINAL;   proceso[6][13] = finalizarDosPuntos;
    /* + */nuevoEstado[6][14] = ESTADO_FINAL;   proceso[6][14] = finalizarDosPuntos;
    /* * */nuevoEstado[6][15] = ESTADO_FINAL;   proceso[6][15] = finalizarDosPuntos;
    /* / */nuevoEstado[6][16] = ESTADO_FINAL;   proceso[6][16] = finalizarDosPuntos;
    /* - */nuevoEstado[6][17] = ESTADO_FINAL;   proceso[6][17] = finalizarDosPuntos;
    /* , */nuevoEstado[6][18] = ESTADO_FINAL;   proceso[6][18] = finalizarDosPuntos;
    /* _ */nuevoEstado[6][19] = ESTADO_FINAL;   proceso[6][19] = finalizarDosPuntos;
    /* . */nuevoEstado[6][20] = ESTADO_FINAL;   proceso[6][20] = finalizarDosPuntos;
    /*$#@*/nuevoEstado[6][21] = ESTADO_FINAL;   proceso[6][21] = finalizarDosPuntos;
    /*esp*/nuevoEstado[6][22] = ESTADO_FINAL;   proceso[6][22] = finalizarDosPuntos;

    /*ESTADO 7*/
    /* 0 */nuevoEstado[7][0]  = 7;              proceso[7][0]  = continuarId;
    /*1-9*/nuevoEstado[7][1]  = 7;              proceso[7][1]  = continuarId;
    /*a-Z*/nuevoEstado[7][2]  = 7;              proceso[7][2]  = continuarId;
    /* ( */nuevoEstado[7][3]  = ESTADO_FINAL;   proceso[7][3]  = finalizarId;
    /* ) */nuevoEstado[7][4]  = ESTADO_FINAL;   proceso[7][4]  = finalizarId;
    /* [ */nuevoEstado[7][5]  = ESTADO_FINAL;   proceso[7][5]  = finalizarId;
    /* ] */nuevoEstado[7][6]  = ESTADO_FINAL;   proceso[7][6]  = finalizarId;
    /* ; */nuevoEstado[7][7]  = ESTADO_FINAL;   proceso[7][7]  = finalizarId;
    /* : */nuevoEstado[7][8]  = ESTADO_FINAL;   proceso[7][8]  = finalizarId;
    /* " */nuevoEstado[7][9]  = ESTADO_FINAL;   proceso[7][9]  = finalizarId;
    /* < */nuevoEstado[7][10] = ESTADO_FINAL;   proceso[7][10] = finalizarId;
    /* > */nuevoEstado[7][11] = ESTADO_FINAL;   proceso[7][11] = finalizarId;
    /* = */nuevoEstado[7][12] = ESTADO_FINAL;   proceso[7][12] = finalizarId;
    /* ! */nuevoEstado[7][13] = ESTADO_FINAL;   proceso[7][13] = finalizarId;
    /* + */nuevoEstado[7][14] = ESTADO_FINAL;   proceso[7][14] = finalizarId;
    /* * */nuevoEstado[7][15] = ESTADO_FINAL;   proceso[7][15] = finalizarId;
    /* / */nuevoEstado[7][16] = ESTADO_FINAL;   proceso[7][16] = finalizarId;
    /* - */nuevoEstado[7][17] = ESTADO_FINAL;   proceso[7][17] = finalizarId;
    /* , */nuevoEstado[7][18] = ESTADO_FINAL;   proceso[7][18] = finalizarId;
    /* _ */nuevoEstado[7][19] = ESTADO_FINAL;   proceso[7][19] = finalizarId;
    /* . */nuevoEstado[7][20] = ESTADO_FINAL;   proceso[7][20] = finalizarId;
    /*$#@*/nuevoEstado[7][21] = ESTADO_FINAL;   proceso[7][21] = finalizarId;
    /*esp*/nuevoEstado[7][22] = ESTADO_FINAL;   proceso[7][22] = finalizarId;

    /*ESTADO 8*/
    /* 0 */nuevoEstado[8][0]  = 8;              proceso[8][0]  = continuarCteStr;
    /*1-9*/nuevoEstado[8][1]  = 8;              proceso[8][1]  = continuarCteStr;
    /*a-Z*/nuevoEstado[8][2]  = 8;              proceso[8][2]  = continuarCteStr;
    /* ( */nuevoEstado[8][3]  = 8;              proceso[8][3]  = continuarCteStr;
    /* ) */nuevoEstado[8][4]  = 8;              proceso[8][4]  = continuarCteStr;
    /* [ */nuevoEstado[8][5]  = 8;              proceso[8][5]  = continuarCteStr;
    /* ] */nuevoEstado[8][6]  = 8;              proceso[8][6]  = continuarCteStr;
    /* ; */nuevoEstado[8][7]  = 8;              proceso[8][7]  = continuarCteStr;
    /* : */nuevoEstado[8][8]  = 8;              proceso[8][8]  = continuarCteStr;
    /* " */nuevoEstado[8][9]  = 9;              proceso[8][9]  = continuarCteStr;
    /* < */nuevoEstado[8][10] = 8;              proceso[8][10] = continuarCteStr;
    /* > */nuevoEstado[8][11] = 8;              proceso[8][11] = continuarCteStr;
    /* = */nuevoEstado[8][12] = 8;              proceso[8][12] = continuarCteStr;
    /* ! */nuevoEstado[8][13] = 8;              proceso[8][13] = continuarCteStr;
    /* + */nuevoEstado[8][14] = 8;              proceso[8][14] = continuarCteStr;
    /* * */nuevoEstado[8][15] = 8;              proceso[8][15] = continuarCteStr;
    /* / */nuevoEstado[8][16] = 8;              proceso[8][16] = continuarCteStr;
    /* - */nuevoEstado[8][17] = 8;              proceso[8][17] = continuarCteStr;
    /* , */nuevoEstado[8][18] = 8;              proceso[8][18] = continuarCteStr;
    /* _ */nuevoEstado[8][19] = 8;              proceso[8][19] = continuarCteStr;
    /* . */nuevoEstado[8][20] = 8;              proceso[8][20] = continuarCteStr;
    /*$#@*/nuevoEstado[8][21] = 8;              proceso[8][21] = continuarCteStr;
    /*esp*/nuevoEstado[8][22] = 8;              proceso[8][22] = continuarCteStr;        

    /*ESTADO 9*/
    /* 0 */nuevoEstado[9][0]  = ESTADO_FINAL;   proceso[9][0]  = finalizarCteStr;
    /*1-9*/nuevoEstado[9][1]  = ESTADO_FINAL;   proceso[9][1]  = finalizarCteStr;
    /*a-Z*/nuevoEstado[9][2]  = ESTADO_FINAL;   proceso[9][2]  = finalizarCteStr;
    /* ( */nuevoEstado[9][3]  = ESTADO_FINAL;   proceso[9][3]  = finalizarCteStr;
    /* ) */nuevoEstado[9][4]  = ESTADO_FINAL;   proceso[9][4]  = finalizarCteStr;
    /* [ */nuevoEstado[9][5]  = ESTADO_FINAL;   proceso[9][5]  = finalizarCteStr;
    /* ] */nuevoEstado[9][6]  = ESTADO_FINAL;   proceso[9][6]  = finalizarCteStr;
    /* ; */nuevoEstado[9][7]  = ESTADO_FINAL;   proceso[9][7]  = finalizarCteStr;
    /* : */nuevoEstado[9][8]  = ESTADO_FINAL;   proceso[9][8]  = finalizarCteStr;
    /* " */nuevoEstado[9][9]  = ESTADO_FINAL;   proceso[9][9]  = finalizarCteStr;
    /* < */nuevoEstado[9][10] = ESTADO_FINAL;   proceso[9][10] = finalizarCteStr;
    /* > */nuevoEstado[9][11] = ESTADO_FINAL;   proceso[9][11] = finalizarCteStr;
    /* = */nuevoEstado[9][12] = ESTADO_FINAL;   proceso[9][12] = finalizarCteStr;
    /* ! */nuevoEstado[9][13] = ESTADO_FINAL;   proceso[9][13] = finalizarCteStr;
    /* + */nuevoEstado[9][14] = ESTADO_FINAL;   proceso[9][14] = finalizarCteStr;
    /* * */nuevoEstado[9][15] = ESTADO_FINAL;   proceso[9][15] = finalizarCteStr;
    /* / */nuevoEstado[9][16] = ESTADO_FINAL;   proceso[9][16] = finalizarCteStr;
    /* - */nuevoEstado[9][17] = ESTADO_FINAL;   proceso[9][17] = finalizarCteStr;
    /* , */nuevoEstado[9][18] = ESTADO_FINAL;   proceso[9][18] = finalizarCteStr;
    /* _ */nuevoEstado[9][19] = ESTADO_FINAL;   proceso[9][19] = finalizarCteStr;
    /* . */nuevoEstado[9][20] = ESTADO_FINAL;   proceso[9][20] = finalizarCteStr;
    /*$#@*/nuevoEstado[9][21] = ESTADO_FINAL;   proceso[9][21] = finalizarCteStr;
    /*esp*/nuevoEstado[9][22] = ESTADO_FINAL;   proceso[9][22] = finalizarCteStr;

    /*ESTADO 10*/
    /* 0 */nuevoEstado[10][0]  = ESTADO_FINAL;   proceso[10][0]  = finalizarOpMenor;
    /*1-9*/nuevoEstado[10][1]  = ESTADO_FINAL;   proceso[10][1]  = finalizarOpMenor;
    /*a-Z*/nuevoEstado[10][2]  = ESTADO_FINAL;   proceso[10][2]  = finalizarOpMenor;
    /* ( */nuevoEstado[10][3]  = ESTADO_FINAL;   proceso[10][3]  = finalizarOpMenor;
    /* ) */nuevoEstado[10][4]  = ESTADO_FINAL;   proceso[10][4]  = finalizarOpMenor;
    /* [ */nuevoEstado[10][5]  = ESTADO_FINAL;   proceso[10][5]  = finalizarOpMenor;
    /* ] */nuevoEstado[10][6]  = ESTADO_FINAL;   proceso[10][6]  = finalizarOpMenor;
    /* ; */nuevoEstado[10][7]  = ESTADO_FINAL;   proceso[10][7]  = finalizarOpMenor;
    /* : */nuevoEstado[10][8]  = ESTADO_FINAL;   proceso[10][8]  = finalizarOpMenor;
    /* " */nuevoEstado[10][9]  = ESTADO_FINAL;   proceso[10][9]  = finalizarOpMenor;
    /* < */nuevoEstado[10][10] = ESTADO_FINAL;   proceso[10][10] = finalizarOpMenor;
    /* > */nuevoEstado[10][11] = ESTADO_FINAL;   proceso[10][11] = finalizarOpMenor;
    /* = */nuevoEstado[10][12] = 11;             proceso[10][12] = continuarOpMenorIgual;
    /* ! */nuevoEstado[10][13] = ESTADO_FINAL;   proceso[10][13] = finalizarOpMenor;
    /* + */nuevoEstado[10][14] = ESTADO_FINAL;   proceso[10][14] = finalizarOpMenor;
    /* * */nuevoEstado[10][15] = ESTADO_FINAL;   proceso[10][15] = finalizarOpMenor;
    /* / */nuevoEstado[10][16] = ESTADO_FINAL;   proceso[10][16] = finalizarOpMenor;
    /* - */nuevoEstado[10][17] = ESTADO_FINAL;   proceso[10][17] = finalizarOpMenor;
    /* , */nuevoEstado[10][18] = ESTADO_FINAL;   proceso[10][18] = finalizarOpMenor;
    /* _ */nuevoEstado[10][19] = ESTADO_FINAL;   proceso[10][19] = finalizarOpMenor;
    /* . */nuevoEstado[10][20] = ESTADO_FINAL;   proceso[10][20] = finalizarOpMenor;
    /*$#@*/nuevoEstado[10][21] = ESTADO_FINAL;   proceso[10][21] = finalizarOpMenor;
    /*esp*/nuevoEstado[10][22] = ESTADO_FINAL;   proceso[10][22] = finalizarOpMenor;

    /*ESTADO 11*/
    /* 0 */nuevoEstado[11][0]  = ESTADO_FINAL;   proceso[11][0]  = finalizarOpMenorIgual;
    /*1-9*/nuevoEstado[11][1]  = ESTADO_FINAL;   proceso[11][1]  = finalizarOpMenorIgual;
    /*a-Z*/nuevoEstado[11][2]  = ESTADO_FINAL;   proceso[11][2]  = finalizarOpMenorIgual;
    /* ( */nuevoEstado[11][3]  = ESTADO_FINAL;   proceso[11][3]  = finalizarOpMenorIgual;
    /* ) */nuevoEstado[11][4]  = ESTADO_FINAL;   proceso[11][4]  = finalizarOpMenorIgual;
    /* [ */nuevoEstado[11][5]  = ESTADO_FINAL;   proceso[11][5]  = finalizarOpMenorIgual;
    /* ] */nuevoEstado[11][6]  = ESTADO_FINAL;   proceso[11][6]  = finalizarOpMenorIgual;
    /* ; */nuevoEstado[11][7]  = ESTADO_FINAL;   proceso[11][7]  = finalizarOpMenorIgual;
    /* : */nuevoEstado[11][8]  = ESTADO_FINAL;   proceso[11][8]  = finalizarOpMenorIgual;
    /* " */nuevoEstado[11][9]  = ESTADO_FINAL;   proceso[11][9]  = finalizarOpMenorIgual;
    /* < */nuevoEstado[11][10] = ESTADO_FINAL;   proceso[11][10] = finalizarOpMenorIgual;
    /* > */nuevoEstado[11][11] = ESTADO_FINAL;   proceso[11][11] = finalizarOpMenorIgual;
    /* = */nuevoEstado[11][12] = ESTADO_FINAL;   proceso[11][12] = finalizarOpMenorIgual;
    /* ! */nuevoEstado[11][13] = ESTADO_FINAL;   proceso[11][13] = finalizarOpMenorIgual;
    /* + */nuevoEstado[11][14] = ESTADO_FINAL;   proceso[11][14] = finalizarOpMenorIgual;
    /* * */nuevoEstado[11][15] = ESTADO_FINAL;   proceso[11][15] = finalizarOpMenorIgual;
    /* / */nuevoEstado[11][16] = ESTADO_FINAL;   proceso[11][16] = finalizarOpMenorIgual;
    /* - */nuevoEstado[11][17] = ESTADO_FINAL;   proceso[11][17] = finalizarOpMenorIgual;
    /* , */nuevoEstado[11][18] = ESTADO_FINAL;   proceso[11][18] = finalizarOpMenorIgual;
    /* _ */nuevoEstado[11][19] = ESTADO_FINAL;   proceso[11][19] = finalizarOpMenorIgual;
    /* . */nuevoEstado[11][20] = ESTADO_FINAL;   proceso[11][20] = finalizarOpMenorIgual;
    /*$#@*/nuevoEstado[11][21] = ESTADO_FINAL;   proceso[11][21] = finalizarOpMenorIgual;
    /*esp*/nuevoEstado[11][22] = ESTADO_FINAL;   proceso[11][22] = finalizarOpMenorIgual;

    /*ESTADO 12*/
    /* 0 */nuevoEstado[12][0]  = ESTADO_FINAL;   proceso[12][0]  = finalizarOpMayor;
    /*1-9*/nuevoEstado[12][1]  = ESTADO_FINAL;   proceso[12][1]  = finalizarOpMayor;
    /*a-Z*/nuevoEstado[12][2]  = ESTADO_FINAL;   proceso[12][2]  = finalizarOpMayor;
    /* ( */nuevoEstado[12][3]  = ESTADO_FINAL;   proceso[12][3]  = finalizarOpMayor;
    /* ) */nuevoEstado[12][4]  = ESTADO_FINAL;   proceso[12][4]  = finalizarOpMayor;
    /* [ */nuevoEstado[12][5]  = ESTADO_FINAL;   proceso[12][5]  = finalizarOpMayor;
    /* ] */nuevoEstado[12][6]  = ESTADO_FINAL;   proceso[12][6]  = finalizarOpMayor;
    /* ; */nuevoEstado[12][7]  = ESTADO_FINAL;   proceso[12][7]  = finalizarOpMayor;
    /* : */nuevoEstado[12][8]  = ESTADO_FINAL;   proceso[12][8]  = finalizarOpMayor;
    /* " */nuevoEstado[12][9]  = ESTADO_FINAL;   proceso[12][9]  = finalizarOpMayor;
    /* < */nuevoEstado[12][10] = ESTADO_FINAL;   proceso[12][10] = finalizarOpMayor;
    /* > */nuevoEstado[12][11] = ESTADO_FINAL;   proceso[12][11] = finalizarOpMayor;
    /* = */nuevoEstado[12][12] = 13;             proceso[12][12] = continuarOpMayorIgual;
    /* ! */nuevoEstado[12][13] = ESTADO_FINAL;   proceso[12][13] = finalizarOpMayor;
    /* + */nuevoEstado[12][14] = ESTADO_FINAL;   proceso[12][14] = finalizarOpMayor;
    /* * */nuevoEstado[12][15] = ESTADO_FINAL;   proceso[12][15] = finalizarOpMayor;
    /* / */nuevoEstado[12][16] = ESTADO_FINAL;   proceso[12][16] = finalizarOpMayor;
    /* - */nuevoEstado[12][17] = ESTADO_FINAL;   proceso[12][17] = finalizarOpMayor;
    /* , */nuevoEstado[12][18] = ESTADO_FINAL;   proceso[12][18] = finalizarOpMayor;
    /* _ */nuevoEstado[12][19] = ESTADO_FINAL;   proceso[12][19] = finalizarOpMayor;
    /* . */nuevoEstado[12][20] = ESTADO_FINAL;   proceso[12][20] = finalizarOpMayor;
    /*$#@*/nuevoEstado[12][21] = ESTADO_FINAL;   proceso[12][21] = finalizarOpMayor;
    /*esp*/nuevoEstado[12][22] = ESTADO_FINAL;   proceso[12][22] = finalizarOpMayor;

    /*ESTADO 13*/
    /* 0 */nuevoEstado[13][0]  = ESTADO_FINAL;   proceso[13][0]  = finalizarOpMayorIgual;
    /*1-9*/nuevoEstado[13][1]  = ESTADO_FINAL;   proceso[13][1]  = finalizarOpMayorIgual;
    /*a-Z*/nuevoEstado[13][2]  = ESTADO_FINAL;   proceso[13][2]  = finalizarOpMayorIgual;
    /* ( */nuevoEstado[13][3]  = ESTADO_FINAL;   proceso[13][3]  = finalizarOpMayorIgual;
    /* ) */nuevoEstado[13][4]  = ESTADO_FINAL;   proceso[13][4]  = finalizarOpMayorIgual;
    /* [ */nuevoEstado[13][5]  = ESTADO_FINAL;   proceso[13][5]  = finalizarOpMayorIgual;
    /* ] */nuevoEstado[13][6]  = ESTADO_FINAL;   proceso[13][6]  = finalizarOpMayorIgual;
    /* ; */nuevoEstado[13][7]  = ESTADO_FINAL;   proceso[13][7]  = finalizarOpMayorIgual;
    /* : */nuevoEstado[13][8]  = ESTADO_FINAL;   proceso[13][8]  = finalizarOpMayorIgual;
    /* " */nuevoEstado[13][9]  = ESTADO_FINAL;   proceso[13][9]  = finalizarOpMayorIgual;
    /* < */nuevoEstado[13][10] = ESTADO_FINAL;   proceso[13][10] = finalizarOpMayorIgual;
    /* > */nuevoEstado[13][11] = ESTADO_FINAL;   proceso[13][11] = finalizarOpMayorIgual;
    /* = */nuevoEstado[13][12] = ESTADO_FINAL;   proceso[13][12] = finalizarOpMayorIgual;
    /* ! */nuevoEstado[13][13] = ESTADO_FINAL;   proceso[13][13] = finalizarOpMayorIgual;
    /* + */nuevoEstado[13][14] = ESTADO_FINAL;   proceso[13][14] = finalizarOpMayorIgual;
    /* * */nuevoEstado[13][15] = ESTADO_FINAL;   proceso[13][15] = finalizarOpMayorIgual;
    /* / */nuevoEstado[13][16] = ESTADO_FINAL;   proceso[13][16] = finalizarOpMayorIgual;
    /* - */nuevoEstado[13][17] = ESTADO_FINAL;   proceso[13][17] = finalizarOpMayorIgual;
    /* , */nuevoEstado[13][18] = ESTADO_FINAL;   proceso[13][18] = finalizarOpMayorIgual;
    /* _ */nuevoEstado[13][19] = ESTADO_FINAL;   proceso[13][19] = finalizarOpMayorIgual;
    /* . */nuevoEstado[13][20] = ESTADO_FINAL;   proceso[13][20] = finalizarOpMayorIgual;
    /*$#@*/nuevoEstado[13][21] = ESTADO_FINAL;   proceso[13][21] = finalizarOpMayorIgual;
    /*esp*/nuevoEstado[13][22] = ESTADO_FINAL;   proceso[13][22] = finalizarOpMayorIgual;

    /*ESTADO 14*/
    /* 0 */nuevoEstado[14][0]  = ESTADO_FINAL;   proceso[14][0]  = finalizarOpAsig;
    /*1-9*/nuevoEstado[14][1]  = ESTADO_FINAL;   proceso[14][1]  = finalizarOpAsig;
    /*a-Z*/nuevoEstado[14][2]  = ESTADO_FINAL;   proceso[14][2]  = finalizarOpAsig;
    /* ( */nuevoEstado[14][3]  = ESTADO_FINAL;   proceso[14][3]  = finalizarOpAsig;
    /* ) */nuevoEstado[14][4]  = ESTADO_FINAL;   proceso[14][4]  = finalizarOpAsig;
    /* [ */nuevoEstado[14][5]  = ESTADO_FINAL;   proceso[14][5]  = finalizarOpAsig;
    /* ] */nuevoEstado[14][6]  = ESTADO_FINAL;   proceso[14][6]  = finalizarOpAsig;
    /* ; */nuevoEstado[14][7]  = ESTADO_FINAL;   proceso[14][7]  = finalizarOpAsig;
    /* : */nuevoEstado[14][8]  = ESTADO_FINAL;   proceso[14][8]  = finalizarOpAsig;
    /* " */nuevoEstado[14][9]  = ESTADO_FINAL;   proceso[14][9]  = finalizarOpAsig;
    /* < */nuevoEstado[14][10] = ESTADO_FINAL;   proceso[14][10] = finalizarOpAsig;
    /* > */nuevoEstado[14][11] = ESTADO_FINAL;   proceso[14][11] = finalizarOpAsig;
    /* = */nuevoEstado[14][12] = 15;             proceso[14][12] = continuarOpIgual;
    /* ! */nuevoEstado[14][13] = ESTADO_FINAL;   proceso[14][13] = finalizarOpAsig;
    /* + */nuevoEstado[14][14] = ESTADO_FINAL;   proceso[14][14] = finalizarOpAsig;
    /* * */nuevoEstado[14][15] = ESTADO_FINAL;   proceso[14][15] = finalizarOpAsig;
    /* / */nuevoEstado[14][16] = ESTADO_FINAL;   proceso[14][16] = finalizarOpAsig;
    /* - */nuevoEstado[14][17] = ESTADO_FINAL;   proceso[14][17] = finalizarOpAsig;
    /* , */nuevoEstado[14][18] = ESTADO_FINAL;   proceso[14][18] = finalizarOpAsig;
    /* _ */nuevoEstado[14][19] = ESTADO_FINAL;   proceso[14][19] = finalizarOpAsig;
    /* . */nuevoEstado[14][20] = ESTADO_FINAL;   proceso[14][20] = finalizarOpAsig;
    /*$#@*/nuevoEstado[14][21] = ESTADO_FINAL;   proceso[14][21] = finalizarOpAsig;
    /*esp*/nuevoEstado[14][22] = ESTADO_FINAL;   proceso[14][22] = finalizarOpAsig;

    /*ESTADO 15*/
    /* 0 */nuevoEstado[15][0]  = ESTADO_FINAL;   proceso[15][0]  = finalizarOpIgual;
    /*1-9*/nuevoEstado[15][1]  = ESTADO_FINAL;   proceso[15][1]  = finalizarOpIgual;
    /*a-Z*/nuevoEstado[15][2]  = ESTADO_FINAL;   proceso[15][2]  = finalizarOpIgual;
    /* ( */nuevoEstado[15][3]  = ESTADO_FINAL;   proceso[15][3]  = finalizarOpIgual;
    /* ) */nuevoEstado[15][4]  = ESTADO_FINAL;   proceso[15][4]  = finalizarOpIgual;
    /* [ */nuevoEstado[15][5]  = ESTADO_FINAL;   proceso[15][5]  = finalizarOpIgual;
    /* ] */nuevoEstado[15][6]  = ESTADO_FINAL;   proceso[15][6]  = finalizarOpIgual;
    /* ; */nuevoEstado[15][7]  = ESTADO_FINAL;   proceso[15][7]  = finalizarOpIgual;
    /* : */nuevoEstado[15][8]  = ESTADO_FINAL;   proceso[15][8]  = finalizarOpIgual;
    /* " */nuevoEstado[15][9]  = ESTADO_FINAL;   proceso[15][9]  = finalizarOpIgual;
    /* < */nuevoEstado[15][10] = ESTADO_FINAL;   proceso[15][10] = finalizarOpIgual;
    /* > */nuevoEstado[15][11] = ESTADO_FINAL;   proceso[15][11] = finalizarOpIgual;
    /* = */nuevoEstado[15][12] = ESTADO_FINAL;   proceso[15][12] = finalizarOpIgual;
    /* ! */nuevoEstado[15][13] = ESTADO_FINAL;   proceso[15][13] = finalizarOpIgual;
    /* + */nuevoEstado[15][14] = ESTADO_FINAL;   proceso[15][14] = finalizarOpIgual;
    /* * */nuevoEstado[15][15] = ESTADO_FINAL;   proceso[15][15] = finalizarOpIgual;
    /* / */nuevoEstado[15][16] = ESTADO_FINAL;   proceso[15][16] = finalizarOpIgual;
    /* - */nuevoEstado[15][17] = ESTADO_FINAL;   proceso[15][17] = finalizarOpIgual;
    /* , */nuevoEstado[15][18] = ESTADO_FINAL;   proceso[15][18] = finalizarOpIgual;
    /* _ */nuevoEstado[15][19] = ESTADO_FINAL;   proceso[15][19] = finalizarOpIgual;
    /* . */nuevoEstado[15][20] = ESTADO_FINAL;   proceso[15][20] = finalizarOpIgual;
    /*$#@*/nuevoEstado[15][21] = ESTADO_FINAL;   proceso[15][21] = finalizarOpIgual;
    /*esp*/nuevoEstado[15][22] = ESTADO_FINAL;   proceso[15][22] = finalizarOpIgual;

    /*ESTADO 16*/
    /* 0 */nuevoEstado[16][0]  = SYNTAX_ERROR;   proceso[16][0]  = syntaxError;
    /*1-9*/nuevoEstado[16][1]  = SYNTAX_ERROR;   proceso[16][1]  = syntaxError;
    /*a-Z*/nuevoEstado[16][2]  = SYNTAX_ERROR;   proceso[16][2]  = syntaxError;
    /* ( */nuevoEstado[16][3]  = SYNTAX_ERROR;   proceso[16][3]  = syntaxError;
    /* ) */nuevoEstado[16][4]  = SYNTAX_ERROR;   proceso[16][4]  = syntaxError;
    /* [ */nuevoEstado[16][5]  = SYNTAX_ERROR;   proceso[16][5]  = syntaxError;
    /* ] */nuevoEstado[16][6]  = SYNTAX_ERROR;   proceso[16][6]  = syntaxError;
    /* ; */nuevoEstado[16][7]  = SYNTAX_ERROR;   proceso[16][7]  = syntaxError;
    /* : */nuevoEstado[16][8]  = SYNTAX_ERROR;   proceso[16][8]  = syntaxError;
    /* " */nuevoEstado[16][9]  = SYNTAX_ERROR;   proceso[16][9]  = syntaxError;
    /* < */nuevoEstado[16][10] = SYNTAX_ERROR;   proceso[16][10] = syntaxError;
    /* > */nuevoEstado[16][11] = SYNTAX_ERROR;   proceso[16][11] = syntaxError;
    /* = */nuevoEstado[16][12] = 17;             proceso[16][12] = continuarOpDistinto;
    /* ! */nuevoEstado[16][13] = SYNTAX_ERROR;   proceso[16][13] = syntaxError;
    /* + */nuevoEstado[16][14] = SYNTAX_ERROR;   proceso[16][14] = syntaxError;
    /* * */nuevoEstado[16][15] = SYNTAX_ERROR;   proceso[16][15] = syntaxError;
    /* / */nuevoEstado[16][16] = SYNTAX_ERROR;   proceso[16][16] = syntaxError;
    /* - */nuevoEstado[16][17] = SYNTAX_ERROR;   proceso[16][17] = syntaxError;
    /* , */nuevoEstado[16][18] = SYNTAX_ERROR;   proceso[16][18] = syntaxError;
    /* _ */nuevoEstado[16][19] = SYNTAX_ERROR;   proceso[16][19] = syntaxError;
    /* . */nuevoEstado[16][20] = SYNTAX_ERROR;   proceso[16][20] = syntaxError;
    /*$#@*/nuevoEstado[16][21] = SYNTAX_ERROR;   proceso[16][21] = syntaxError;
    /*esp*/nuevoEstado[16][22] = SYNTAX_ERROR;   proceso[16][22] = syntaxError;

    /*ESTADO 17*/
    /* 0 */nuevoEstado[17][0]  = ESTADO_FINAL;   proceso[17][0]  = finalizarOpDistinto;
    /*1-9*/nuevoEstado[17][1]  = ESTADO_FINAL;   proceso[17][1]  = finalizarOpDistinto;
    /*a-Z*/nuevoEstado[17][2]  = ESTADO_FINAL;   proceso[17][2]  = finalizarOpDistinto;
    /* ( */nuevoEstado[17][3]  = ESTADO_FINAL;   proceso[17][3]  = finalizarOpDistinto;
    /* ) */nuevoEstado[17][4]  = ESTADO_FINAL;   proceso[17][4]  = finalizarOpDistinto;
    /* [ */nuevoEstado[17][5]  = ESTADO_FINAL;   proceso[17][5]  = finalizarOpDistinto;
    /* ] */nuevoEstado[17][6]  = ESTADO_FINAL;   proceso[17][6]  = finalizarOpDistinto;
    /* ; */nuevoEstado[17][7]  = ESTADO_FINAL;   proceso[17][7]  = finalizarOpDistinto;
    /* : */nuevoEstado[17][8]  = ESTADO_FINAL;   proceso[17][8]  = finalizarOpDistinto;
    /* " */nuevoEstado[17][9]  = ESTADO_FINAL;   proceso[17][9]  = finalizarOpDistinto;
    /* < */nuevoEstado[17][10] = ESTADO_FINAL;   proceso[17][10] = finalizarOpDistinto;
    /* > */nuevoEstado[17][11] = ESTADO_FINAL;   proceso[17][11] = finalizarOpDistinto;
    /* = */nuevoEstado[17][12] = ESTADO_FINAL;   proceso[17][12] = finalizarOpDistinto;
    /* ! */nuevoEstado[17][13] = ESTADO_FINAL;   proceso[17][13] = finalizarOpDistinto;
    /* + */nuevoEstado[17][14] = ESTADO_FINAL;   proceso[17][14] = finalizarOpDistinto;
    /* * */nuevoEstado[17][15] = ESTADO_FINAL;   proceso[17][15] = finalizarOpDistinto;
    /* / */nuevoEstado[17][16] = ESTADO_FINAL;   proceso[17][16] = finalizarOpDistinto;
    /* - */nuevoEstado[17][17] = ESTADO_FINAL;   proceso[17][17] = finalizarOpDistinto;
    /* , */nuevoEstado[17][18] = ESTADO_FINAL;   proceso[17][18] = finalizarOpDistinto;
    /* _ */nuevoEstado[17][19] = ESTADO_FINAL;   proceso[17][19] = finalizarOpDistinto;
    /* . */nuevoEstado[17][20] = ESTADO_FINAL;   proceso[17][20] = finalizarOpDistinto;
    /*$#@*/nuevoEstado[17][21] = ESTADO_FINAL;   proceso[17][21] = finalizarOpDistinto;
    /*esp*/nuevoEstado[17][22] = ESTADO_FINAL;   proceso[17][22] = finalizarOpDistinto;

    /*ESTADO 18*/
    /* 0 */nuevoEstado[18][0]  = ESTADO_FINAL;   proceso[18][0]  = finalizarOpSuma;
    /*1-9*/nuevoEstado[18][1]  = ESTADO_FINAL;   proceso[18][1]  = finalizarOpSuma;
    /*a-Z*/nuevoEstado[18][2]  = ESTADO_FINAL;   proceso[18][2]  = finalizarOpSuma;
    /* ( */nuevoEstado[18][3]  = ESTADO_FINAL;   proceso[18][3]  = finalizarOpSuma;
    /* ) */nuevoEstado[18][4]  = ESTADO_FINAL;   proceso[18][4]  = finalizarOpSuma;
    /* [ */nuevoEstado[18][5]  = ESTADO_FINAL;   proceso[18][5]  = finalizarOpSuma;
    /* ] */nuevoEstado[18][6]  = ESTADO_FINAL;   proceso[18][6]  = finalizarOpSuma;
    /* ; */nuevoEstado[18][7]  = ESTADO_FINAL;   proceso[18][7]  = finalizarOpSuma;
    /* : */nuevoEstado[18][8]  = ESTADO_FINAL;   proceso[18][8]  = finalizarOpSuma;
    /* " */nuevoEstado[18][9]  = ESTADO_FINAL;   proceso[18][9]  = finalizarOpSuma;
    /* < */nuevoEstado[18][10] = ESTADO_FINAL;   proceso[18][10] = finalizarOpSuma;
    /* > */nuevoEstado[18][11] = ESTADO_FINAL;   proceso[18][11] = finalizarOpSuma;
    /* = */nuevoEstado[18][12] = ESTADO_FINAL;   proceso[18][12] = finalizarOpSuma;
    /* ! */nuevoEstado[18][13] = ESTADO_FINAL;   proceso[18][13] = finalizarOpSuma;
    /* + */nuevoEstado[18][14] = 19;             proceso[18][14] = continuarOpConcat;
    /* * */nuevoEstado[18][15] = ESTADO_FINAL;   proceso[18][15] = finalizarOpSuma;
    /* / */nuevoEstado[18][16] = ESTADO_FINAL;   proceso[18][16] = finalizarOpSuma;
    /* - */nuevoEstado[18][17] = ESTADO_FINAL;   proceso[18][17] = finalizarOpSuma;
    /* , */nuevoEstado[18][18] = ESTADO_FINAL;   proceso[18][18] = finalizarOpSuma;
    /* _ */nuevoEstado[18][19] = ESTADO_FINAL;   proceso[18][19] = finalizarOpSuma;
    /* . */nuevoEstado[18][20] = ESTADO_FINAL;   proceso[18][20] = finalizarOpSuma;
    /*$#@*/nuevoEstado[18][21] = ESTADO_FINAL;   proceso[18][21] = finalizarOpSuma;
    /*esp*/nuevoEstado[18][22] = ESTADO_FINAL;   proceso[18][22] = finalizarOpSuma;

    /*ESTADO 19*/
    /* 0 */nuevoEstado[19][0]  = ESTADO_FINAL;   proceso[19][0]  = finalizarOpConcat;
    /*1-9*/nuevoEstado[19][1]  = ESTADO_FINAL;   proceso[19][1]  = finalizarOpConcat;
    /*a-Z*/nuevoEstado[19][2]  = ESTADO_FINAL;   proceso[19][2]  = finalizarOpConcat;
    /* ( */nuevoEstado[19][3]  = ESTADO_FINAL;   proceso[19][3]  = finalizarOpConcat;
    /* ) */nuevoEstado[19][4]  = ESTADO_FINAL;   proceso[19][4]  = finalizarOpConcat;
    /* [ */nuevoEstado[19][5]  = ESTADO_FINAL;   proceso[19][5]  = finalizarOpConcat;
    /* ] */nuevoEstado[19][6]  = ESTADO_FINAL;   proceso[19][6]  = finalizarOpConcat;
    /* ; */nuevoEstado[19][7]  = ESTADO_FINAL;   proceso[19][7]  = finalizarOpConcat;
    /* : */nuevoEstado[19][8]  = ESTADO_FINAL;   proceso[19][8]  = finalizarOpConcat;
    /* " */nuevoEstado[19][9]  = ESTADO_FINAL;   proceso[19][9]  = finalizarOpConcat;
    /* < */nuevoEstado[19][10] = ESTADO_FINAL;   proceso[19][10] = finalizarOpConcat;
    /* > */nuevoEstado[19][11] = ESTADO_FINAL;   proceso[19][11] = finalizarOpConcat;
    /* = */nuevoEstado[19][12] = ESTADO_FINAL;   proceso[19][12] = finalizarOpConcat;
    /* ! */nuevoEstado[19][13] = ESTADO_FINAL;   proceso[19][13] = finalizarOpConcat;
    /* + */nuevoEstado[19][14] = ESTADO_FINAL;   proceso[19][14] = finalizarOpConcat;
    /* * */nuevoEstado[19][15] = ESTADO_FINAL;   proceso[19][15] = finalizarOpConcat;
    /* / */nuevoEstado[19][16] = ESTADO_FINAL;   proceso[19][16] = finalizarOpConcat;
    /* - */nuevoEstado[19][17] = ESTADO_FINAL;   proceso[19][17] = finalizarOpConcat;
    /* , */nuevoEstado[19][18] = ESTADO_FINAL;   proceso[19][18] = finalizarOpConcat;
    /* _ */nuevoEstado[19][19] = ESTADO_FINAL;   proceso[19][19] = finalizarOpConcat;
    /* . */nuevoEstado[19][20] = ESTADO_FINAL;   proceso[19][20] = finalizarOpConcat;
    /*$#@*/nuevoEstado[19][21] = ESTADO_FINAL;   proceso[19][21] = finalizarOpConcat;
    /*esp*/nuevoEstado[19][22] = ESTADO_FINAL;   proceso[19][22] = finalizarOpConcat;

    /*ESTADO 20*/
    /* 0 */nuevoEstado[20][0]  = ESTADO_FINAL;   proceso[20][0]  = finalizarOpMulti;
    /*1-9*/nuevoEstado[20][1]  = ESTADO_FINAL;   proceso[20][1]  = finalizarOpMulti;
    /*a-Z*/nuevoEstado[20][2]  = ESTADO_FINAL;   proceso[20][2]  = finalizarOpMulti;
    /* ( */nuevoEstado[20][3]  = ESTADO_FINAL;   proceso[20][3]  = finalizarOpMulti;
    /* ) */nuevoEstado[20][4]  = ESTADO_FINAL;   proceso[20][4]  = finalizarOpMulti;
    /* [ */nuevoEstado[20][5]  = ESTADO_FINAL;   proceso[20][5]  = finalizarOpMulti;
    /* ] */nuevoEstado[20][6]  = ESTADO_FINAL;   proceso[20][6]  = finalizarOpMulti;
    /* ; */nuevoEstado[20][7]  = ESTADO_FINAL;   proceso[20][7]  = finalizarOpMulti;
    /* : */nuevoEstado[20][8]  = ESTADO_FINAL;   proceso[20][8]  = finalizarOpMulti;
    /* " */nuevoEstado[20][9]  = ESTADO_FINAL;   proceso[20][9]  = finalizarOpMulti;
    /* < */nuevoEstado[20][10] = ESTADO_FINAL;   proceso[20][10] = finalizarOpMulti;
    /* > */nuevoEstado[20][11] = ESTADO_FINAL;   proceso[20][11] = finalizarOpMulti;
    /* = */nuevoEstado[20][12] = ESTADO_FINAL;   proceso[20][12] = finalizarOpMulti;
    /* ! */nuevoEstado[20][13] = ESTADO_FINAL;   proceso[20][13] = finalizarOpMulti;
    /* + */nuevoEstado[20][14] = ESTADO_FINAL;   proceso[20][14] = finalizarOpMulti;
    /* * */nuevoEstado[20][15] = ESTADO_FINAL;   proceso[20][15] = finalizarOpMulti;
    /* / */nuevoEstado[20][16] = ESTADO_FINAL;   proceso[20][16] = finalizarOpMulti;
    /* - */nuevoEstado[20][17] = ESTADO_FINAL;   proceso[20][17] = finalizarOpMulti;
    /* , */nuevoEstado[20][18] = ESTADO_FINAL;   proceso[20][18] = finalizarOpMulti;
    /* _ */nuevoEstado[20][19] = ESTADO_FINAL;   proceso[20][19] = finalizarOpMulti;
    /* . */nuevoEstado[20][20] = ESTADO_FINAL;   proceso[20][20] = finalizarOpMulti;
    /*$#@*/nuevoEstado[20][21] = ESTADO_FINAL;   proceso[20][21] = finalizarOpMulti;
    /*esp*/nuevoEstado[20][22] = ESTADO_FINAL;   proceso[20][22] = finalizarOpMulti;

    /*ESTADO 21*/
    /* 0 */nuevoEstado[21][0]  = 21;             proceso[21][0]  = continuarCteEnteraOReal;
    /*1-9*/nuevoEstado[21][1]  = 21;             proceso[21][1]  = continuarCteEnteraOReal;
    /*a-Z*/nuevoEstado[21][2]  = ESTADO_FINAL;   proceso[21][2]  = finalizarCteEntera;
    /* ( */nuevoEstado[21][3]  = ESTADO_FINAL;   proceso[21][3]  = finalizarCteEntera;
    /* ) */nuevoEstado[21][4]  = ESTADO_FINAL;   proceso[21][4]  = finalizarCteEntera;
    /* [ */nuevoEstado[21][5]  = ESTADO_FINAL;   proceso[21][5]  = finalizarCteEntera;
    /* ] */nuevoEstado[21][6]  = ESTADO_FINAL;   proceso[21][6]  = finalizarCteEntera;
    /* ; */nuevoEstado[21][7]  = ESTADO_FINAL;   proceso[21][7]  = finalizarCteEntera;
    /* : */nuevoEstado[21][8]  = ESTADO_FINAL;   proceso[21][8]  = finalizarCteEntera;
    /* " */nuevoEstado[21][9]  = ESTADO_FINAL;   proceso[21][9]  = finalizarCteEntera;
    /* < */nuevoEstado[21][10] = ESTADO_FINAL;   proceso[21][10] = finalizarCteEntera;
    /* > */nuevoEstado[21][11] = ESTADO_FINAL;   proceso[21][11] = finalizarCteEntera;
    /* = */nuevoEstado[21][12] = ESTADO_FINAL;   proceso[21][12] = finalizarCteEntera;
    /* ! */nuevoEstado[21][13] = ESTADO_FINAL;   proceso[21][13] = finalizarCteEntera;
    /* + */nuevoEstado[21][14] = ESTADO_FINAL;   proceso[21][14] = finalizarCteEntera;
    /* * */nuevoEstado[21][15] = ESTADO_FINAL;   proceso[21][15] = finalizarCteEntera;
    /* / */nuevoEstado[21][16] = ESTADO_FINAL;   proceso[21][16] = finalizarCteEntera;
    /* - */nuevoEstado[21][17] = ESTADO_FINAL;   proceso[21][17] = finalizarCteEntera;
    /* , */nuevoEstado[21][18] = ESTADO_FINAL;   proceso[21][18] = finalizarCteEntera;
    /* _ */nuevoEstado[21][19] = ESTADO_FINAL;   proceso[21][19] = finalizarCteEntera;
    /* . */nuevoEstado[21][20] = 23;             proceso[21][20] = continuarCteReal;
    /*$#@*/nuevoEstado[21][21] = ESTADO_FINAL;   proceso[21][21] = finalizarCteEntera;
    /*esp*/nuevoEstado[21][22] = ESTADO_FINAL;   proceso[21][22] = finalizarCteEntera;

    /*ESTADO 22*/
    /* 0 */nuevoEstado[22][0]  = 23;             proceso[22][0]  = continuarCteReal;
    /*1-9*/nuevoEstado[22][1]  = 23;             proceso[22][1]  = continuarCteReal;
    /*a-Z*/nuevoEstado[22][2]  = SYNTAX_ERROR;   proceso[22][2]  = syntaxError;
    /* ( */nuevoEstado[22][3]  = SYNTAX_ERROR;   proceso[22][3]  = syntaxError;
    /* ) */nuevoEstado[22][4]  = SYNTAX_ERROR;   proceso[22][4]  = syntaxError;
    /* [ */nuevoEstado[22][5]  = SYNTAX_ERROR;   proceso[22][5]  = syntaxError;
    /* ] */nuevoEstado[22][6]  = SYNTAX_ERROR;   proceso[22][6]  = syntaxError;
    /* ; */nuevoEstado[22][7]  = SYNTAX_ERROR;   proceso[22][7]  = syntaxError;
    /* : */nuevoEstado[22][8]  = SYNTAX_ERROR;   proceso[22][8]  = syntaxError;
    /* " */nuevoEstado[22][9]  = SYNTAX_ERROR;   proceso[22][9]  = syntaxError;
    /* < */nuevoEstado[22][10] = SYNTAX_ERROR;   proceso[22][10] = syntaxError;
    /* > */nuevoEstado[22][11] = SYNTAX_ERROR;   proceso[22][11] = syntaxError;
    /* = */nuevoEstado[22][12] = SYNTAX_ERROR;   proceso[22][12] = syntaxError;
    /* ! */nuevoEstado[22][13] = SYNTAX_ERROR;   proceso[22][13] = syntaxError;
    /* + */nuevoEstado[22][14] = SYNTAX_ERROR;   proceso[22][14] = syntaxError;
    /* * */nuevoEstado[22][15] = SYNTAX_ERROR;   proceso[22][15] = syntaxError;
    /* / */nuevoEstado[22][16] = SYNTAX_ERROR;   proceso[22][16] = syntaxError;
    /* - */nuevoEstado[22][17] = SYNTAX_ERROR;   proceso[22][17] = syntaxError;
    /* , */nuevoEstado[22][18] = SYNTAX_ERROR;   proceso[22][18] = syntaxError;
    /* _ */nuevoEstado[22][19] = SYNTAX_ERROR;   proceso[22][19] = syntaxError;
    /* . */nuevoEstado[22][20] = SYNTAX_ERROR;   proceso[22][20] = syntaxError;
    /*$#@*/nuevoEstado[22][21] = SYNTAX_ERROR;   proceso[22][21] = syntaxError;
    /*esp*/nuevoEstado[22][22] = SYNTAX_ERROR;   proceso[22][22] = syntaxError;

    /*ESTADO 23*/
    /* 0 */nuevoEstado[23][0]  = 23;             proceso[23][0]  = continuarCteReal;
    /*1-9*/nuevoEstado[23][1]  = 23;             proceso[23][1]  = continuarCteReal;
    /*a-Z*/nuevoEstado[23][2]  = ESTADO_FINAL;   proceso[23][2]  = finalizarCteReal;
    /* ( */nuevoEstado[23][3]  = ESTADO_FINAL;   proceso[23][3]  = finalizarCteReal;
    /* ) */nuevoEstado[23][4]  = ESTADO_FINAL;   proceso[23][4]  = finalizarCteReal;
    /* [ */nuevoEstado[23][5]  = ESTADO_FINAL;   proceso[23][5]  = finalizarCteReal;
    /* ] */nuevoEstado[23][6]  = ESTADO_FINAL;   proceso[23][6]  = finalizarCteReal;
    /* ; */nuevoEstado[23][7]  = ESTADO_FINAL;   proceso[23][7]  = finalizarCteReal;
    /* : */nuevoEstado[23][8]  = ESTADO_FINAL;   proceso[23][8]  = finalizarCteReal;
    /* " */nuevoEstado[23][9]  = ESTADO_FINAL;   proceso[23][9]  = finalizarCteReal;
    /* < */nuevoEstado[23][10] = ESTADO_FINAL;   proceso[23][10] = finalizarCteReal;
    /* > */nuevoEstado[23][11] = ESTADO_FINAL;   proceso[23][11] = finalizarCteReal;
    /* = */nuevoEstado[23][12] = ESTADO_FINAL;   proceso[23][12] = finalizarCteReal;
    /* ! */nuevoEstado[23][13] = ESTADO_FINAL;   proceso[23][13] = finalizarCteReal;
    /* + */nuevoEstado[23][14] = ESTADO_FINAL;   proceso[23][14] = finalizarCteReal;
    /* * */nuevoEstado[23][15] = ESTADO_FINAL;   proceso[23][15] = finalizarCteReal;
    /* / */nuevoEstado[23][16] = ESTADO_FINAL;   proceso[23][16] = finalizarCteReal;
    /* - */nuevoEstado[23][17] = ESTADO_FINAL;   proceso[23][17] = finalizarCteReal;
    /* , */nuevoEstado[23][18] = ESTADO_FINAL;   proceso[23][18] = finalizarCteReal;
    /* _ */nuevoEstado[23][19] = ESTADO_FINAL;   proceso[23][19] = finalizarCteReal;
    /* . */nuevoEstado[23][20] = ESTADO_FINAL;   proceso[23][20] = finalizarCteReal;
    /*$#@*/nuevoEstado[23][21] = ESTADO_FINAL;   proceso[23][21] = finalizarCteReal;
    /*esp*/nuevoEstado[23][22] = ESTADO_FINAL;   proceso[23][22] = finalizarCteReal;

    /*ESTADO 24*/
    /* 0 */nuevoEstado[24][0]  = ESTADO_FINAL;   proceso[24][0]  = finalizarOpDivi;
    /*1-9*/nuevoEstado[24][1]  = ESTADO_FINAL;   proceso[24][1]  = finalizarOpDivi;
    /*a-Z*/nuevoEstado[24][2]  = ESTADO_FINAL;   proceso[24][2]  = finalizarOpDivi;
    /* ( */nuevoEstado[24][3]  = ESTADO_FINAL;   proceso[24][3]  = finalizarOpDivi;
    /* ) */nuevoEstado[24][4]  = ESTADO_FINAL;   proceso[24][4]  = finalizarOpDivi;
    /* [ */nuevoEstado[24][5]  = ESTADO_FINAL;   proceso[24][5]  = finalizarOpDivi;
    /* ] */nuevoEstado[24][6]  = ESTADO_FINAL;   proceso[24][6]  = finalizarOpDivi;
    /* ; */nuevoEstado[24][7]  = ESTADO_FINAL;   proceso[24][7]  = finalizarOpDivi;
    /* : */nuevoEstado[24][8]  = ESTADO_FINAL;   proceso[24][8]  = finalizarOpDivi;
    /* " */nuevoEstado[24][9]  = ESTADO_FINAL;   proceso[24][9]  = finalizarOpDivi;
    /* < */nuevoEstado[24][10] = ESTADO_FINAL;   proceso[24][10] = finalizarOpDivi;
    /* > */nuevoEstado[24][11] = ESTADO_FINAL;   proceso[24][11] = finalizarOpDivi;
    /* = */nuevoEstado[24][12] = ESTADO_FINAL;   proceso[24][12] = finalizarOpDivi;
    /* ! */nuevoEstado[24][13] = ESTADO_FINAL;   proceso[24][13] = finalizarOpDivi;
    /* + */nuevoEstado[24][14] = ESTADO_FINAL;   proceso[24][14] = finalizarOpDivi;
    /* * */nuevoEstado[24][15] = ESTADO_FINAL;   proceso[24][15] = finalizarOpDivi;
    /* / */nuevoEstado[24][16] = ESTADO_FINAL;   proceso[24][16] = finalizarOpDivi;
    /* - */nuevoEstado[24][17] = ESTADO_FINAL;   proceso[24][17] = finalizarOpDivi;
    /* , */nuevoEstado[24][18] = ESTADO_FINAL;   proceso[24][18] = finalizarOpDivi;
    /* _ */nuevoEstado[24][19] = ESTADO_FINAL;   proceso[24][19] = finalizarOpDivi;
    /* . */nuevoEstado[24][20] = ESTADO_FINAL;   proceso[24][20] = finalizarOpDivi;
    /*$#@*/nuevoEstado[24][21] = ESTADO_FINAL;   proceso[24][21] = finalizarOpDivi;
    /*esp*/nuevoEstado[24][22] = ESTADO_FINAL;   proceso[24][22] = finalizarOpDivi;

    /*ESTADO 25*/
    /* 0 */nuevoEstado[25][0]  = ESTADO_FINAL;   proceso[25][0]  = finalizarOpResta;
    /*1-9*/nuevoEstado[25][1]  = ESTADO_FINAL;   proceso[25][1]  = finalizarOpResta;
    /*a-Z*/nuevoEstado[25][2]  = ESTADO_FINAL;   proceso[25][2]  = finalizarOpResta;
    /* ( */nuevoEstado[25][3]  = ESTADO_FINAL;   proceso[25][3]  = finalizarOpResta;
    /* ) */nuevoEstado[25][4]  = ESTADO_FINAL;   proceso[25][4]  = finalizarOpResta;
    /* [ */nuevoEstado[25][5]  = ESTADO_FINAL;   proceso[25][5]  = finalizarOpResta;
    /* ] */nuevoEstado[25][6]  = ESTADO_FINAL;   proceso[25][6]  = finalizarOpResta;
    /* ; */nuevoEstado[25][7]  = ESTADO_FINAL;   proceso[25][7]  = finalizarOpResta;
    /* : */nuevoEstado[25][8]  = ESTADO_FINAL;   proceso[25][8]  = finalizarOpResta;
    /* " */nuevoEstado[25][9]  = ESTADO_FINAL;   proceso[25][9]  = finalizarOpResta;
    /* < */nuevoEstado[25][10] = ESTADO_FINAL;   proceso[25][10] = finalizarOpResta;
    /* > */nuevoEstado[25][11] = ESTADO_FINAL;   proceso[25][11] = finalizarOpResta;
    /* = */nuevoEstado[25][12] = ESTADO_FINAL;   proceso[25][12] = finalizarOpResta;
    /* ! */nuevoEstado[25][13] = ESTADO_FINAL;   proceso[25][13] = finalizarOpResta;
    /* + */nuevoEstado[25][14] = ESTADO_FINAL;   proceso[25][14] = finalizarOpResta;
    /* * */nuevoEstado[25][15] = ESTADO_FINAL;   proceso[25][15] = finalizarOpResta;
    /* / */nuevoEstado[25][16] = 26;             proceso[25][16] = continuarComentario;
    /* - */nuevoEstado[25][17] = ESTADO_FINAL;   proceso[25][17] = finalizarOpResta;
    /* , */nuevoEstado[25][18] = ESTADO_FINAL;   proceso[25][18] = finalizarOpResta;
    /* _ */nuevoEstado[25][19] = ESTADO_FINAL;   proceso[25][19] = finalizarOpResta;
    /* . */nuevoEstado[25][20] = ESTADO_FINAL;   proceso[25][20] = finalizarOpResta;
    /*$#@*/nuevoEstado[25][21] = ESTADO_FINAL;   proceso[25][21] = finalizarOpResta;
    /*esp*/nuevoEstado[25][22] = ESTADO_FINAL;   proceso[25][22] = finalizarOpResta;

    /*ESTADO 26*/
    /* 0 */nuevoEstado[26][0]  = 26;             proceso[26][0]  = continuarComentario;
    /*1-9*/nuevoEstado[26][1]  = 26;             proceso[26][1]  = continuarComentario;
    /*a-Z*/nuevoEstado[26][2]  = 26;             proceso[26][2]  = continuarComentario;
    /* ( */nuevoEstado[26][3]  = 26;             proceso[26][3]  = continuarComentario;
    /* ) */nuevoEstado[26][4]  = 26;             proceso[26][4]  = continuarComentario;
    /* [ */nuevoEstado[26][5]  = 26;             proceso[26][5]  = continuarComentario;
    /* ] */nuevoEstado[26][6]  = 26;             proceso[26][6]  = continuarComentario;
    /* ; */nuevoEstado[26][7]  = 26;             proceso[26][7]  = continuarComentario;
    /* : */nuevoEstado[26][8]  = 26;             proceso[26][8]  = continuarComentario;
    /* " */nuevoEstado[26][9]  = 26;             proceso[26][9]  = continuarComentario;
    /* < */nuevoEstado[26][10] = 26;             proceso[26][10] = continuarComentario;
    /* > */nuevoEstado[26][11] = 26;             proceso[26][11] = continuarComentario;
    /* = */nuevoEstado[26][12] = 26;             proceso[26][12] = continuarComentario;
    /* ! */nuevoEstado[26][13] = 26;             proceso[26][13] = continuarComentario;
    /* + */nuevoEstado[26][14] = 26;             proceso[26][14] = continuarComentario;
    /* * */nuevoEstado[26][15] = 26;             proceso[26][15] = continuarComentario;
    /* / */nuevoEstado[26][16] = 27;             proceso[26][16] = continuarComentario;
    /* - */nuevoEstado[26][17] = 26;             proceso[26][17] = continuarComentario;
    /* , */nuevoEstado[26][18] = 26;             proceso[26][18] = continuarComentario;
    /* _ */nuevoEstado[26][19] = 26;             proceso[26][19] = continuarComentario;
    /* . */nuevoEstado[26][20] = 26;             proceso[26][20] = continuarComentario;
    /*$#@*/nuevoEstado[26][21] = 26;             proceso[26][21] = continuarComentario;
    /*esp*/nuevoEstado[26][22] = 26;             proceso[26][22] = continuarComentario;

    /*ESTADO 27*/
    /* 0 */nuevoEstado[27][0]  = 26;             proceso[27][0]  = continuarComentario;
    /*1-9*/nuevoEstado[27][1]  = 26;             proceso[27][1]  = continuarComentario;
    /*a-Z*/nuevoEstado[27][2]  = 26;             proceso[27][2]  = continuarComentario;
    /* ( */nuevoEstado[27][3]  = 26;             proceso[27][3]  = continuarComentario;
    /* ) */nuevoEstado[27][4]  = 26;             proceso[27][4]  = continuarComentario;
    /* [ */nuevoEstado[27][5]  = 26;             proceso[27][5]  = continuarComentario;
    /* ] */nuevoEstado[27][6]  = 26;             proceso[27][6]  = continuarComentario;
    /* ; */nuevoEstado[27][7]  = 26;             proceso[27][7]  = continuarComentario;
    /* : */nuevoEstado[27][8]  = 26;             proceso[27][8]  = continuarComentario;
    /* " */nuevoEstado[27][9]  = 26;             proceso[27][9]  = continuarComentario;
    /* < */nuevoEstado[27][10] = 26;             proceso[27][10] = continuarComentario;
    /* > */nuevoEstado[27][11] = 26;             proceso[27][11] = continuarComentario;
    /* = */nuevoEstado[27][12] = 26;             proceso[27][12] = continuarComentario;
    /* ! */nuevoEstado[27][13] = 26;             proceso[27][13] = continuarComentario;
    /* + */nuevoEstado[27][14] = 26;             proceso[27][14] = continuarComentario;
    /* * */nuevoEstado[27][15] = 26;             proceso[27][15] = continuarComentario;
    /* / */nuevoEstado[27][16] = 27;             proceso[27][16] = continuarComentario;
    /* - */nuevoEstado[27][17] = 0;              proceso[27][17] = finalizarComentario;
    /* , */nuevoEstado[27][18] = 26;             proceso[27][18] = continuarComentario;
    /* _ */nuevoEstado[27][19] = 26;             proceso[27][19] = continuarComentario;
    /* . */nuevoEstado[27][20] = 26;             proceso[27][20] = continuarComentario;
    /*$#@*/nuevoEstado[27][21] = 26;             proceso[27][21] = continuarComentario;
    /*esp*/nuevoEstado[27][22] = 26;             proceso[27][22] = continuarComentario;

    /*ESTADO 28*/
    /* 0 */nuevoEstado[28][0]  = ESTADO_FINAL;   proceso[28][0]  = finalizarComa;
    /*1-9*/nuevoEstado[28][1]  = ESTADO_FINAL;   proceso[28][1]  = finalizarComa;
    /*a-Z*/nuevoEstado[28][2]  = ESTADO_FINAL;   proceso[28][2]  = finalizarComa;
    /* ( */nuevoEstado[28][3]  = ESTADO_FINAL;   proceso[28][3]  = finalizarComa;
    /* ) */nuevoEstado[28][4]  = ESTADO_FINAL;   proceso[28][4]  = finalizarComa;
    /* [ */nuevoEstado[28][5]  = ESTADO_FINAL;   proceso[28][5]  = finalizarComa;
    /* ] */nuevoEstado[28][6]  = ESTADO_FINAL;   proceso[28][6]  = finalizarComa;
    /* ; */nuevoEstado[28][7]  = ESTADO_FINAL;   proceso[28][7]  = finalizarComa;
    /* : */nuevoEstado[28][8]  = ESTADO_FINAL;   proceso[28][8]  = finalizarComa;
    /* " */nuevoEstado[28][9]  = ESTADO_FINAL;   proceso[28][9]  = finalizarComa;
    /* < */nuevoEstado[28][10] = ESTADO_FINAL;   proceso[28][10] = finalizarComa;
    /* > */nuevoEstado[28][11] = ESTADO_FINAL;   proceso[28][11] = finalizarComa;
    /* = */nuevoEstado[28][12] = ESTADO_FINAL;   proceso[28][12] = finalizarComa;
    /* ! */nuevoEstado[28][13] = ESTADO_FINAL;   proceso[28][13] = finalizarComa;
    /* + */nuevoEstado[28][14] = ESTADO_FINAL;   proceso[28][14] = finalizarComa;
    /* * */nuevoEstado[28][15] = ESTADO_FINAL;   proceso[28][15] = finalizarComa;
    /* / */nuevoEstado[28][16] = ESTADO_FINAL;   proceso[28][16] = finalizarComa;
    /* - */nuevoEstado[28][17] = ESTADO_FINAL;   proceso[28][17] = finalizarComa;
    /* , */nuevoEstado[28][18] = ESTADO_FINAL;   proceso[28][18] = finalizarComa;
    /* _ */nuevoEstado[28][19] = ESTADO_FINAL;   proceso[28][19] = finalizarComa;
    /* . */nuevoEstado[28][20] = ESTADO_FINAL;   proceso[28][20] = finalizarComa;
    /*$#@*/nuevoEstado[28][21] = ESTADO_FINAL;   proceso[28][21] = finalizarComa;
    /*esp*/nuevoEstado[28][22] = ESTADO_FINAL;   proceso[28][22] = finalizarComa;

    /*ESTADO 29*/
    /* 0 */nuevoEstado[29][0]  = ESTADO_FINAL;   proceso[29][0]  = finalizarGuionBajo;
    /*1-9*/nuevoEstado[29][1]  = ESTADO_FINAL;   proceso[29][1]  = finalizarGuionBajo;
    /*a-Z*/nuevoEstado[29][2]  = ESTADO_FINAL;   proceso[29][2]  = finalizarGuionBajo;
    /* ( */nuevoEstado[29][3]  = ESTADO_FINAL;   proceso[29][3]  = finalizarGuionBajo;
    /* ) */nuevoEstado[29][4]  = ESTADO_FINAL;   proceso[29][4]  = finalizarGuionBajo;
    /* [ */nuevoEstado[29][5]  = ESTADO_FINAL;   proceso[29][5]  = finalizarGuionBajo;
    /* ] */nuevoEstado[29][6]  = ESTADO_FINAL;   proceso[29][6]  = finalizarGuionBajo;
    /* ; */nuevoEstado[29][7]  = ESTADO_FINAL;   proceso[29][7]  = finalizarGuionBajo;
    /* : */nuevoEstado[29][8]  = ESTADO_FINAL;   proceso[29][8]  = finalizarGuionBajo;
    /* " */nuevoEstado[29][9]  = ESTADO_FINAL;   proceso[29][9]  = finalizarGuionBajo;
    /* < */nuevoEstado[29][10] = ESTADO_FINAL;   proceso[29][10] = finalizarGuionBajo;
    /* > */nuevoEstado[29][11] = ESTADO_FINAL;   proceso[29][11] = finalizarGuionBajo;
    /* = */nuevoEstado[29][12] = ESTADO_FINAL;   proceso[29][12] = finalizarGuionBajo;
    /* ! */nuevoEstado[29][13] = ESTADO_FINAL;   proceso[29][13] = finalizarGuionBajo;
    /* + */nuevoEstado[29][14] = ESTADO_FINAL;   proceso[29][14] = finalizarGuionBajo;
    /* * */nuevoEstado[29][15] = ESTADO_FINAL;   proceso[29][15] = finalizarGuionBajo;
    /* / */nuevoEstado[29][16] = ESTADO_FINAL;   proceso[29][16] = finalizarGuionBajo;
    /* - */nuevoEstado[29][17] = ESTADO_FINAL;   proceso[29][17] = finalizarGuionBajo;
    /* , */nuevoEstado[29][18] = ESTADO_FINAL;   proceso[29][18] = finalizarGuionBajo;
    /* _ */nuevoEstado[29][19] = ESTADO_FINAL;   proceso[29][19] = finalizarGuionBajo;
    /* . */nuevoEstado[29][20] = ESTADO_FINAL;   proceso[29][20] = finalizarGuionBajo;
    /*$#@*/nuevoEstado[29][21] = ESTADO_FINAL;   proceso[29][21] = finalizarGuionBajo;
    /*esp*/nuevoEstado[29][22] = ESTADO_FINAL;   proceso[29][22] = finalizarGuionBajo;

    /*ESTADO 30*/
    /* 0 */nuevoEstado[30][0]  = SYNTAX_ERROR;   proceso[30][0]  = syntaxError;
    /*1-9*/nuevoEstado[30][1]  = SYNTAX_ERROR;   proceso[30][1]  = syntaxError;
    /*a-Z*/nuevoEstado[30][2]  = ESTADO_FINAL;   proceso[30][2]  = finalizarCteEntera;
    /* ( */nuevoEstado[30][3]  = ESTADO_FINAL;   proceso[30][3]  = finalizarCteEntera;
    /* ) */nuevoEstado[30][4]  = ESTADO_FINAL;   proceso[30][4]  = finalizarCteEntera;
    /* [ */nuevoEstado[30][5]  = ESTADO_FINAL;   proceso[30][5]  = finalizarCteEntera;
    /* ] */nuevoEstado[30][6]  = ESTADO_FINAL;   proceso[30][6]  = finalizarCteEntera;
    /* ; */nuevoEstado[30][7]  = ESTADO_FINAL;   proceso[30][7]  = finalizarCteEntera;
    /* : */nuevoEstado[30][8]  = ESTADO_FINAL;   proceso[30][8]  = finalizarCteEntera;
    /* " */nuevoEstado[30][9]  = ESTADO_FINAL;   proceso[30][9]  = finalizarCteEntera;
    /* < */nuevoEstado[30][10] = ESTADO_FINAL;   proceso[30][10] = finalizarCteEntera;
    /* > */nuevoEstado[30][11] = ESTADO_FINAL;   proceso[30][11] = finalizarCteEntera;
    /* = */nuevoEstado[30][12] = ESTADO_FINAL;   proceso[30][12] = finalizarCteEntera;
    /* ! */nuevoEstado[30][13] = ESTADO_FINAL;   proceso[30][13] = finalizarCteEntera;
    /* + */nuevoEstado[30][14] = ESTADO_FINAL;   proceso[30][14] = finalizarCteEntera;
    /* * */nuevoEstado[30][15] = ESTADO_FINAL;   proceso[30][15] = finalizarCteEntera;
    /* / */nuevoEstado[30][16] = ESTADO_FINAL;   proceso[30][16] = finalizarCteEntera;
    /* - */nuevoEstado[30][17] = ESTADO_FINAL;   proceso[30][17] = finalizarCteEntera;
    /* , */nuevoEstado[30][18] = ESTADO_FINAL;   proceso[30][18] = finalizarCteEntera;
    /* _ */nuevoEstado[30][19] = ESTADO_FINAL;   proceso[30][19] = finalizarCteEntera;
    /* . */nuevoEstado[30][20] = 22;			 proceso[30][20] = continuarCteReal;
    /*$#@*/nuevoEstado[30][21] = ESTADO_FINAL;   proceso[30][21] = finalizarCteEntera;
    /*esp*/nuevoEstado[30][22] = ESTADO_FINAL;   proceso[30][22] = finalizarCteEntera;
}
