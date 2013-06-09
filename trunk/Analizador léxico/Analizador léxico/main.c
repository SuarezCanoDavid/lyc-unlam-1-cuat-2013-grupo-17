
# line 2 "main.y"
#include "AnalizadorLexico.h"
#include <stdio.h>
#include <string.h>

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

# line 1087 "main.y"



int main(int argc, char *argv[])
{
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
  -1, 34,
  294, 5,
  -2, 7,
  -1, 45,
  269, 131,
  270, 134,
  -2, 94,
  -1, 46,
  280, 101,
  -2, 95,
  -1, 48,
  279, 92,
  280, 100,
  -2, 145,
  -1, 49,
  271, 138,
  272, 141,
  -2, 137,
  -1, 82,
  289, 74,
  -2, 34,
  -1, 89,
  261, 22,
  -2, 25,
  -1, 99,
  269, 131,
  270, 134,
  -2, 149,
  -1, 110,
  282, 88,
  -2, 34,
  -1, 122,
  290, 103,
  291, 106,
  -2, 102,
  -1, 124,
  270, 134,
  273, 113,
  274, 116,
  275, 119,
  276, 122,
  277, 125,
  278, 128,
  -2, 131,
  -1, 129,
  271, 138,
  272, 141,
  -2, 133,
  -1, 130,
  271, 138,
  272, 141,
  -2, 136,
  -1, 138,
  290, 161,
  291, 164,
  -2, 160,
  -1, 140,
  274, 174,
  275, 177,
  276, 180,
  277, 183,
  278, 186,
  -2, 171,
  -1, 218,
  282, 85,
  285, 85,
  -2, 34,
  -1, 222,
  269, 131,
  270, 134,
  -2, 115,
  -1, 223,
  269, 131,
  270, 134,
  -2, 118,
  -1, 224,
  269, 131,
  270, 134,
  -2, 121,
  -1, 225,
  269, 131,
  270, 134,
  -2, 124,
  -1, 226,
  269, 131,
  270, 134,
  -2, 127,
  -1, 227,
  269, 131,
  270, 134,
  -2, 130,
  -1, 234,
  269, 131,
  270, 134,
  -2, 173,
  -1, 235,
  269, 131,
  270, 134,
  -2, 176,
  -1, 236,
  269, 131,
  270, 134,
  -2, 179,
  -1, 237,
  269, 131,
  270, 134,
  -2, 182,
  -1, 238,
  269, 131,
  270, 134,
  -2, 185,
  -1, 239,
  269, 131,
  270, 134,
  -2, 188,
  -1, 252,
  261, 26,
  -2, 29,
  -1, 256,
  261, 189,
  -2, 157,
  -1, 257,
  269, 131,
  270, 134,
  -2, 192,
  -1, 280,
  269, 131,
  270, 134,
  -2, 191,
  -1, 282,
  284, 71,
  -2, 34,
  0,
};

#define YYNPROD 193
#define YYLAST 349

