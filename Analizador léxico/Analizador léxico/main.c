
# line 2 "main.y"
#include "AnalizadorLexico.h"
#include "Pila.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *salidaAS;
FILE *archivo;
extern FILE *archivoDeTokens;

extern tablaDeSimbolos TS[LONG_TS];
extern int cantTokensEnTS;
int cantIDsEnTS = 0;

int cantIDsEnDeclaracion = 0;
int posicionIDEnTS[50];

int cantTiposEnDeclaracion = 0;
int tipoDeID[50];

extern int lineaActual;

int aux = 0;

PilaDeInt pilaExpresiones;
PilaDeInt pilaCondiciones;
PilaDeColas pilaColasTercetos;
ColaDeTercetos *ptrColaTercetos;

int tercetoInicial;

Terceto tercetoAux;

extern int cantTercetos;
extern Terceto listaDeTercetos[MAX_TERCETOS];

int registroBHUsado;

#define PAR_ABRE 257
#define PAR_CIERRA 258
#define COR_ABRE 259
#define COR_CIERRA 260
#define COMA 261
#define PUNTO_COMA 262
#define DOS_PUNTOS 263
#define GUION_BAJO 264
#define ID 265
#define CTE_STRING 266
#define CTE_ENTERA 267
#define CTE_REAL 268
#define OP_SUMA 269
#define OP_RESTA 270
#define OP_MULTIPLICACION 271
#define OP_DIVISION 272
#define OP_MAYOR 273
#define OP_MAYOR_IGUAL 274
#define OP_MENOR 275
#define OP_MENOR_IGUAL 276
#define OP_IGUAL 277
#define OP_DISTINTO 278
#define OP_ASIGNACION 279
#define OP_CONCATENACION 280
#define PR_IF 281
#define PR_FI 282
#define PR_FOR 283
#define PR_ROF 284
#define PR_ELSE 285
#define PR_WPRINT 286
#define PR_FILTERC 287
#define PR_DO 288
#define PR_WHILE 289
#define PR_AND 290
#define PR_OR 291
#define PR_NOT 292
#define PR_VAR 293
#define PR_ENDVAR 294
#define PR_INT 295
#define PR_FLOAT 296
#define PR_STRING 297
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
#define YYERRCODE 256

# line 1500 "main.y"



int main(int argc, char *argv[])
{
	vaciarPilaDeInt(&pilaExpresiones);
	vaciarPilaDeInt(&pilaCondiciones);
	vaciarPilaDeCola(&pilaColasTercetos);

	if(argc != 2)
	{
		printf("Formato invalido\nSe espera: Compilador <archivo_fuente>");
		return 1;
	}

    fopen_s(&archivo,argv[1],"r");
	fopen_s(&salidaAS,"reglas_aplicadas.txt","w");
	
    if(archivo == NULL || salidaAS == NULL)
    {
        printf("No se pudo abrir archivos");
    }
    else
    {
        inicializarAL(archivo);

		yyparse();
		
        fclose(archivo);
		fclose(salidaAS);
    }

	imprimirTercetos();

    return 0;
}

int yyerror(char *mensaje)
{
	fprintf(salidaAS,"\nLinea %d: %s",lineaActual,mensaje);
	
	fclose(archivo);
	fclose(archivoDeTokens);
	fclose(salidaAS);
	
	return 1;
}

void equilibrarDeclaracion()
{
	if(cantIDsEnDeclaracion > cantTiposEnDeclaracion)
	{
		for( ;cantIDsEnDeclaracion > cantTiposEnDeclaracion; --cantIDsEnDeclaracion, --cantTokensEnTS);
	}
	
	if(cantIDsEnDeclaracion < cantTiposEnDeclaracion)
	{
		for( ;cantIDsEnDeclaracion < cantTiposEnDeclaracion; --cantTiposEnDeclaracion);
	}
}

void asignarTipoDeDato()
{
	int i;
	
	for(i = 0; i < cantIDsEnDeclaracion; ++i)
	{
		TS[posicionIDEnTS[i]].tipo = tipoDeID[i];
	}
}

void lanzarError(char *mensaje)
{
	fprintf(salidaAS,"\nLinea %d: %s",lineaActual,mensaje);
		
	fclose(archivo);
	fclose(archivoDeTokens);
	fclose(salidaAS);

	exit(1);
}

void verificarDeclaracion(int posicionEnTS)
{
	char mensaje[50]; 

	if(posicionEnTS >= cantIDsEnTS)
	{
		strcpy_s(mensaje,50,"\"");

		strcat_s(mensaje,50,TS[posicionEnTS].nombre);
		
		strcat_s(mensaje,50,"\" variable no declarada");
		
		lanzarError(mensaje); 	
	}
}

FILE *yytfilep;
char *yytfilen;
int yytflag = 0;
int svdprd[2];
char svdnams[2][2];

