#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H

/*Constantes tokens*/
#define PARENTESIS_ABRE		1
#define PARENTESIS_CIERRA	2
#define CORCHETES_ABRE		3
#define CORCHETES_CIERRA	4
#define COMA				5
#define PUNTO_COMA			6
#define DOS_PUNTOS			7
#define GUION_BAJO			8
#define PALABRA_RESERVADA	9
#define ID					10
#define CTE_STRING			11
#define CTE_NUMERICA		12
#define OP_SUMA				13
#define OP_RESTA			14
#define OP_MULTIPLICACION	15
#define OP_DIVISION			16
#define OP_MAYOR			17
#define OP_MAYOR_IGUAL		18
#define OP_MENOR			19
#define OP_MENOR_IGUAL		20
#define OP_IGUAL			21
#define OP_DISTINTO			22
#define OP_ASIGNACION		23
#define OP_CONCATENACION	24

/*Constantes TRUE y FALSE*/
#define TRUE	1
#define FALSE	0

/*Constante de longitud máxima de un token*/
#define MAX_LONG_TOKEN	50

/*Constante de ESTADO_FINAL*/
#define ESTADO_FINAL	29

/*Constante de cantidad de columnas*/
#define CANT_COLUMNAS	10

/*Estructura que describe al token que está siendo analizado*/
struct tokenAAnalizar
{
	int token[MAX_LONG_TOKEN];
	int longitudToken;
};
typedef struct tokenAAnalizar tokenAAnalizar;

/*Estructura que describe a la Tabla de Símbolos o TS*/
struct tablaDeSimbolos
{
	int token[MAX_LONG_TOKEN];
	int tipo;
};
typedef struct tablaDeSimbolos tablaDeSimbolos;

/*Funciones*/
int yylex();
int determinarColumna(char caracter);
void insertarEnTS(tokenAAnalizar *tokenActual, const int tipoDeToken);

void iniciarId(tokenAAnalizar *tokenActual, char caracter);
void continuarId(tokenAAnalizar *tokenActual, char caracter);
void finalizarId(tokenAAnalizar *tokenActual, char caracter);

//CTE_ENTERA o CTE_REAL
void iniciarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter);
void continuarCteEnteraOReal(tokenAAnalizar *tokenActual, char caracter);
void finalizarCteEntera(tokenAAnalizar *tokenActual, char caracter);
void iniciarCteReal(tokenAAnalizar *tokenActual, char caracter);
void continuarCteReal(tokenAAnalizar *tokenActual, char caracter);
void finalizarCteReal(tokenAAnalizar *tokenActual, char caracter);





void iniciarCteStr(tokenAAnalizar *tokenActual, char caracter);
void continuarCteStr(tokenAAnalizar *tokenActual, char caracter);
void finalizarCteStr(tokenAAnalizar *tokenActual, char caracter);




void iniciarOpSumaOConcat(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpSuma(tokenAAnalizar *tokenActual, char caracter);
void continuarOpConcat(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpConcat(tokenAAnalizar *tokenActual, char caracter);


//OP_RESTA o comentario 
void iniciarOpRestaOComent(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpResta(tokenAAnalizar *tokenActual, char caracter);
void continuarComentario(tokenAAnalizar *tokenActual, char caracter);
void finalizarComentario(tokenAAnalizar *tokenActual, char caracter);

//OP_MULTIPLICACION 
void iniciarOpMulti(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMulti(tokenAAnalizar *tokenActual, char caracter);

//OP_DIVISION 
void iniciarOpDivi(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpDivi(tokenAAnalizar *tokenActual, char caracter);

//PARENTESIS_ABRE 
void iniciarParAbre(tokenAAnalizar *tokenActual, char caracter);
void finalizarParAbre(tokenAAnalizar *tokenActual, char caracter);

//PARENTESIS_CIERRA 
void iniciarParCierra(tokenAAnalizar *tokenActual, char caracter);
void finalizarParCierra(tokenAAnalizar *tokenActual, char caracter);

//CORCHETES_ABRE 
void iniciarCorAbre(tokenAAnalizar *tokenActual, char caracter);
void finalizarCorAbre(tokenAAnalizar *tokenActual, char caracter);

//CORCHETES_CIERRA 
void iniciarCorCierra(tokenAAnalizar *tokenActual, char caracter);
void finalizarCorCierra(tokenAAnalizar *tokenActual, char caracter);

//PUNTO_COMA 
void iniciarPuntoComa(tokenAAnalizar *tokenActual, char caracter);
void finalizarPuntoComa(tokenAAnalizar *tokenActual, char caracter);

//DOS_PUNTOS 
void iniciarDosPuntos(tokenAAnalizar *tokenActual, char caracter);
void finalizarDosPuntos(tokenAAnalizar *tokenActual, char caracter);

//GUION_BAJO 
void iniciarGuionBajo(tokenAAnalizar *tokenActual, char caracter);
void finalizarGuionBajo(tokenAAnalizar *tokenActual, char caracter);

//COMA 
void iniciarComa(tokenAAnalizar *tokenActual, char caracter);
void finalizarComa(tokenAAnalizar *tokenActual, char caracter);

//OP_MENOR u OP_MENOR_IGUAL 
void iniciarOpMenorOMenorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMenor(tokenAAnalizar *tokenActual, char caracter);
void continuarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMenorIgual(tokenAAnalizar *tokenActual, char caracter);

//OP_MAYOR u OP_MAYOR_IGUAL 
void iniciarOpMayorOMayorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMayor(tokenAAnalizar *tokenActual, char caracter);
void continuarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpMayorIgual(tokenAAnalizar *tokenActual, char caracter);

//OP_ASIGNACION u OP_IGUAL 
void iniciarOpAsigOIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpAsig(tokenAAnalizar *tokenActual, char caracter);
void continuarOpIgual(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpIgual(tokenAAnalizar *tokenActual, char caracter);

//OP_DISTINTO 
void iniciarOpDistinto(tokenAAnalizar *tokenActual, char caracter);
void continuarOpDistinto(tokenAAnalizar *tokenActual, char caracter);
void finalizarOpDistinto(tokenAAnalizar *tokenActual, char caracter);


//Espacios vacíos (ENTER, TAB, ESPACIO) 
void ignorarEspacios(tokenAAnalizar * (tokenActual), char caracter);

//Error de sintáxis 
void syntaxError(tokenAAnalizar * (tokenActual), char caracter);
	



/*Matriz de Nuevo Estado*/
int nuevoEstado[ESTADO_FINAL][CANT_COLUMNAS];

/*Matriz de proceso*/
void (*proceso[ESTADO_FINAL][CANT_COLUMNAS])(tokenAAnalizar *, char);

#endif