int yyact[] = {
     253,     254,     255,      54,      86,       7,     140,      54,
      32,     184,     183,     100,       4,      52,      53,      48,
      46,      52,      53,     171,     124,     170,     120,       7,
      31,     284,      29,      71,      98,       7,      72,      30,
      54,      56,     139,      16,     156,      56,     123,      84,
     100,     191,      52,      53,     178,     190,     177,     189,
      45,     176,      97,     188,     175,     251,     187,     174,
     186,     173,      96,      95,      94,      38,      56,     134,
     135,      37,      78,      32,      89,      80,     140,     228,
     180,     247,     167,     109,      85,      69,      36,      21,
     270,     268,     182,     127,     277,     269,     267,     179,
     126,     250,     230,     229,      87,      35,     279,      82,
     276,     258,     249,      99,     248,     169,     136,     118,
      91,      90,     185,      43,     172,     168,     101,      83,
      81,      39,     138,     122,     121,      17,      93,     275,
     198,      28,       6,     103,     264,     215,      22,       5,
      28,     110,     166,       6,       9,      50,     214,     165,
      33,     213,      51,     164,     212,     163,     211,      88,
     162,     210,     194,     161,     246,      49,     209,     160,
     208,     159,     207,     158,     274,     263,      68,     256,
      22,     245,     206,     157,     137,     117,      79,      55,
     116,      78,     114,      76,     113,      75,     112,      74,
     111,      73,     203,     152,     202,     151,     201,     119,
     150,     200,      17,     149,     199,     148,     198,     147,
     242,     197,     146,     196,     145,     195,     144,     259,
     115,      77,      47,      61,      44,      27,     128,      93,
     194,     143,     105,      60,      70,      42,      26,     241,
     193,     142,     104,     222,     223,     224,     225,     226,
     227,      59,     283,     281,     271,     259,     218,     234,
     235,     236,     237,     238,     239,     240,     192,     141,
     103,      58,      25,     199,     200,     201,     202,     203,
     210,     133,     125,      24,     131,     132,     281,     211,
     212,     213,     214,     215,     245,     129,     130,      14,
     102,      57,     257,      50,      67,     155,     114,      51,
      13,      66,      12,      23,      11,      62,     275,     112,
      49,       9,      20,      19,      18,      41,      92,      40,
     273,     262,     252,     155,     108,     261,     244,     205,
     280,     154,     107,     181,      65,     124,     272,     260,
     243,     204,     153,     106,      64,     216,     217,     219,
     220,     221,     266,      63,      34,     282,      16,      10,
     273,       3,     231,     232,     233,      88,      15,     278,
       8,       2,       1,       0,     251,     193,     121,     196,
     197,     122,       0,     265,       0,       0,       0,       0,
       0,      21,     208,     209,     138,
};

int yypact[] = {
    -281,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -257,    -263,    -166,    -184,    -201,    -207,    -144,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -250,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -257,    -185,    -255,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -196,    -145,    -257,    -146,    -240,    -186,    -290,
    -167,    -197,    -153,    -154,   -1000,   -1000,   -1000,   -1000,
   -1000,    -209,    -211,    -213,    -222,    -252,    -225,    -147,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,    -187,    -257,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -155,    -198,
    -267,    -254,    -197,    -172,    -178,   -1000,   -1000,    -225,
    -225,    -225,    -225,    -202,    -156,    -258,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -246,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -188,    -148,    -157,
    -269,    -272,    -149,    -216,    -219,    -223,    -227,    -231,
    -234,    -173,    -191,    -197,   -1000,    -179,    -280,    -282,
    -151,    -217,    -220,    -224,    -229,    -232,    -237,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -254,    -254,    -257,    -225,    -225,    -225,    -225,    -225,
    -225,    -225,    -225,    -225,    -192,    -168,    -169,    -194,
    -194,    -194,    -225,    -225,    -225,    -225,    -225,    -225,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -189,    -158,    -160,    -170,    -295,    -225,    -161,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -198,    -295,    -174,    -180,    -175,
    -181,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -162,
    -176,    -295,    -164,    -225,   -1000,   -1000,   -1000,   -1000,
   -1000,    -257,   -1000,    -259,   -1000,
};