int yyexca[] = {
  -1, 1,
  0, -1,
  -2, 0,
  -1, 3,
  286, 43,
  -2, 3,
  -1, 6,
  268, 53,
  -2, 50,
  -1, 16,
  0, 33,
  -2, 34,
  -1, 28,
  266, 50,
  268, 53,
  -2, 56,
  -1, 33,
  294, 5,
  -2, 7,
  -1, 59,
  269, 127,
  270, 130,
  -2, 90,
  -1, 60,
  280, 97,
  -2, 91,
  -1, 62,
  280, 96,
  -2, 141,
  -1, 63,
  271, 134,
  272, 137,
  -2, 133,
  -1, 73,
  289, 73,
  -2, 34,
  -1, 79,
  261, 22,
  -2, 25,
  -1, 99,
  282, 87,
  -2, 34,
  -1, 105,
  269, 127,
  270, 130,
  -2, 145,
  -1, 112,
  290, 99,
  291, 102,
  -2, 98,
  -1, 114,
  270, 130,
  273, 109,
  274, 112,
  275, 115,
  276, 118,
  277, 121,
  278, 124,
  -2, 127,
  -1, 142,
  271, 134,
  272, 137,
  -2, 129,
  -1, 143,
  271, 134,
  272, 137,
  -2, 132,
  -1, 151,
  290, 157,
  291, 160,
  -2, 156,
  -1, 153,
  274, 170,
  275, 173,
  276, 176,
  277, 179,
  278, 182,
  -2, 167,
  -1, 205,
  282, 84,
  285, 84,
  -2, 34,
  -1, 209,
  269, 127,
  270, 130,
  -2, 111,
  -1, 210,
  269, 127,
  270, 130,
  -2, 114,
  -1, 211,
  269, 127,
  270, 130,
  -2, 117,
  -1, 212,
  269, 127,
  270, 130,
  -2, 120,
  -1, 213,
  269, 127,
  270, 130,
  -2, 123,
  -1, 214,
  269, 127,
  270, 130,
  -2, 126,
  -1, 236,
  269, 127,
  270, 130,
  -2, 169,
  -1, 237,
  269, 127,
  270, 130,
  -2, 172,
  -1, 238,
  269, 127,
  270, 130,
  -2, 175,
  -1, 239,
  269, 127,
  270, 130,
  -2, 178,
  -1, 240,
  269, 127,
  270, 130,
  -2, 181,
  -1, 241,
  269, 127,
  270, 130,
  -2, 184,
  -1, 247,
  261, 26,
  -2, 29,
  -1, 257,
  261, 185,
  -2, 153,
  -1, 258,
  269, 127,
  270, 130,
  -2, 188,
  -1, 278,
  269, 127,
  270, 130,
  -2, 187,
  0,
};

#define YYNPROD 189
#define YYLAST 379

int yyact[] = {
     248,     249,     250,      68,      76,       7,     153,      68,
      27,     195,     114,     106,       4,      66,      67,      62,
      60,      66,      67,     158,     194,     157,     110,       7,
      31,     280,      29,      56,     104,       7,      57,      30,
      68,      70,     152,     141,      16,      70,     113,      42,
     106,     202,      66,      67,     165,     201,     164,     200,
     163,     199,     197,     162,     198,      59,     161,     160,
     103,     102,     101,     246,     100,      37,      70,     147,
     148,      36,      27,      79,      71,     153,      89,     215,
     167,     242,     154,      98,      75,      54,      35,     270,
     263,     193,      73,     117,     272,     269,     262,     166,
     116,     245,     232,     216,      77,      34,     277,     271,
     259,     244,     243,     156,     105,     149,     108,      81,
      80,     196,     159,     155,     107,      74,      83,      72,
      38,     111,      64,     151,     185,      21,     112,      17,
      99,      65,     275,      78,      28,       6,     265,     226,
     178,      22,       5,      28,     225,      63,       6,       9,
     181,     177,     224,      32,     176,     223,     175,     222,
     174,     221,     173,     252,     220,     172,     219,     171,
     218,     170,     274,     264,     257,     251,     217,      53,
      58,     169,      22,     150,     125,      90,      69,     124,
      89,     122,      87,     121,      86,      17,     120,      85,
      92,     119,      84,     190,     137,     189,     136,     188,
     135,     187,     134,     186,     133,     185,     132,     229,
     184,     131,     183,     130,     209,     210,     211,     212,
     213,     214,     182,     129,     123,      88,      61,     118,
      83,     181,     109,     128,      94,      46,      55,      41,
      26,     228,     205,     115,     186,     187,     188,     189,
     190,     221,     180,     253,     127,      93,      45,     276,
     236,     237,     238,     239,     240,     241,     146,     140,
     266,     253,     276,     144,     145,     227,     179,     222,
     223,     224,     225,     226,     251,     142,     143,     126,
      92,      44,      25,      24,      14,      64,     258,      91,
     168,      43,     122,      65,      52,     275,      13,      51,
      12,      23,      11,      47,     120,      63,       9,      20,
      19,      18,      40,      82,      39,     268,     278,     256,
     247,     114,     140,      97,      78,     203,     204,     255,
     231,     192,     139,      96,      50,     206,     207,     208,
     267,     254,     230,     191,     138,      95,      49,      48,
      33,     279,     261,      10,      16,     180,     111,       3,
      15,     268,       8,       2,       1,     183,     184,     112,
     273,       0,       0,       0,       0,     246,     233,     234,
     235,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,     219,     220,     151,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,     260,       0,       0,       0,       0,
       0,       0,      21,
};

int yypact[] = {
    -281,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -257,    -263,    -166,    -184,    -201,    -207,    -145,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -240,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -257,
    -185,    -255,    -250,    -197,    -146,    -257,    -148,    -186,
    -290,    -167,    -198,    -154,    -155,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -240,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -187,    -257,    -209,    -212,    -214,    -216,
    -252,    -225,    -149,    -156,    -199,    -267,    -254,    -198,
    -172,    -178,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -247,    -225,
    -225,    -225,    -225,    -202,    -157,    -258,    -188,    -150,
    -159,    -269,    -272,    -151,    -218,    -220,    -224,    -228,
    -231,    -234,    -173,    -191,    -198,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -180,    -270,    -282,    -152,    -223,    -222,    -226,
    -229,    -232,    -237,    -254,    -254,    -257,    -225,    -225,
    -225,    -225,    -225,    -225,    -225,    -225,    -225,    -192,
    -168,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -169,    -195,    -195,    -195,    -225,    -225,    -225,
    -225,    -225,    -225,    -189,    -160,    -161,    -170,    -295,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -225,    -162,    -199,    -295,    -174,
    -181,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -175,    -182,    -163,    -176,    -295,   -1000,   -1000,   -1000,
   -1000,   -1000,    -164,    -225,    -257,   -1000,   -1000,    -259,
   -1000,
};

