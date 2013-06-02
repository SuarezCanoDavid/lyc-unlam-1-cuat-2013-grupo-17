#include <stdio.h>

#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H

/*Constantes TRUE y FALSE*/
#define TRUE	1
#define FALSE	0

/*Constante de longitud máxima de un token*/
#define MAX_LONG_TOKEN	500

#define LONG_TIPO_TOKEN 20

/*Constante de ESTADO_FINAL*/
#define ESTADO_FINAL	31

/*Constante de cantidad de columnas*/
#define CANT_COLUMNAS	23

/*Constante syntax errror*/
#define SYNTAX_ERROR	-1

/*Longitud de la tabla de símbolos*/
#define LONG_TS			150

/*Cantidad de palabras reservadas*/
#define CANT_PR			36

//Cantidad Maxima para Cadenas
#define MAX_LONG_CTE_STRING		30
//Cantidad Maxima para Enteros
#define MAX_VALOR_CTE_ENTERA	65535

/*Errores*/
#define	ERROR_CTE_STRING_ABIERTA		0
#define ERROR_COMENTARIO_ABIERTO		1
#define	ERROR_CTE_STRING_SUPERA_30		2
#define ERROR_CTE_ENTERA_FUERA_DE_RANGO	3
#define	ERROR_CTE_REAL_FUERA_DE_RANGO	4
#define ERROR_FORMATO_NUMERICO_INVALIDO	5
#define ERROR_OP_DISTINTO_NO_FINALIZADO 6
#define ERROR_CARACTER_NO_VALIDO		7

/*Cantidad de errores*/
#define CANT_ERRORES	8

/*Longitud de la descripcion de un error*/
#define LONG_DESC_ERROR		50

/*Estructura que describe al token que está siendo analizado*/
struct tokenAAnalizar
{
	char token[MAX_LONG_TOKEN];
	int longitudToken;
};
typedef struct tokenAAnalizar tokenAAnalizar;

/*Estructura que describe a la Tabla de Símbolos o TS*/
struct tablaDeSimbolos
{
	char nombre[MAX_LONG_TOKEN+1];
	int tipo;
	char valor[MAX_LONG_TOKEN];
	int longitud;
	int descartada;
};
typedef struct tablaDeSimbolos tablaDeSimbolos;

/*Estructura para manejar errores*/
struct manejadorDeErrores
{
	int estado;
	char descripcion[LONG_DESC_ERROR];
};
typedef struct manejadorDeErrores manejadorDeErrores;

/*yylval*/
int yylval;

/*Funciones*/
void inicializarAL(FILE *fuente);
void inicializarMatrices();
int yylex();
int determinarColumna(char caracter);
void insertarCaracterEnToken(tokenAAnalizar *tokenActual, char caracter);
void insertarTokenEnTS(tokenAAnalizar *tokenActual, const int tipoDeToken);
void imprimirTS();
char *identificarTipoToken(int tipo);

/*ID*/
void iniciarId(tokenAAnalizar *tokenActual, char caracter);
void continuarId(tokenAAnalizar *tokenActual, char caracter);
void finalizarId(tokenAAnalizar *tokenActual, char caracter);

/*CTE_ENTERA o CTE_REAL*/
void iniciarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter);
void continuarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter);
void finalizarCteEntera(tokenAAnalizar *tokenActual, char caracter);
void iniciarCteReal(tokenAAnalizar *tokenActual, char caracter);
void continuarCteReal(tokenAAnalizar *tokenActual, char caracter);
void finalizarCteReal(tokenAAnalizar *tokenActual, char caracter);

/*CTE_STRING*/
void iniciarCteStr(tokenAAnalizar *tokenActual, char caracter);
void continuarCteStr(tokenAAnalizar *tokenActual, char caracter);
void finalizarCteStr(tokenAAnalizar *tokenActual, char caracter);

/*OP_SUMA o CONCATENACION*/
void iniciarOpSumaOConcat(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpSuma(tokenAAnalizar *tokenActual, char caracter);
void continuarOpConcat(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpConcat(tokenAAnalizar *tokenActual, char caracter);

/*OP_RESTA o COMENTARIO*/ 
void iniciarOpRestaOComent(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpResta(tokenAAnalizar *tokenActual, char caracter);
void continuarComentario(tokenAAnalizar *tokenActual, char caracter);
void finalizarComentario(tokenAAnalizar *tokenActual, char caracter);

/*OP_MULTIPLICACION*/ 
void iniciarOpMulti(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMulti(tokenAAnalizar *tokenActual, char caracter);

/*OP_DIVISION*/ 
void iniciarOpDivi(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpDivi(tokenAAnalizar *tokenActual, char caracter);

/*PARENTESIS_ABRE*/ 
void iniciarParAbre(tokenAAnalizar *tokenActual, char caracter);
void finalizarParAbre(tokenAAnalizar *tokenActual, char caracter);

/*PARENTESIS_CIERRA*/ 
void iniciarParCierra(tokenAAnalizar *tokenActual, char caracter);
void finalizarParCierra(tokenAAnalizar *tokenActual, char caracter);

/*CORCHETES_ABRE*/ 
void iniciarCorAbre(tokenAAnalizar *tokenActual, char caracter);
void finalizarCorAbre(tokenAAnalizar *tokenActual, char caracter);

/*CORCHETES_CIERRA*/ 
void iniciarCorCierra(tokenAAnalizar *tokenActual, char caracter);
void finalizarCorCierra(tokenAAnalizar *tokenActual, char caracter);

/*PUNTO_COMA*/ 
void iniciarPuntoComa(tokenAAnalizar *tokenActual, char caracter);
void finalizarPuntoComa(tokenAAnalizar *tokenActual, char caracter);

/*DOS_PUNTOS*/ 
void iniciarDosPuntos(tokenAAnalizar *tokenActual, char caracter);
void finalizarDosPuntos(tokenAAnalizar *tokenActual, char caracter);

/*GUION_BAJO*/ 
void iniciarGuionBajo(tokenAAnalizar *tokenActual, char caracter);
void finalizarGuionBajo(tokenAAnalizar *tokenActual, char caracter);

/*COMA*/ 
void iniciarComa(tokenAAnalizar *tokenActual, char caracter);
void finalizarComa(tokenAAnalizar *tokenActual, char caracter);

/*OP_MENOR u OP_MENOR_IGUAL*/ 
void iniciarOpMenorOMenorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMenor(tokenAAnalizar *tokenActual, char caracter);
void continuarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter);

/*OP_MAYOR u OP_MAYOR_IGUAL*/ 
void iniciarOpMayorOMayorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMayor(tokenAAnalizar *tokenActual, char caracter);
void continuarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter);

/*OP_ASIGNACION u OP_IGUAL*/ 
void iniciarOpAsigOIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpAsig(tokenAAnalizar *tokenActual, char caracter);
void continuarOpIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpIgual(tokenAAnalizar *tokenActual, char caracter);

/*OP_DISTINTO*/ 
void iniciarOpDistinto(tokenAAnalizar *tokenActual, char caracter);
void continuarOpDistinto(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpDistinto(tokenAAnalizar *tokenActual, char caracter);

/*Espacios vacíos (ENTER, TAB, ESPACIO)*/ 
void ignorarEspacios(tokenAAnalizar * (tokenActual), char caracter);

/*Error de sintáxis*/ 
void syntaxError(tokenAAnalizar * (tokenActual), char caracter);

#endif