int yypgo[] = {
       0,     330,     329,     328,     326,     321,     319,     316,
     315,     308,     307,     143,     306,     305,     304,     303,
      53,     302,     300,     298,     297,     295,     294,     293,
     292,     291,     290,     289,     288,      35,     287,     117,
     286,     285,     284,     283,     282,      79,     281,     126,
     277,     276,     275,     121,     274,     273,     272,     268,
     265,     264,     263,     251,     242,     241,     240,     239,
     238,     116,     237,     229,     228,     227,     226,     225,
     218,     217,     216,     215,     214,     213,     212,     211,
     210,     209,     208,     207,     206,     205,     204,     203,
      20,     202,     133,     201,     200,     115,     198,     197,
     196,     195,     194,     193,     192,     191,     190,     189,
     188,     187,     185,     184,     182,     181,     180,     179,
     178,     177,     176,     149,     175,     174,     173,     172,
     138,     171,     170,     169,     168,     167,     166,     165,
     164,     163,     162,     161,     159,     157,     156,     114,
     155,     154,     153,     152,     151,     150,     148,     147,
     145,     144,     142,     141,     140,     139,     137,     135,
     134,     130,     125,     124,     119,
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
      54,      55,      56,      58,      59,      60,      61,      62,
      51,      63,      64,      65,      66,      67,      52,      69,
      36,      71,      72,      73,      74,      68,      70,      75,
      76,      70,      37,      37,      79,      77,      78,      78,
      78,      83,      84,      81,      82,      82,      57,      86,
      87,      57,      88,      89,      57,      90,      91,      92,
      57,      93,      94,      85,      95,      96,      85,      97,
      98,      85,      99,     100,      85,     101,     102,      85,
     103,     104,      85,     105,     106,      80,     108,     109,
      80,      80,     110,     111,     107,     113,     114,     107,
     107,     112,     112,     112,     115,     116,     112,     112,
     118,     119,     121,     122,     123,     125,     126,     117,
     120,     128,     129,     120,     130,     131,     120,     132,
     133,     134,     120,     135,     136,     127,     137,     138,
     127,     139,     140,     127,     141,     142,     127,     143,
     144,     127,     145,     146,     127,     147,     148,     124,
     124,
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
       0,       0,       0,       0,       0,       0,       0,       0,
      19,       0,       0,       0,       0,       0,      11,       0,
       3,       0,       0,       0,       0,       9,       1,       0,
       0,       5,       2,       2,       0,       3,       1,       1,
       1,       0,       0,       5,       1,       1,       1,       0,
       0,       5,       0,       0,       5,       0,       0,       0,
       7,       0,       0,       5,       0,       0,       5,       0,
       0,       5,       0,       0,       5,       0,       0,       5,
       0,       0,       5,       0,       0,       5,       0,       0,
       5,       1,       0,       0,       5,       0,       0,       5,
       1,       1,       1,       1,       0,       0,       5,       1,
       0,       0,       0,       0,       0,       0,       0,      15,
       1,       0,       0,       5,       0,       0,       5,       0,
       0,       0,       7,       0,       0,       5,       0,       0,
       5,       0,       0,       5,       0,       0,       5,       0,
       0,       5,       0,       0,       5,       0,       0,       5,
       1,
};