int yypgo[] = {
       0,     324,     323,     322,     320,     319,     315,     312,
     311,     310,     309,     123,     308,     307,     306,     305,
      59,     304,     300,     299,     298,     297,     296,     295,
     291,     290,     288,     287,     285,      36,     284,     119,
     283,     282,     281,     280,     279,     117,     278,     129,
     275,     274,     273,     124,     272,     271,     270,     268,
     265,     263,     260,     259,     258,     257,     256,     255,
     246,     113,     245,     241,     240,     231,     230,     229,
     228,     226,     217,     216,     215,     214,     213,     212,
     211,     209,     208,     207,      10,     206,     114,     205,
     204,     118,     203,     202,     195,     194,     193,     192,
     191,     190,     189,     188,     187,     186,     185,     184,
     183,     182,     181,     180,     179,     178,     177,     133,
     175,     174,     172,     171,     121,     170,     169,     168,
     167,     166,     165,     164,     163,     161,     158,     157,
     156,     155,     154,     115,     153,     152,     151,     150,
     149,     148,     147,     146,     145,     144,     143,     142,
     141,     140,     138,     137,     132,     128,     127,     126,
     122,
};

int yyr1[] = {
       0,       3,       1,       1,       6,       8,       2,       9,
      10,      12,      13,      14,      15,      17,       7,      18,
      19,      20,      21,      22,      23,       7,      24,      25,
      11,      11,      27,      28,      16,      16,      26,      26,
      26,       4,      30,      32,      29,      33,      29,      31,
      31,      31,      31,      38,      40,       5,      41,       5,
      34,      34,      44,      45,      39,      46,      47,      39,
      48,      49,      42,      50,      43,      35,      35,      53,
      54,      55,      56,      58,      59,      60,      61,      51,
      62,      63,      64,      65,      66,      52,      68,      36,
      70,      71,      72,      73,      67,      69,      74,      75,
      69,      37,      37,      37,      37,      79,      80,      77,
      78,      78,      57,      82,      83,      57,      84,      85,
      57,      86,      87,      88,      57,      89,      90,      81,
      91,      92,      81,      93,      94,      81,      95,      96,
      81,      97,      98,      81,      99,     100,      81,     101,
     102,      76,     104,     105,      76,      76,     106,     107,
     103,     109,     110,     103,     103,     108,     108,     108,
     111,     112,     108,     108,     114,     115,     117,     118,
     119,     121,     122,     113,     116,     124,     125,     116,
     126,     127,     116,     128,     129,     130,     116,     131,
     132,     123,     133,     134,     123,     135,     136,     123,
     137,     138,     123,     139,     140,     123,     141,     142,
     123,     143,     144,     120,     120,
};

int yyr2[] = {
       2,       0,       3,       1,       0,       0,       5,       0,
       0,       0,       0,       0,       0,       0,      15,       0,
       0,       0,       0,       0,       0,      13,       0,       0,
       5,       1,       0,       0,       5,       1,       1,       1,
       1,       1,       0,       0,       5,       0,       3,       1,
       1,       1,       1,       0,       0,       5,       0,       3,
       1,       1,       0,       0,       5,       0,       0,       5,
       0,       0,       5,       0,       3,       1,       1,       0,
       0,       0,       0,       0,       0,       0,       0,      18,
       0,       0,       0,       0,       0,      11,       0,       3,
       0,       0,       0,       0,       9,       1,       0,       0,
       5,       3,       3,       3,       3,       0,       0,       5,
       1,       1,       1,       0,       0,       5,       0,       0,
       5,       0,       0,       0,       7,       0,       0,       5,
       0,       0,       5,       0,       0,       5,       0,       0,
       5,       0,       0,       5,       0,       0,       5,       0,
       0,       5,       0,       0,       5,       1,       0,       0,
       5,       0,       0,       5,       1,       1,       1,       1,
       0,       0,       5,       1,       0,       0,       0,       0,
       0,       0,       0,      15,       1,       0,       0,       5,
       0,       0,       5,       0,       0,       0,       7,       0,
       0,       5,       0,       0,       5,       0,       0,       5,
       0,       0,       5,       0,       0,       5,       0,       0,
       5,       0,       0,       5,       1,
};

int yychk[] = {
   -1000,      -1,      -2,      -5,     293,     -39,     -43,     286,
      -3,     -38,      -6,     -41,     -44,     -46,     -50,      -4,
     -29,     -31,     -34,     -35,     -36,     -37,     -39,     -42,
     -51,     -52,     -67,     265,     -43,     283,     288,     281,
     -39,      -7,     259,     262,     266,     268,     257,     -30,
     -33,     -68,     279,     -48,     -53,     -62,     -70,     -40,
      -8,      -9,     -18,     -45,     -47,     -31,     262,     -69,
     282,     285,     -37,     -76,     266,     -77,     265,    -103,
     -78,    -108,     267,     268,     257,    -113,     287,     265,
     257,     -29,     257,     262,     294,     259,     -11,     265,
     258,     258,     -32,     -74,    -101,    -104,    -106,    -109,
     -79,    -111,    -114,     -49,     -54,     -63,     -71,     -10,
     -19,     -24,     262,     -29,     269,     270,     271,     272,
     280,     -76,     265,     257,     258,     -37,     289,     -57,
     -81,     292,     -76,     -11,     260,     261,     -75,    -102,
    -105,    -107,    -110,     -80,    -112,    -115,     -55,     -64,
     -72,     -82,     -84,     -86,     -89,     -91,     -93,     -95,
     -97,     -99,     -12,     -20,     -25,     282,    -103,    -103,
    -108,    -108,     -78,     265,     266,     258,    -116,    -123,
     292,     264,     262,     257,     258,     290,     291,     257,
     273,     274,     275,     276,     277,     278,     260,     263,
     -11,    -117,    -124,    -126,    -128,    -131,    -133,    -135,
    -137,    -139,    -141,     -56,     -65,     -73,     -83,     -85,
     -87,     -90,     -92,     -94,     -96,     -98,    -100,     -13,
     -21,     261,     290,     291,     257,     273,     274,     275,
     276,     277,     278,     -57,     -57,     -29,     -81,     -81,
     -81,     -76,     -76,     -76,     -76,     -76,     -76,     263,
     259,    -118,    -125,    -127,    -129,    -132,    -134,    -136,
    -138,    -140,    -142,     -58,     -66,     -88,     -14,     -22,
     259,    -123,    -123,    -123,     -76,     -76,     -76,     -76,
     -76,     -76,     262,     258,     258,     259,     -16,     -26,
     295,     296,     297,    -119,    -130,     -59,     -15,     -23,
     -27,    -120,     -76,     258,     -37,     -16,     260,     261,
    -121,    -143,     -60,     -17,     -28,     260,     261,     258,
     260,     -16,    -122,    -144,     -61,     258,     -76,     -29,
     284,
};

int yydef[] = {
       0,      -2,       1,      -2,       4,      46,      -2,      59,
       0,       0,       0,       0,       0,       0,       0,       2,
      -2,      37,      39,      40,      41,      42,      48,      49,
      61,      62,      78,       0,      -2,      63,      72,      80,
      44,      -2,      15,      47,      51,      54,      60,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,      35,      38,      79,
      85,      86,      89,      -2,      -2,      92,      -2,      -2,
      93,     140,     142,     143,     144,     147,     148,      57,
      64,      -2,      81,      45,       6,       8,      16,      -2,
      52,      55,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,      36,      -2,     128,     131,     135,     138,
      94,      -2,     141,     149,      58,      65,      74,      82,
      -2,     105,      -2,       9,      17,      23,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,      88,      -2,      -2,
     136,     139,      95,      96,      97,     146,     150,      -2,
     163,      -2,      66,      75,      83,     100,     103,     106,
     110,     113,     116,     119,     122,     125,      10,      18,
      24,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,     151,     158,     161,     164,     168,     171,     174,
     177,     180,     183,      67,      76,      -2,     101,     104,
     107,      -2,      -2,      -2,      -2,      -2,      -2,      11,
      19,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
     152,     159,     162,     165,      -2,      -2,      -2,      -2,
      -2,      -2,      68,      77,     108,      12,      20,      -2,
      30,      31,      32,       0,       0,       0,       0,       0,
       0,      -2,      -2,     166,      69,      13,      21,      27,
       0,       0,       0,       0,       0,     154,     186,      70,
      14,      28,       0,       0,       0,     155,      -2,      34,
      71,
};

int *yyxi;


/*****************************************************************/
/* PCYACC LALR parser driver routine -- a table driven procedure */
/* for recognizing sentences of a language defined by the        */
/* grammar that PCYACC analyzes. An LALR parsing table is then   */
/* constructed for the grammar and the skeletal parser uses the  */
/* table when performing syntactical analysis on input source    */
/* programs. The actions associated with grammar rules are       */
/* inserted into a switch statement for execution.               */
/*****************************************************************/


#ifndef YYMAXDEPTH
#define YYMAXDEPTH 200
#endif
#ifndef YYREDMAX
#define YYREDMAX 1000
#endif
#define PCYYFLAG -1000
#define WAS0ERR 0
#define WAS1ERR 1
#define WAS2ERR 2
#define WAS3ERR 3
#define yyclearin pcyytoken = -1
#define yyerrok   pcyyerrfl = 0
YYSTYPE yyv[YYMAXDEPTH];     /* value stack */
int pcyyerrct = 0;           /* error count */
int pcyyerrfl = 0;           /* error flag */
int redseq[YYREDMAX];
int redcnt = 0;
int pcyytoken = -1;          /* input token */