int yychk[] = {
   -1000,      -1,      -2,      -5,     293,     -39,     -43,     286,
      -3,     -38,      -6,     -41,     -44,     -46,     -50,      -4,
     -29,     -31,     -34,     -35,     -36,     -37,     -39,     -42,
     -51,     -52,     -68,     -77,     -43,     283,     288,     281,
     265,     -39,      -7,     259,     262,     266,     268,     257,
     -30,     -33,     -69,     -37,     -78,     -80,     266,     -81,
     265,    -107,     -82,    -112,     267,     268,     257,    -117,
     287,     -48,     -53,     -63,     -71,     -79,     -40,      -8,
      -9,     -18,     -45,     -47,     -31,     262,     -70,     282,
     285,    -105,    -108,    -110,    -113,     -83,    -115,    -118,
     265,     257,     -29,     257,     279,     262,     294,     259,
     -11,     265,     258,     258,     -32,     -75,     269,     270,
     271,     272,     280,     -80,     265,     257,     -49,     -54,
     -64,     -72,     -10,     -19,     -24,     262,     -29,    -106,
    -109,    -111,    -114,     -84,    -116,    -119,     258,     -37,
     289,     -57,     -85,     292,     -80,     -11,     260,     261,
     -76,    -107,    -107,    -112,    -112,     -82,     265,     266,
     258,    -120,    -127,     292,     264,     -55,     -65,     -73,
     -86,     -88,     -90,     -93,     -95,     -97,     -99,    -101,
    -103,     -12,     -20,     -25,     282,    -121,    -128,    -130,
    -132,    -135,    -137,    -139,    -141,    -143,    -145,     262,
     257,     258,     290,     291,     257,     273,     274,     275,
     276,     277,     278,     260,     263,     -11,     261,     290,
     291,     257,     273,     274,     275,     276,     277,     278,
     -56,     -66,     -74,     -87,     -89,     -91,     -94,     -96,
     -98,    -100,    -102,    -104,     -13,     -21,    -122,    -129,
    -131,    -133,    -136,    -138,    -140,    -142,    -144,    -146,
     -57,     -57,     -29,     -85,     -85,     -85,     -80,     -80,
     -80,     -80,     -80,     -80,     263,     259,     259,    -127,
    -127,    -127,     -80,     -80,     -80,     -80,     -80,     -80,
     -58,     -67,     -92,     -14,     -22,    -123,    -134,     262,
     258,     258,     259,     -16,     -26,     295,     296,     297,
    -124,     -80,     258,     -59,     -15,     -23,     -27,    -125,
    -147,     -37,     -16,     260,     261,     260,     261,     -60,
     -17,     -28,    -126,    -148,     258,     260,     -16,     258,
     -80,     -61,     -29,     -62,     284,
};