yyparse()
{
  int statestack[YYMAXDEPTH]; /* state stack */
  int      j, m;              /* working index */
  YYSTYPE *yypvt;
  int      tmpstate, tmptoken, *yyps, n;
  YYSTYPE *yypv;


  tmpstate = 0;
  pcyytoken = -1;
#ifdef YYDEBUG
  tmptoken = -1;
#endif
  pcyyerrct = 0;
  pcyyerrfl = 0;
  yyps = &statestack[-1];
  yypv = &yyv[-1];


  enstack:    /* push stack */
#ifdef YYDEBUG
    printf("at state %d, next token %d\n", tmpstate, tmptoken);
#endif
    if (++yyps - &statestack[YYMAXDEPTH] > 0) {
      yyerror("pcyacc internal stack overflow");
      return(1);
    }
    *yyps = tmpstate;
    ++yypv;
    *yypv = yyval;


  newstate:
    n = yypact[tmpstate];
    if (n <= PCYYFLAG) goto defaultact; /*  a simple state */


    if (pcyytoken < 0) if ((pcyytoken=yylex()) < 0) pcyytoken = 0;
    if ((n += pcyytoken) < 0 || n >= YYLAST) goto defaultact;


    if (yychk[n=yyact[n]] == pcyytoken) { /* a shift */
#ifdef YYDEBUG
      tmptoken  = pcyytoken;
#endif
      pcyytoken = -1;
      yyval = yylval;
      tmpstate = n;
      if (pcyyerrfl > 0) --pcyyerrfl;
      goto enstack;
    }


  defaultact:


    if ((n=yydef[tmpstate]) == -2) {
      if (pcyytoken < 0) if ((pcyytoken=yylex())<0) pcyytoken = 0;
      for (yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=tmpstate); yyxi += 2);
      while (*(yyxi+=2) >= 0) if (*yyxi == pcyytoken) break;
      if ((n=yyxi[1]) < 0) { /* an accept action */
        if (yytflag) {
          int ti; int tj;
          yytfilep = fopen(yytfilen, "w");
          if (yytfilep == NULL) {
            fprintf(stderr, "Can't open t file: %s\n", yytfilen);
            return(0);          }
          for (ti=redcnt-1; ti>=0; ti--) {
            tj = svdprd[redseq[ti]];
            while (strcmp(svdnams[tj], "$EOP"))
              fprintf(yytfilep, "%s ", svdnams[tj++]);
            fprintf(yytfilep, "\n");
          }
          fclose(yytfilep);
        }
        return (0);
      }
    }


    if (n == 0) {        /* error situation */
      switch (pcyyerrfl) {
        case WAS0ERR:          /* an error just occurred */
          yyerror("syntax error");
          yyerrlab:
            ++pcyyerrct;
        case WAS1ERR:
        case WAS2ERR:           /* try again */
          pcyyerrfl = 3;
	   /* find a state for a legal shift action */
          while (yyps >= statestack) {
	     n = yypact[*yyps] + YYERRCODE;
	     if (n >= 0 && n < YYLAST && yychk[yyact[n]] == YYERRCODE) {
	       tmpstate = yyact[n];  /* simulate a shift of "error" */
	       goto enstack;
            }
	     n = yypact[*yyps];


	     /* the current yyps has no shift on "error", pop stack */
#ifdef YYDEBUG
            printf("error: pop state %d, recover state %d\n", *yyps, yyps[-1]);
#endif
	     --yyps;
	     --yypv;
	   }


	   yyabort:
            if (yytflag) {
              int ti; int tj;
              yytfilep = fopen(yytfilen, "w");
              if (yytfilep == NULL) {
                fprintf(stderr, "Can't open t file: %s\n", yytfilen);
                return(1);              }
              for (ti=1; ti<redcnt; ti++) {
                tj = svdprd[redseq[ti]];
                while (strcmp(svdnams[tj], "$EOP"))
                  fprintf(yytfilep, "%s ", svdnams[tj++]);
                fprintf(yytfilep, "\n");
              }
              fclose(yytfilep);
            }
	     return(1);


	 case WAS3ERR:  /* clobber input char */
#ifdef YYDEBUG
          printf("error: discard token %d\n", pcyytoken);
#endif
          if (pcyytoken == 0) goto yyabort; /* quit */
	   pcyytoken = -1;
	   goto newstate;      } /* switch */
    } /* if */


    /* reduction, given a production n */
#ifdef YYDEBUG
    printf("reduce with rule %d\n", n);
#endif
    if (yytflag && redcnt<YYREDMAX) redseq[redcnt++] = n;
    yyps -= yyr2[n];
    yypvt = yypv;
    yypv -= yyr2[n];
    yyval = yypv[1];
    m = n;
    /* find next state from goto table */
    n = yyr1[n];
    j = yypgo[n] + *yyps + 1;
    if (j>=YYLAST || yychk[ tmpstate = yyact[j] ] != -n) tmpstate = yyact[yypgo[n]];
    switch (m) { /* actions associated with grammar rules */
      
      case 1:
# line 94 "main.y"
      {
      				++aux;
      			} break;
      case 2:
# line 99 "main.y"
      {
      				GenerarAssembler();
      			} break;
      case 3:
# line 104 "main.y"
      {
      				++aux;
      			} break;
      case 4:
# line 110 "main.y"
      {
      						fprintf(salidaAS,"VAR\n");
      					} break;
      case 5:
# line 115 "main.y"
      {
      						++aux;
      					} break;
      case 6:
# line 120 "main.y"
      {
      						fprintf(salidaAS,"ENDVAR\n");
      					} break;
      case 7:
# line 126 "main.y"
      {
      					++aux;
      				} break;
      case 8:
# line 131 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 9:
# line 136 "main.y"
      {
      					++aux;
      				} break;
      case 10:
# line 141 "main.y"
      {
      					fprintf(salidaAS,"]");
      				} break;
      case 11:
# line 146 "main.y"
      {
      					fprintf(salidaAS,":");
      				} break;
      case 12:
# line 151 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 13:
# line 156 "main.y"
      {
      					++aux;
      				} break;
      case 14:
# line 161 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 15:
# line 174 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 16:
# line 179 "main.y"
      {
      					++aux;
      				} break;
      case 17:
# line 184 "main.y"
      {
      					fprintf(salidaAS,"]");
      				} break;
      case 18:
# line 189 "main.y"
      {
      					fprintf(salidaAS,":");
      				} break;
      case 19:
# line 194 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 20:
# line 199 "main.y"
      {
      					++aux;
      				} break;
      case 21:
# line 204 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 22:
# line 218 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      	
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 23:
# line 225 "main.y"
      {
      						fprintf(salidaAS,",");
      					} break;
      case 24:
# line 230 "main.y"
      {
      						++aux;
      					} break;
      case 25:
# line 235 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      	
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 26:
# line 243 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 27:
# line 248 "main.y"
      {
      					fprintf(salidaAS,",");
      				} break;
      case 28:
# line 253 "main.y"
      {
      					++aux;
      				} break;
      case 29:
# line 258 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 30:
# line 264 "main.y"
      {
      			fprintf(salidaAS,"int");
      	
      			yyval = PR_INT;
      		} break;
      case 31:
# line 271 "main.y"
      {
      			fprintf(salidaAS,"float");
      	
      			yyval = PR_FLOAT;
      		} break;
      case 32:
# line 278 "main.y"
      {
      			fprintf(salidaAS,"string");
      	
      			yyval = PR_STRING;
      		} break;
      case 33:
# line 286 "main.y"
      {
      						++aux;
      					} break;
      case 34:
# line 292 "main.y"
      {
      						++aux;
      					} break;
      case 35:
# line 297 "main.y"
      {
      						++aux;
      					} break;
      case 36:
# line 302 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 37:
# line 307 "main.y"
      {
      						++aux;
      					} break;
      case 38:
# line 312 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 39:
# line 318 "main.y"
      {
      				++aux;
      			} break;
      case 40:
# line 323 "main.y"
      {
      				++aux;
      			} break;
      case 41:
# line 328 "main.y"
      {
      				++aux;
      			} break;
      case 42:
# line 333 "main.y"
      {
      				++aux;
      			} break;
      case 43:
# line 339 "main.y"
      {
      						++aux;
      					} break;
      case 44:
# line 344 "main.y"
      {
      						++aux;
      					} break;
      case 45:
# line 349 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 46:
# line 354 "main.y"
      {
      						++aux;
      					} break;
      case 47:
# line 359 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 48:
# line 365 "main.y"
      {
      			++aux;
      		} break;
      case 49:
# line 370 "main.y"
      {
      			++aux;
      		} break;
      case 50:
# line 376 "main.y"
      {
      				++aux;
      			} break;
      case 51:
# line 381 "main.y"
      {
      				fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      			} break;
      case 52:
# line 386 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 53:
# line 391 "main.y"
      {
      				++aux;
      			} break;
      case 54:
# line 396 "main.y"
      {
      				fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			} break;
      case 55:
# line 401 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 56:
# line 407 "main.y"
      {
      				++aux;
      			} break;
      case 57:
# line 412 "main.y"
      {
      				fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      			} break;
      case 58:
# line 417 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 59:
# line 422 "main.y"
      {
      					fprintf(salidaAS,"WPRINT");
      				} break;
      case 60:
# line 427 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 61:
# line 433 "main.y"
      {
      				++aux;
      			} break;
      case 62:
# line 438 "main.y"
      {
      				++aux;
      			} break;
      case 63:
# line 444 "main.y"
      {
      					fprintf(salidaAS,"FOR");
      				} break;
      case 64:
# line 449 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 65:
# line 454 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 66:
# line 459 "main.y"
      {
      					fprintf(salidaAS,";");
      
					registroBHUsado = FALSE;
      				} break;
      case 67:
# line 466 "main.y"
      {
      					tercetoInicial = cantTercetos;
      				} break;
      case 68:
# line 471 "main.y"
      {
      					fprintf(salidaAS,";");
      				} break;
      case 69:
# line 476 "main.y"
      {
      					ptrColaTercetos = (ColaDeTercetos *)malloc(sizeof(ColaDeTercetos));
      
					vaciarColaDeTercetos(ptrColaTercetos);
      
					for(aux = tercetoInicial; aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],ptrColaTercetos);
      					}
      
					pushCola(ptrColaTercetos,&pilaColasTercetos);
      					cantTercetos = tercetoInicial;
      				} break;
      case 70:
# line 491 "main.y"
      {
      					fprintf(salidaAS,")\n");
      				} break;
      case 71:
# line 498 "main.y"
      {
      					fprintf(salidaAS,"ROF");
      
					ptrColaTercetos = popCola(&pilaColasTercetos);
      					ajustarTercetos(ptrColaTercetos,cantTercetos);
      
					while(colaDeTercetosEstaVacia(ptrColaTercetos) == FALSE)
      					{
      						crearTerceto(desencolarTerceto(ptrColaTercetos));
      					}
      
					free(ptrColaTercetos);
      
					aux = popInt(&pilaCondiciones);
      					listaDeTercetos[aux].y = cantTercetos + 1;
      					listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					
      					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JMP;
      					tercetoAux.y = popInt(&pilaCondiciones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      				} break;
      case 72:
# line 524 "main.y"
      {
      						fprintf(salidaAS,"DO\n");
      
						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 73:
# line 531 "main.y"
      {
      						++aux;
      					} break;
      case 74:
# line 536 "main.y"
      {
      						fprintf(salidaAS,"WHILE");
      					} break;
      case 75:
# line 541 "main.y"
      {
      						fprintf(salidaAS,"(");
      
						registroBHUsado = FALSE;
      					} break;
      case 76:
# line 548 "main.y"
      {
      						++aux;
      					} break;
      case 77:
# line 553 "main.y"
      {
      						fprintf(salidaAS,")");
      
						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 78:
# line 564 "main.y"
      {
      				++aux;
      			} break;
      case 79:
# line 569 "main.y"
      {
      				++aux;
      			} break;
      case 80:
# line 574 "main.y"
      {
      						fprintf(salidaAS,"IF");
      					} break;
      case 81:
# line 579 "main.y"
      {
      						fprintf(salidaAS,"(");
      
						registroBHUsado = FALSE;
      					} break;
      case 82:
# line 586 "main.y"
      {
      						++aux;
      					} break;
      case 83:
# line 591 "main.y"
      {
      						fprintf(salidaAS,")\n");
      					} break;
      case 84:
# line 596 "main.y"
      {
      						++aux;
      					} break;
      case 85:
# line 601 "main.y"
      {
      						fprintf(salidaAS,"FI");
      
						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 86:
# line 610 "main.y"
      {
      						fprintf(salidaAS,"ELSE\n");
      
						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 87:
# line 622 "main.y"
      {
      						++aux;
      					} break;
      case 88:
# line 627 "main.y"
      {
      						fprintf(salidaAS,"FI");
      
						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 89:
# line 637 "main.y"
      {
      				fprintf(salidaAS,"(=%d)",yypvt[-2]);
      				printf("1 ASIGNACION -> ID := ASIGNACION\n");
      
				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = yypvt[-0];
      				tercetoAux.tipoDeZ = INDICE_TS; 
      
				crearTerceto(&tercetoAux);
      
				printf("tope=%d\n",pilaExpresiones.tope);
      
				yyval = yypvt[-2];
      			} break;
      case 90:
# line 656 "main.y"
      {
      				fprintf(salidaAS,"(=%d)",yypvt[-2]);
      				printf("1 ASIGNACION -> ID := EXPRESION\n");
      
				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO; 
      
				crearTerceto(&tercetoAux);
      
				yyval = yypvt[-2];
      			} break;
      case 93:
# line 677 "main.y"
      {
      					++aux;
      				} break;
      case 94:
# line 682 "main.y"
      {
      					fprintf(salidaAS," ++ "); 
      				} break;
      case 95:
# line 687 "main.y"
      {
      					++aux;
      				} break;
      case 96:
# line 692 "main.y"
      {
      									fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      								} break;
      case 97:
# line 697 "main.y"
      {
      									fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      								} break;
      case 98:
# line 703 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.tipoDeY = BL;
      				tercetoAux.z = 1;
      				tercetoAux.tipoDeZ = VALOR;
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_AND;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.tipoDeY = BH;
      				tercetoAux.tipoDeZ = BL;
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.tipoDeX = JZ;
      				pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      			} break;
      case 99:
# line 725 "main.y"
      {
      				++aux;
      			} break;
      case 100:
# line 730 "main.y"
      {
      				fprintf(salidaAS," AND ");
      			} break;
      case 101:
# line 735 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_AND;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.tipoDeY = BH;
      				tercetoAux.tipoDeZ = BL;
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.tipoDeX = JZ;
      				pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      			} break;
      case 102:
# line 749 "main.y"
      {
      				++aux;
      			} break;
      case 103:
# line 754 "main.y"
      {
      				fprintf(salidaAS," OR ");
      			} break;
      case 104:
# line 759 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_OR;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.tipoDeY = BH;
      				tercetoAux.tipoDeZ = BL;
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.tipoDeX = JZ;
      				pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      			} break;
      case 105:
# line 773 "main.y"
      {
      				fprintf(salidaAS," NOT ");
      			} break;
      case 106:
# line 778 "main.y"
      {
      				fprintf(salidaAS,"(");
      			} break;
      case 107:
# line 783 "main.y"
      {
      				++aux;
      			} break;
      case 108:
# line 788 "main.y"
      {
      				fprintf(salidaAS,")");
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_NOT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.tipoDeY = BH;
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.tipoDeX = JZ;
      				pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      			} break;
      case 109:
# line 804 "main.y"
      {
      					++aux;
      				} break;
      case 110:
# line 809 "main.y"
      {
      					fprintf(salidaAS," > ");
      				} break;
      case 111:
# line 814 "main.y"
      {
      					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JG;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 112:
# line 855 "main.y"
      {
      					++aux;
      				} break;
      case 113:
# line 860 "main.y"
      {
      					fprintf(salidaAS," >= ");
      				} break;
      case 114:
# line 865 "main.y"
      {
      					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JGE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 115:
# line 906 "main.y"
      {
      					++aux;
      				} break;
      case 116:
# line 911 "main.y"
      {
      					fprintf(salidaAS," < ");
      				} break;
      case 117:
# line 916 "main.y"
      {
      					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JL;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 118:
# line 957 "main.y"
      {
      					++aux;
      				} break;
      case 119:
# line 962 "main.y"
      {
      					fprintf(salidaAS," <= ");
      				} break;
      case 120:
# line 967 "main.y"
      {
      					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JLE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 121:
# line 1008 "main.y"
      {
      					++aux;
      				} break;
      case 122:
# line 1013 "main.y"
      {
      					fprintf(salidaAS," == ");
      				} break;
      case 123:
# line 1018 "main.y"
      {
      					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 124:
# line 1059 "main.y"
      {
      					++aux;
      				} break;
      case 125:
# line 1064 "main.y"
      {
      					fprintf(salidaAS," != ");
      				} break;
      case 126:
# line 1069 "main.y"
      {
      					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JNE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					if(registroBHUsado == TRUE)
      					{
      						tercetoAux.tipoDeY = BL;
      					}
      					else
      					{
      						tercetoAux.tipoDeY = BH;
      					}
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 127:
# line 1111 "main.y"
      {
      				++aux;
      			} break;
      case 128:
# line 1116 "main.y"
      {
      				fprintf(salidaAS," + ");
      			} break;
      case 129:
# line 1121 "main.y"
      {
      				yyval = yypvt[-4] + yypvt[-0];
      				printf("2 EXPRESION -> EXPRESION + TERMINO\n");
      
				tercetoAux.x = OP_SUMA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 130:
# line 1136 "main.y"
      {
      				++aux;
      			} break;
      case 131:
# line 1141 "main.y"
      {
      				fprintf(salidaAS," - ");
      			} break;
      case 132:
# line 1146 "main.y"
      {
      				yyval = yypvt[-4] - yypvt[-0];
      
				tercetoAux.x = OP_RESTA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 133:
# line 1160 "main.y"
      {
      				++aux;
      				printf("3 EXPRESION -> TERMINO\n");
      			} break;
      case 134:
# line 1167 "main.y"
      {
      				++aux;
      			} break;
      case 135:
# line 1172 "main.y"
      {
      				fprintf(salidaAS," * ");
      			} break;
      case 136:
# line 1177 "main.y"
      {
      				yyval = yypvt[-4] * yypvt[-0];
      				printf("4 TERMINO -> TERMINO * FACTOR\n");
      
				tercetoAux.x = OP_MULTIPLICACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 137:
# line 1192 "main.y"
      {
      				++aux;
      			} break;
      case 138:
# line 1197 "main.y"
      {
      				fprintf(salidaAS," / ");
      			} break;
      case 139:
# line 1202 "main.y"
      {
      				yyval = yypvt[-4] / yypvt[-0];
      
				tercetoAux.x = OP_DIVISION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 140:
# line 1216 "main.y"
      {
      				yyval = yypvt[-0];
      				printf("5 TERMINO -> FACTOR\n");
      			} break;
      case 141:
# line 1223 "main.y"
      {
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      			printf("6 FACTOR -> ID (%s)\n",TS[yypvt[-0]].nombre);
      
			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 142:
# line 1236 "main.y"
      {
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			yyval = atoi(TS[yypvt[-0]].valor);
      			printf("7 FACTOR -> CTE (%d)\n",yyval);
      
			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 143:
# line 1250 "main.y"
      {	
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			yyval = atoi(TS[yypvt[-0]].valor);
      
			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 144:
# line 1263 "main.y"
      {
      			fprintf(salidaAS,"(");
      		} break;
      case 145:
# line 1268 "main.y"
      {
      			++aux;
      		} break;
      case 146:
# line 1273 "main.y"
      {
      			fprintf(salidaAS,")");
      			yyval = yypvt[-2];
      			printf("8 FACTOR -> ( EXPRESION )\n");
      
			tercetoAux.x = popInt(&pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 147:
# line 1287 "main.y"
      {
      			++aux;
      		} break;
      case 148:
# line 1293 "main.y"
      {
      				fprintf(salidaAS,"FILTERC");
      			} break;
      case 149:
# line 1298 "main.y"
      {
      				fprintf(salidaAS,"(");
      			} break;
      case 150:
# line 1303 "main.y"
      {
      				++aux;
      			} break;
      case 151:
# line 1308 "main.y"
      {
      				fprintf(salidaAS,",");
      			} break;
      case 152:
# line 1313 "main.y"
      {
      				fprintf(salidaAS,"[");
      			} break;
      case 153:
# line 1318 "main.y"
      {
      				++aux;
      			} break;
      case 154:
# line 1323 "main.y"
      {
      				fprintf(salidaAS,"]");
      			} break;
      case 155:
# line 1328 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 156:
# line 1334 "main.y"
      {
      					++aux;
      				} break;
      case 157:
# line 1339 "main.y"
      {
      					++aux;
      				} break;
      case 158:
# line 1344 "main.y"
      {
      					fprintf(salidaAS," AND ");
      				} break;
      case 159:
# line 1349 "main.y"
      {
      					++aux;
      				} break;
      case 160:
# line 1354 "main.y"
      {
      					++aux;
      				} break;
      case 161:
# line 1359 "main.y"
      {
      					fprintf(salidaAS," OR ");
      				} break;
      case 162:
# line 1364 "main.y"
      {
      					++aux;
      				} break;
      case 163:
# line 1369 "main.y"
      {
      					fprintf(salidaAS,"NOT");
      				} break;
      case 164:
# line 1374 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 165:
# line 1379 "main.y"
      {
      					++aux;
      				} break;
      case 166:
# line 1384 "main.y"
      {
      					fprintf(salidaAS,")");
      				} break;
      case 167:
# line 1390 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 168:
# line 1395 "main.y"
      {
      					fprintf(salidaAS," > ");
      				} break;
      case 169:
# line 1400 "main.y"
      {
      					++aux;
      				} break;
      case 170:
# line 1405 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 171:
# line 1410 "main.y"
      {
      					fprintf(salidaAS," >= ");
      				} break;
      case 172:
# line 1415 "main.y"
      {
      					++aux;
      				} break;
      case 173:
# line 1420 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 174:
# line 1425 "main.y"
      {
      					fprintf(salidaAS," < ");
      				} break;
      case 175:
# line 1430 "main.y"
      {
      					++aux;
      				} break;
      case 176:
# line 1435 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 177:
# line 1440 "main.y"
      {
      					fprintf(salidaAS," <= ");
      				} break;
      case 178:
# line 1445 "main.y"
      {
      					++aux;
      				} break;
      case 179:
# line 1450 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 180:
# line 1455 "main.y"
      {
      					fprintf(salidaAS," == ");
      				} break;
      case 181:
# line 1460 "main.y"
      {
      					++aux;
      				} break;
      case 182:
# line 1465 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 183:
# line 1470 "main.y"
      {
      					fprintf(salidaAS," != ");
      				} break;
      case 184:
# line 1475 "main.y"
      {
      					++aux;
      				} break;
      case 185:
# line 1481 "main.y"
      {
      						++aux;
      					} break;
      case 186:
# line 1486 "main.y"
      {
      						fprintf(salidaAS,",");
      					} break;
      case 187:
# line 1491 "main.y"
      {
      						++aux;
      					} break;
      case 188:
# line 1496 "main.y"
      {
      						++aux;
      					} break;    }
    goto enstack;
}