int yydef[] = {
       0,      -2,       1,      -2,       4,      46,      -2,      59,
       0,       0,       0,       0,       0,       0,       0,       2,
      -2,      37,      39,      40,      41,      42,      48,      49,
      61,      62,      79,       0,      -2,      63,      73,      81,
      92,      44,      -2,      15,      47,      51,      54,      60,
       0,       0,       0,      90,      91,      -2,      -2,      96,
      -2,      -2,      97,     144,     146,     147,     148,     151,
     152,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,      35,      38,      80,      86,
      87,       0,       0,       0,       0,       0,       0,       0,
      57,      64,      -2,      82,      93,      45,       6,       8,
      16,      -2,      52,      55,       0,       0,     132,     135,
     139,     142,      98,      -2,     145,     153,       0,       0,
       0,       0,       0,       0,       0,      36,      -2,       0,
       0,       0,       0,       0,       0,       0,      58,      65,
      75,      83,      -2,     109,      -2,       9,      17,      23,
       0,      -2,      -2,     140,     143,      99,     100,     101,
     150,     154,      -2,     167,      -2,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,      89,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,      66,
      76,      84,     104,     107,     110,     114,     117,     120,
     123,     126,     129,      10,      18,      24,     155,     162,
     165,     168,     172,     175,     178,     181,     184,     187,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
      67,      77,      -2,     105,     108,     111,      -2,      -2,
      -2,      -2,      -2,      -2,      11,      19,     156,     163,
     166,     169,      -2,      -2,      -2,      -2,      -2,      -2,
       0,       0,       0,       0,       0,       0,       0,      68,
      78,     112,      12,      20,      -2,      30,      31,      32,
      -2,      -2,     170,       0,       0,       0,       0,       0,
       0,      69,      13,      21,      27,     158,     190,       0,
       0,       0,       0,       0,      70,      14,      28,     159,
      -2,       0,      -2,       0,      72,
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
# line 78 "main.y"
      {
      				++aux;
      			} break;
      case 2:
# line 83 "main.y"
      {
      				++aux;
      			} break;
      case 3:
# line 88 "main.y"
      {
      				++aux;
      			} break;
      case 4:
# line 94 "main.y"
      {
      						fprintf(salidaAS,"VAR\n");
      					} break;
      case 5:
# line 99 "main.y"
      {
      						++aux;
      					} break;
      case 6:
# line 104 "main.y"
      {
      						fprintf(salidaAS,"ENDVAR\n");
      					} break;
      case 7:
# line 110 "main.y"
      {
      					++aux;
      				} break;
      case 8:
# line 115 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 9:
# line 120 "main.y"
      {
      					++aux;
      				} break;
      case 10:
# line 125 "main.y"
      {
      					fprintf(salidaAS,"]");
      				} break;
      case 11:
# line 130 "main.y"
      {
      					fprintf(salidaAS,":");
      				} break;
      case 12:
# line 135 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 13:
# line 140 "main.y"
      {
      					++aux;
      				} break;
      case 14:
# line 145 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 15:
# line 158 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 16:
# line 163 "main.y"
      {
      					++aux;
      				} break;
      case 17:
# line 168 "main.y"
      {
      					fprintf(salidaAS,"]");
      				} break;
      case 18:
# line 173 "main.y"
      {
      					fprintf(salidaAS,":");
      				} break;
      case 19:
# line 178 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 20:
# line 183 "main.y"
      {
      					++aux;
      				} break;
      case 21:
# line 188 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 22:
# line 202 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      	
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 23:
# line 209 "main.y"
      {
      						fprintf(salidaAS,",");
      					} break;
      case 24:
# line 214 "main.y"
      {
      						++aux;
      					} break;
      case 25:
# line 219 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      	
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 26:
# line 227 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 27:
# line 232 "main.y"
      {
      					fprintf(salidaAS,",");
      				} break;
      case 28:
# line 237 "main.y"
      {
      					++aux;
      				} break;
      case 29:
# line 242 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 30:
# line 248 "main.y"
      {
      			fprintf(salidaAS,"int");
      	
      			yyval = PR_INT;
      		} break;
      case 31:
# line 255 "main.y"
      {
      			fprintf(salidaAS,"float");
      	
      			yyval = PR_FLOAT;
      		} break;
      case 32:
# line 262 "main.y"
      {
      			fprintf(salidaAS,"string");
      	
      			yyval = PR_STRING;
      		} break;
      case 33:
# line 270 "main.y"
      {
      						++aux;
      					} break;
      case 34:
# line 276 "main.y"
      {
      						++aux;
      					} break;
      case 35:
# line 281 "main.y"
      {
      						++aux;
      					} break;
      case 36:
# line 286 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 37:
# line 291 "main.y"
      {
      						++aux;
      					} break;
      case 38:
# line 296 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 39:
# line 302 "main.y"
      {
      				++aux;
      			} break;
      case 40:
# line 307 "main.y"
      {
      				++aux;
      			} break;
      case 41:
# line 312 "main.y"
      {
      				++aux;
      			} break;
      case 42:
# line 317 "main.y"
      {
      				++aux;
      			} break;
      case 43:
# line 323 "main.y"
      {
      						++aux;
      					} break;
      case 44:
# line 328 "main.y"
      {
      						++aux;
      					} break;
      case 45:
# line 333 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 46:
# line 338 "main.y"
      {
      						++aux;
      					} break;
      case 47:
# line 343 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 48:
# line 349 "main.y"
      {
      			++aux;
      		} break;
      case 49:
# line 354 "main.y"
      {
      			++aux;
      		} break;
      case 50:
# line 360 "main.y"
      {
      				++aux;
      			} break;
      case 51:
# line 365 "main.y"
      {
      				fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      			} break;
      case 52:
# line 370 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 53:
# line 375 "main.y"
      {
      				++aux;
      			} break;
      case 54:
# line 380 "main.y"
      {
      				fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			} break;
      case 55:
# line 385 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 56:
# line 391 "main.y"
      {
      				++aux;
      			} break;
      case 57:
# line 396 "main.y"
      {
      				fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      			} break;
      case 58:
# line 401 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 59:
# line 406 "main.y"
      {
      					fprintf(salidaAS,"WPRINT");
      				} break;
      case 60:
# line 411 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 61:
# line 417 "main.y"
      {
      				++aux;
      			} break;
      case 62:
# line 422 "main.y"
      {
      				++aux;
      			} break;
      case 63:
# line 428 "main.y"
      {
      					fprintf(salidaAS,"FOR");
      				} break;
      case 64:
# line 433 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 65:
# line 438 "main.y"
      {
      					++aux;
      				} break;
      case 66:
# line 443 "main.y"
      {
      					fprintf(salidaAS,";");
      				} break;
      case 67:
# line 448 "main.y"
      {
      					++aux;
      				} break;
      case 68:
# line 453 "main.y"
      {
      					fprintf(salidaAS,";");
      				} break;
      case 69:
# line 458 "main.y"
      {
      					++aux;
      				} break;
      case 70:
# line 463 "main.y"
      {
      					fprintf(salidaAS,")\n");
      				} break;
      case 71:
# line 468 "main.y"
      {
      					++aux;
      				} break;
      case 72:
# line 473 "main.y"
      {
      					fprintf(salidaAS,"ROF");
      				} break;
      case 73:
# line 479 "main.y"
      {
      						fprintf(salidaAS,"DO\n");
      					} break;
      case 74:
# line 484 "main.y"
      {
      						++aux;
      					} break;
      case 75:
# line 489 "main.y"
      {
      						fprintf(salidaAS,"WHILE");
      					} break;
      case 76:
# line 494 "main.y"
      {
      						fprintf(salidaAS,"(");
      					} break;
      case 77:
# line 499 "main.y"
      {
      						++aux;
      					} break;
      case 78:
# line 504 "main.y"
      {
      						fprintf(salidaAS,")");
      					} break;
      case 79:
# line 510 "main.y"
      {
      				++aux;
      			} break;
      case 80:
# line 515 "main.y"
      {
      				++aux;
      			} break;
      case 81:
# line 520 "main.y"
      {
      						fprintf(salidaAS,"IF");
      					} break;
      case 82:
# line 525 "main.y"
      {
      						fprintf(salidaAS,"(");
      					} break;
      case 83:
# line 530 "main.y"
      {
      						++aux;
      					} break;
      case 84:
# line 535 "main.y"
      {
      						fprintf(salidaAS,")\n");
      					} break;
      case 85:
# line 540 "main.y"
      {
      						++aux;
      					} break;
      case 86:
# line 545 "main.y"
      {
      						fprintf(salidaAS,"FI");
      					} break;
      case 87:
# line 550 "main.y"
      {
      						fprintf(salidaAS,"ELSE\n");
      					} break;
      case 88:
# line 555 "main.y"
      {
      						++aux;
      					} break;
      case 89:
# line 560 "main.y"
      {
      						fprintf(salidaAS,"FI");
      					} break;
      case 92:
# line 570 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      					} break;
      case 93:
# line 575 "main.y"
      {
      						fprintf(salidaAS," = ");
      					} break;
      case 94:
# line 580 "main.y"
      {
      						fprintf(salidaAS,"(=%d)",yypvt[-0]);
      					} break;
      case 95:
# line 585 "main.y"
      {
      						fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      					} break;
      case 96:
# line 590 "main.y"
      {
      						++aux;
      					} break;
      case 97:
# line 596 "main.y"
      {
      					++aux;
      				} break;
      case 98:
# line 601 "main.y"
      {
      					fprintf(salidaAS," ++ "); 
      				} break;
      case 99:
# line 606 "main.y"
      {
      					++aux;
      				} break;
      case 100:
# line 611 "main.y"
      {
      									fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      								} break;
      case 101:
# line 616 "main.y"
      {
      									fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      								} break;
      case 102:
# line 622 "main.y"
      {
      				++aux;
      			} break;
      case 103:
# line 627 "main.y"
      {
      				++aux;
      			} break;
      case 104:
# line 632 "main.y"
      {
      				fprintf(salidaAS," AND ");
      			} break;
      case 105:
# line 637 "main.y"
      {
      				++aux;
      			} break;
      case 106:
# line 642 "main.y"
      {
      				++aux;
      			} break;
      case 107:
# line 647 "main.y"
      {
      				fprintf(salidaAS," OR ");
      			} break;
      case 108:
# line 652 "main.y"
      {
      				++aux;
      			} break;
      case 109:
# line 657 "main.y"
      {
      				fprintf(salidaAS," NOT ");
      			} break;
      case 110:
# line 662 "main.y"
      {
      				fprintf(salidaAS,"(");
      			} break;
      case 111:
# line 667 "main.y"
      {
      				++aux;
      			} break;
      case 112:
# line 672 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 113:
# line 678 "main.y"
      {
      					++aux;
      				} break;
      case 114:
# line 683 "main.y"
      {
      					fprintf(salidaAS," > ");
      				} break;
      case 115:
# line 688 "main.y"
      {
      					++aux;
      				} break;
      case 116:
# line 693 "main.y"
      {
      					++aux;
      				} break;
      case 117:
# line 698 "main.y"
      {
      					fprintf(salidaAS," >= ");
      				} break;
      case 118:
# line 703 "main.y"
      {
      					++aux;
      				} break;
      case 119:
# line 708 "main.y"
      {
      					++aux;
      				} break;
      case 120:
# line 713 "main.y"
      {
      					fprintf(salidaAS," < ");
      				} break;
      case 121:
# line 718 "main.y"
      {
      					++aux;
      				} break;
      case 122:
# line 723 "main.y"
      {
      					++aux;
      				} break;
      case 123:
# line 728 "main.y"
      {
      					fprintf(salidaAS," <= ");
      				} break;
      case 124:
# line 733 "main.y"
      {
      					++aux;
      				} break;
      case 125:
# line 738 "main.y"
      {
      					++aux;
      				} break;
      case 126:
# line 743 "main.y"
      {
      					fprintf(salidaAS," == ");
      				} break;
      case 127:
# line 748 "main.y"
      {
      					++aux;
      				} break;
      case 128:
# line 753 "main.y"
      {
      					++aux;
      				} break;
      case 129:
# line 758 "main.y"
      {
      					fprintf(salidaAS," != ");
      				} break;
      case 130:
# line 763 "main.y"
      {
      					++aux;
      				} break;
      case 131:
# line 769 "main.y"
      {
      				++aux;
      			} break;
      case 132:
# line 774 "main.y"
      {
      				fprintf(salidaAS," + ");
      			} break;
      case 133:
# line 779 "main.y"
      {
      				yyval = yypvt[-4] + yypvt[-0];
      			} break;
      case 134:
# line 784 "main.y"
      {
      				++aux;
      			} break;
      case 135:
# line 789 "main.y"
      {
      				fprintf(salidaAS," - ");
      			} break;
      case 136:
# line 794 "main.y"
      {
      				yyval = yypvt[-4] - yypvt[-0];
      			} break;
      case 137:
# line 799 "main.y"
      {
      				++aux;
      			} break;
      case 138:
# line 805 "main.y"
      {
      				++aux;
      			} break;
      case 139:
# line 810 "main.y"
      {
      				fprintf(salidaAS," * ");
      			} break;
      case 140:
# line 815 "main.y"
      {
      				yyval = yypvt[-4] * yypvt[-0];
      			} break;
      case 141:
# line 820 "main.y"
      {
      				++aux;
      			} break;
      case 142:
# line 825 "main.y"
      {
      				fprintf(salidaAS," / ");
      			} break;
      case 143:
# line 830 "main.y"
      {
      				yyval = yypvt[-4] / yypvt[-0];
      			} break;
      case 144:
# line 835 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 145:
# line 841 "main.y"
      {
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      		} break;
      case 146:
# line 846 "main.y"
      {
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			yyval = atoi(TS[yypvt[-0]].valor);
      		} break;
      case 147:
# line 852 "main.y"
      {	
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			yyval = atoi(TS[yypvt[-0]].valor);
      		} break;
      case 148:
# line 858 "main.y"
      {
      			fprintf(salidaAS,"(");
      		} break;
      case 149:
# line 863 "main.y"
      {
      			++aux;
      		} break;
      case 150:
# line 868 "main.y"
      {
      			fprintf(salidaAS,")");
      			yyval = yypvt[-2];
      		} break;
      case 151:
# line 874 "main.y"
      {
      			++aux;
      		} break;
      case 152:
# line 880 "main.y"
      {
      				fprintf(salidaAS,"FILTERC");
      			} break;
      case 153:
# line 885 "main.y"
      {
      				fprintf(salidaAS,"(");
      			} break;
      case 154:
# line 890 "main.y"
      {
      				++aux;
      			} break;
      case 155:
# line 895 "main.y"
      {
      				fprintf(salidaAS,",");
      			} break;
      case 156:
# line 900 "main.y"
      {
      				fprintf(salidaAS,"[");
      			} break;
      case 157:
# line 905 "main.y"
      {
      				++aux;
      			} break;
      case 158:
# line 910 "main.y"
      {
      				fprintf(salidaAS,"]");
      			} break;
      case 159:
# line 915 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 160:
# line 921 "main.y"
      {
      					++aux;
      				} break;
      case 161:
# line 926 "main.y"
      {
      					++aux;
      				} break;
      case 162:
# line 931 "main.y"
      {
      					fprintf(salidaAS," AND ");
      				} break;
      case 163:
# line 936 "main.y"
      {
      					++aux;
      				} break;
      case 164:
# line 941 "main.y"
      {
      					++aux;
      				} break;
      case 165:
# line 946 "main.y"
      {
      					fprintf(salidaAS," OR ");
      				} break;
      case 166:
# line 951 "main.y"
      {
      					++aux;
      				} break;
      case 167:
# line 956 "main.y"
      {
      					fprintf(salidaAS,"NOT");
      				} break;
      case 168:
# line 961 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 169:
# line 966 "main.y"
      {
      					++aux;
      				} break;
      case 170:
# line 971 "main.y"
      {
      					fprintf(salidaAS,")");
      				} break;
      case 171:
# line 977 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 172:
# line 982 "main.y"
      {
      					fprintf(salidaAS," > ");
      				} break;
      case 173:
# line 987 "main.y"
      {
      					++aux;
      				} break;
      case 174:
# line 992 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 175:
# line 997 "main.y"
      {
      					fprintf(salidaAS," >= ");
      				} break;
      case 176:
# line 1002 "main.y"
      {
      					++aux;
      				} break;
      case 177:
# line 1007 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 178:
# line 1012 "main.y"
      {
      					fprintf(salidaAS," < ");
      				} break;
      case 179:
# line 1017 "main.y"
      {
      					++aux;
      				} break;
      case 180:
# line 1022 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 181:
# line 1027 "main.y"
      {
      					fprintf(salidaAS," <= ");
      				} break;
      case 182:
# line 1032 "main.y"
      {
      					++aux;
      				} break;
      case 183:
# line 1037 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 184:
# line 1042 "main.y"
      {
      					fprintf(salidaAS," == ");
      				} break;
      case 185:
# line 1047 "main.y"
      {
      					++aux;
      				} break;
      case 186:
# line 1052 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 187:
# line 1057 "main.y"
      {
      					fprintf(salidaAS," != ");
      				} break;
      case 188:
# line 1062 "main.y"
      {
      					++aux;
      				} break;
      case 189:
# line 1068 "main.y"
      {
      						++aux;
      					} break;
      case 190:
# line 1073 "main.y"
      {
      						fprintf(salidaAS,",");
      					} break;
      case 191:
# line 1078 "main.y"
      {
      						++aux;
      					} break;
      case 192:
# line 1083 "main.y"
      {
      						++aux;
      					} break;    }
    goto enstack;
}
