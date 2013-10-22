
# line 2 "main.y"
#include "AnalizadorLexico.h"
#include "Pila.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *archivo;
extern FILE *archivoDeTokens;

/*Tabla de símbolos*/
extern tablaDeSimbolos TS[LONG_TS];

/*Cantidad de tokens en la tabla de símbolos*/
extern int cantTokensEnTS;

/*Cantidad de IDs en la tabla de símbolos*/
int cantIDsEnTS = 0;

/*Variables para controlar los IDs declarados*/
int cantIDsEnDeclaracion = 0;
int posicionIDEnTS[50];
char ambitoActual[20];

/*Variables para controlar los tipos de los IDs declarados*/
int cantTiposEnDeclaracion = 0;
int tipoDeID[50];

extern int lineaActual;

/*Pila de desplazamiento para ajustar los números de tercetos*/
PilaDeInt pilaDesplazamientos;

/*Pila utilizada para controlar el anidamiento en expresiones*/
PilaDeInt pilaExpresionesNormal;
PilaDeInt pilaExpresionesFilterc;
PilaDeInt *pilaExpresiones = &pilaExpresionesNormal;

/*Pila utilizada para almacenar los tercetos de salto (JMP)*/
PilaDeInt pilaCondiciones;

/*Puntero a cola utilizada para almacenar el incremento de un FOR*/
ColaDeTercetos *ptrColaIncremento;

/*Pila utilizada para almacenar colas de incrementos de FOR*/
PilaDeColas pilaColasIncrementos;

/*Cola utilizada para almacenar las expresiones de un filterc*/
ColaDeTercetos colasExpresionesFilterc[3];

/*Almacena los tercetos iniciales de cada expresion del filterc*/
int tercetosInicialesFilterc[3];

/*Variable utilizada para indicar el tipo de condicion en un filterc (Compuesta:AND/OR Simple:0)*/
int tipoCondicionFilterc;

/*Bandera para indicar que el registro CH está siendo usado*/
int registroCHUsado;

/*Terceto auxiliar de uso multiple*/
Terceto tercetoAux;

/*Variables auxiliares de uso multiple*/
int aux;
int auy;

/*Cantidad de tercetos almacenados en la lista de tercetos*/
extern int cantTercetos;

/*Lista de tercetos*/
extern Terceto listaDeTercetos[MAX_TERCETOS];

/*Bandera para indicar que el registro BH está siendo usado*/
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
#define PR_FUNCTION 298
#define PR_MAIN 299
#define PR_BEGINPROG 300
#define PR_ENDPROG 301
#define PR_RETURN 302
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
#define YYERRCODE 256

# line 1995 "main.y"



int main(int argc, char *argv[])
{
	int i;
	vaciarPilaDeInt(pilaExpresiones);
	vaciarPilaDeInt(&pilaCondiciones);
	vaciarPilaDeCola(&pilaColasIncrementos);
	vaciarPilaDeInt(&pilaDesplazamientos);

	if(argc != 2)
	{
		printf("Formato invalido\nSe espera: Compilador <archivo_fuente>");
		return 1;
	}

    fopen_s(&archivo,argv[1],"r");
	
    if(archivo == NULL)
    {
        printf("No se pudo abrir el archivo fuente");
    }
    else
    {
        inicializarAL(archivo);

		yyparse();
		
        fclose(archivo);
    }
	
    return 0;
}

int yyerror(char *mensaje)
{
	printf("\nERROR\nLINEA: %d\nDESCRIPCION: %s\n",lineaActual,mensaje);
	
	fclose(archivo);
	fclose(archivoDeTokens);
	
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
	printf("\nERROR\nLINEA: %d\nDESCRIPCION: %s\n",lineaActual,mensaje);
		
	fclose(archivo);
	fclose(archivoDeTokens);

	exit(1);
}

void verificarDeclaracion(int posicionEnTS)
{
	char mensaje[50]; 
	
	
/*Hacer la busqueda para de ambito de las funciones*/

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
  -1, 70,
  280, 62,
  -2, 58,
  -1, 72,
  280, 61,
  -2, 79,
  -1, 211,
  261, 104,
  -2, 107,
  0,
};

#define YYNPROD 108
#define YYLAST 423

int yyact[] = {
      31,      31,     167,     147,      78,      93,       9,      65,
      63,      45,       5,      11,     101,       8,      76,      77,
      35,      35,      33,      33,      99,      32,      32,      34,
      34,      78,      31,      11,       9,      43,      57,     151,
      78,     101,      81,      76,      77,     114,     141,     108,
      72,      70,      76,      77,      11,      11,      17,      11,
       5,      15,      90,      91,      92,      31,     112,      81,
     171,     172,      51,     150,      59,       2,      81,      31,
      38,      56,      11,     129,     130,      35,     151,      33,
      31,      11,      32,     138,      34,     105,      31,      35,
     209,      33,     222,      40,      32,      41,      34,     111,
      35,     116,      33,      97,      98,      32,      35,      34,
      33,      49,     126,      32,      50,      34,      95,      96,
      95,      96,     132,     133,     134,     135,     136,     137,
     174,     175,     176,     177,     178,     179,     124,     143,
     146,     144,     145,      82,      40,      66,      41,     122,
     123,      95,      96,      60,     199,     185,     184,     185,
     110,      88,      87,      88,     218,      47,      39,      13,
     217,     170,     213,     187,     164,     139,      37,     216,
     202,     198,     182,     128,     103,      62,      61,     173,
     131,     127,     125,      55,      53,      52,      14,     109,
      21,     166,     210,      25,     165,      73,      74,     219,
     149,      20,     107,      80,      58,       6,      10,      19,
     214,       7,      10,      18,     196,      46,       7,      75,
      16,     195,     194,     193,      18,     192,      80,      42,
     191,      44,       7,     201,     106,      26,       4,     148,
     102,      12,       3,      79,      71,      67,      85,      21,
      48,      30,      26,      69,     153,      54,     215,      68,
     197,      83,      17,      64,      43,     152,      89,     104,
      84,      29,      28,     142,      27,      78,      86,      24,
      53,      23,      22,      36,       3,      94,     100,       1,
      89,       0,       0,       0,       0,      46,       0,     209,
       0,       0,       0,     185,       0,       0,      67,      46,
       0,       0,       0,      58,     132,     117,     118,     113,
     128,     115,     121,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,     119,     120,      96,      73,       0,       0,       6,
      74,     140,       0,     113,     158,     159,     160,     161,
     162,     163,     154,       0,     155,     156,     157,      98,
      75,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,      46,      19,       0,     133,     134,
     135,     136,     137,     191,     218,       0,     130,     131,
     107,     183,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,     188,     189,     190,       0,
       0,       0,       0,     200,     165,     180,     181,     203,
     204,     205,     206,     207,     208,     172,     173,     149,
       0,     211,     166,     192,     193,     194,     195,     196,
     201,     168,       0,     109,       0,     212,       0,     169,
      25,     153,     106,     211,       0,      46,     221,       0,
     210,       0,       0,       0,     220,       0,      20,       0,
       0,       0,       0,     186,     147,     167,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,     142,
};

int yypact[] = {
    -238,   -1000,    -287,    -276,    -119,     -91,    -251,    -254,
    -187,    -109,   -1000,    -201,    -120,   -1000,    -183,    -187,
    -271,    -187,   -1000,    -292,    -187,    -121,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -185,    -221,
     -92,     -93,   -1000,     -94,    -229,    -205,    -132,   -1000,
    -100,    -101,    -293,    -187,    -294,   -1000,    -137,   -1000,
   -1000,   -1000,   -1000,    -225,    -142,    -239,    -187,   -1000,
   -1000,    -205,    -122,   -1000,    -245,   -1000,   -1000,   -1000,
    -296,   -1000,   -1000,    -187,   -1000,    -165,   -1000,   -1000,
    -221,    -180,    -260,   -1000,   -1000,   -1000,    -232,   -1000,
   -1000,   -1000,    -102,   -1000,    -212,    -253,    -124,    -176,
    -211,    -265,   -1000,   -1000,   -1000,   -1000,    -193,    -232,
    -232,    -232,    -232,    -138,    -140,   -1000,     -95,   -1000,
    -164,     -96,    -103,    -223,     -97,    -167,    -188,    -110,
   -1000,    -264,    -146,    -299,   -1000,    -180,    -180,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -233,   -1000,   -1000,
    -187,    -232,    -232,    -232,    -232,    -232,    -232,    -232,
    -232,    -232,    -111,    -245,    -300,    -146,   -1000,   -1000,
   -1000,   -1000,   -1000,    -146,    -116,    -234,     -98,    -161,
    -253,    -253,    -187,   -1000,   -1000,    -104,    -165,    -165,
    -165,    -165,    -165,    -165,    -245,    -126,   -1000,    -146,
   -1000,   -1000,    -112,    -194,    -194,    -194,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -105,   -1000,    -128,
   -1000,    -245,   -1000,   -1000,   -1000,   -1000,    -106,    -232,
    -232,    -232,    -232,    -232,    -232,    -182,   -1000,   -1000,
   -1000,    -232,   -1000,    -165,    -165,    -165,    -165,    -165,
    -165,    -239,    -114,    -165,   -1000,    -107,    -117,    -118,
   -1000,   -1000,    -187,    -232,    -202,   -1000,   -1000,
};

int yypgo[] = {
       0,     247,     181,     183,     185,     244,     179,     169,
     235,     243,     180,     172,     177,     168,     242,     241,
     239,     171,     205,     236,     234,     233,     231,     229,
     204,     224,     222,     221,     220,     217,     216,     214,
     213,     167,     212,     174,     178,     173,     191,     211,
     208,     207,     203,     170,     176,     200,     197,     195,
     194,     193,     188,     184,     175,
};

int yyr1[] = {
       0,       1,       1,       1,       1,       1,       4,       4,
       6,       6,       6,       6,       8,       8,       8,       8,
       2,       9,       9,      10,      10,      11,      11,       7,
       7,       7,       3,      12,      12,      13,      13,      13,
      13,       5,       5,      14,      14,      18,      18,      19,
      15,      15,      22,      23,      25,      26,      20,      27,
      28,      21,      16,      31,      29,      30,      32,      30,
      17,      17,      17,      17,      34,      35,      35,      24,
      24,      24,      24,      36,      36,      36,      36,      36,
      36,      33,      33,      33,      37,      37,      37,      38,
      38,      38,      38,      38,      38,      40,      42,      39,
      41,      41,      41,      41,      45,      44,      46,      44,
      47,      44,      48,      44,      49,      44,      50,      44,
      51,      52,      43,      43,
};

int yyr2[] = {
       2,       5,       6,       5,       4,       1,       2,       1,
       8,       6,       7,       7,       1,       1,       1,       1,
       3,       8,       7,       3,       1,       3,       1,       1,
       1,       1,       1,       3,       2,       1,       1,       1,
       1,       3,       2,       1,       1,       4,       4,       4,
       1,       1,       0,       0,       0,       0,      14,       0,
       0,       8,       2,       0,       6,       1,       0,       4,
       3,       3,       3,       3,       3,       1,       1,       1,
       3,       3,       4,       3,       3,       3,       3,       3,
       3,       3,       3,       1,       3,       3,       1,       1,
       1,       1,       3,       1,       1,       0,       0,      10,
       1,       3,       3,       4,       0,       4,       0,       4,
       0,       4,       0,       4,       0,       4,       0,       4,
       0,       0,       5,       1,
};

int yychk[] = {
   -1000,      -1,     299,      -5,     -18,     286,      -2,      -4,
     300,     293,      -6,     298,     -18,     262,     257,     300,
      -4,     300,      -6,      -3,     -12,     -13,     -14,     -15,
     -16,     -17,     -18,     -19,     -20,     -21,     -29,     265,
     286,     283,     288,     281,      -9,     259,     265,     262,
     266,     268,      -3,     300,      -3,     301,     -13,     262,
     -30,     282,     285,     279,     257,     257,     -27,     257,
     294,     259,     -10,     265,     263,     258,     258,     301,
      -3,     301,     262,     -32,     -17,     -33,     266,     -34,
     265,     -37,     -35,     -38,     267,     268,     257,     -39,
      -6,     287,     265,     -17,     -12,     -31,     -10,     260,
     261,      -7,     295,     296,     297,     301,     -12,     269,
     270,     271,     272,     280,     -33,     265,     -40,     258,
     -22,     289,     -24,     -36,     292,     -33,     260,     263,
     265,      -2,     302,      -3,     282,     -37,     -37,     -38,
     -38,     -35,     265,     266,     258,     257,     262,     257,
     258,     290,     291,     257,     273,     274,     275,     276,
     277,     278,     263,     259,      -3,     302,      -8,     265,
     267,     268,     266,     302,     -41,     -44,     292,     264,
     -23,     -28,     -12,     -36,     -36,     -36,     -33,     -33,
     -33,     -33,     -33,     -33,     259,     -11,      -7,     302,
      -8,      -8,     261,     290,     291,     257,     273,     274,
     275,     276,     277,     278,     -24,     -24,     258,     -11,
     260,     261,      -8,     259,     -44,     -44,     -44,     -45,
     -46,     -47,     -48,     -49,     -50,     -25,     258,     260,
      -7,     -42,     258,     -33,     -33,     -33,     -33,     -33,
     -33,     262,     -43,     -33,     -17,     260,     -51,     -26,
     258,     261,     258,     -52,     -12,     -43,     284,
};

int yydef[] = {
       0,      -2,       0,       5,       0,       0,       0,       0,
       0,       0,       7,       0,       0,      34,       0,       0,
       0,       0,       6,       0,      26,       0,      29,      30,
      31,      32,      35,      36,      40,      41,       0,       0,
       0,       0,      47,       0,       0,       0,       0,      33,
       0,       0,       0,       0,       0,       4,       0,      28,
      50,      53,      54,       0,       0,       0,       0,      51,
      16,       0,       0,      20,       0,      37,      38,       1,
       0,       3,      27,       0,      56,      57,      -2,      59,
      -2,      75,       0,      78,      80,      81,       0,      83,
      84,      85,       0,      42,       0,       0,       0,       0,
       0,       0,      23,      24,      25,       2,       0,       0,
       0,       0,       0,       0,       0,      79,       0,      39,
       0,       0,       0,      63,       0,       0,       0,       0,
      19,       0,       0,       0,      55,      73,      74,      76,
      77,      60,      61,      62,      82,       0,      43,      48,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       9,      12,
      13,      14,      15,       0,       0,      88,       0,       0,
       0,       0,      52,      64,      65,       0,      67,      68,
      69,      70,      71,      72,       0,       0,      22,       0,
      11,      10,       0,       0,       0,       0,      92,      94,
      96,      98,     100,     102,      44,       0,      66,       0,
      18,       0,       8,      86,      89,      90,       0,       0,
       0,       0,       0,       0,       0,       0,      49,      17,
      21,       0,      91,      93,      95,      97,      99,     101,
     103,       0,       0,      -2,      45,       0,       0,       0,
      87,     105,       0,       0,       0,     106,      46,
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
# line 138 "main.y"
      {
      		
      				imprimirTercetos();
      				GenerarAssembler();
      				
      			} break;
      case 2:
# line 146 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      				printf("holaHola");
      			} break;
      case 3:
# line 152 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 4:
# line 158 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 5:
# line 164 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 6:
# line 170 "main.y"
      {
      				//printf("hola3");
      			} break;
      case 7:
# line 174 "main.y"
      {
      				//printf("hola1");
      				} break;
      case 8:
# line 179 "main.y"
      {
      				printf("hola2");
      			} break;
      case 17:
# line 195 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 18:
# line 206 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 19:
# line 218 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 20:
# line 223 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 21:
# line 229 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 22:
# line 234 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 23:
# line 240 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 24:
# line 245 "main.y"
      {
      			yyval = PR_FLOAT;
      		} break;
      case 25:
# line 250 "main.y"
      {
      			yyval = PR_STRING;
      		} break;
      case 37:
# line 285 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 38:
# line 295 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 39:
# line 306 "main.y"
      {
      				verificarDeclaracion(yypvt[-1]);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 42:
# line 328 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 43:
# line 333 "main.y"
      {
      					registroBHUsado = FALSE;
      				} break;
      case 44:
# line 338 "main.y"
      {
      					pushInt(cantTercetos,&pilaDesplazamientos);
      				} break;
      case 45:
# line 345 "main.y"
      {
      					ptrColaIncremento = (ColaDeTercetos *)malloc(sizeof(ColaDeTercetos));
      
					vaciarColaDeTercetos(ptrColaIncremento);
      
					for(aux = VerTope(&pilaDesplazamientos); aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],ptrColaIncremento);
      					}
      
					pushCola(ptrColaIncremento,&pilaColasIncrementos);
      					cantTercetos = VerTope(&pilaDesplazamientos);
      				} break;
      case 46:
# line 364 "main.y"
      {
      					ptrColaIncremento = popCola(&pilaColasIncrementos);
      					aux = cantTercetos - popInt(&pilaDesplazamientos);
      					ajustarTercetos(ptrColaIncremento,aux);
      
					while(colaDeTercetosEstaVacia(ptrColaIncremento) == FALSE)
      					{
      						crearTerceto(desencolarTerceto(ptrColaIncremento));
      					}
      
					free(ptrColaIncremento);
      
					aux = popInt(&pilaCondiciones);
      					listaDeTercetos[aux].y = cantTercetos + 1;
      					listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					
      					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JMP;
      					tercetoAux.y = popInt(&pilaCondiciones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      				} break;
      case 47:
# line 389 "main.y"
      {
      						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 48:
# line 398 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 49:
# line 405 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 51:
# line 418 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 53:
# line 429 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 54:
# line 436 "main.y"
      {
      						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 55:
# line 448 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 56:
# line 456 "main.y"
      {
      				verificarDeclaracion(yypvt[-2]);
      
				if(TS[yypvt[-2]].tipo == PR_INT && TS[yypvt[-0]].tipo == PR_STRING)
      				{
      					printf("\n%d %d\n",TS[yypvt[-2]].tipo,TS[yypvt[-0]].tipo);
      					lanzarError("No se puede asignar un tipo STRING a un tipo INT");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_FLOAT && TS[yypvt[-0]].tipo == PR_STRING)
      				{
      					lanzarError("No se puede asignar un tipo STRING a un tipo FLOAT");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_STRING && TS[yypvt[-0]].tipo == PR_INT)
      				{
      					lanzarError("No se puede asignar un tipo INT a un tipo STRING");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_STRING && TS[yypvt[-0]].tipo == PR_FLOAT)
      				{
      					lanzarError("No se puede asignar un tipo FLOAT a un tipo STRING");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_INT && TS[yypvt[-0]].tipo == PR_FLOAT)
      				{
      					printf("\nADVERTENCIA\nLINEA: %d\nDESCRIPCION: Asignacion de tipo FLOAT a tipo INT con posible perdida de precision\n",lineaActual);
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = yypvt[-0];
      				tercetoAux.tipoDeZ = INDICE_TS; 
      				crearTerceto(&tercetoAux);
      
				yyval = yypvt[-2];
      			} break;
      case 57:
# line 498 "main.y"
      {
      				verificarDeclaracion(yypvt[-2]);
      
				if(TS[yypvt[-2]].tipo == PR_INT && yypvt[-0] == PR_STRING)
      				{
      					lanzarError("No se puede asignar un tipo STRING a un tipo INT");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_FLOAT && yypvt[-0] == PR_STRING)
      				{
      					lanzarError("No se puede asignar un tipo STRING a un tipo FLOAT");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_STRING && yypvt[-0] == PR_INT)
      				{
      					lanzarError("No se puede asignar un tipo INT a un tipo STRING");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_STRING && yypvt[-0] == PR_FLOAT)
      				{
      					lanzarError("No se puede asignar un tipo FLOAT a un tipo STRING");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_INT && yypvt[-0] == PR_FLOAT)
      				{
      					printf("\nADVERTENCIA\nLINEA: %d\nDESCRIPCION: Asignacion de tipo FLOAT a tipo INT con posible perdida de precision\n",lineaActual);
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO; 
      				crearTerceto(&tercetoAux);
      
				yyval = yypvt[-2];
      			} break;
      case 58:
# line 539 "main.y"
      {
      				verificarDeclaracion(yypvt[-2]);
      
				if(TS[yypvt[-2]].tipo == PR_INT)
      				{
      					lanzarError("No puede asignar un tipo STRING a un tipo INT");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_FLOAT)
      				{
      					lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = yypvt[-0];
      				tercetoAux.tipoDeZ = INDICE_TS;
      				crearTerceto(&tercetoAux);
      
				yyval = yypvt[-2];
      			} break;
      case 59:
# line 565 "main.y"
      {
      				verificarDeclaracion(yypvt[-2]);
      
				if(TS[yypvt[-2]].tipo == PR_INT)
      				{
      					lanzarError("No puede asignar un tipo STRING a un tipo INT");
      				}
      
				if(TS[yypvt[-2]].tipo == PR_FLOAT)
      				{
      					lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = cantTercetos - 1;
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				crearTerceto(&tercetoAux);
      
				yyval = yypvt[-2];
      			} break;
      case 60:
# line 591 "main.y"
      {
      					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_CONCATENACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.y = yypvt[-2];
      					tercetoAux.tipoDeY = INDICE_TS;
      					tercetoAux.z = yypvt[-0];
      					tercetoAux.tipoDeZ = INDICE_TS;
      					crearTerceto(&tercetoAux);
      				} break;
      case 61:
# line 603 "main.y"
      {
      									verificarDeclaracion(yypvt[-0]);
      
									if(TS[yypvt[-0]].tipo != PR_STRING)
      									{
      										lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      									}
      								} break;
      case 63:
# line 616 "main.y"
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
      case 64:
# line 638 "main.y"
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
      case 65:
# line 652 "main.y"
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
      case 66:
# line 666 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_NOT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.tipoDeY = BH;
      				crearTerceto(&tercetoAux);
      
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
      case 67:
# line 695 "main.y"
      {
      					if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroBHUsado == FALSE)
      					{
      						auy = BH;
      					}
      					else
      					{
      						auy = BL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MAYOR;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JG;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 68:
# line 745 "main.y"
      {
      					if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroBHUsado == FALSE)
      					{
      						auy = BH;
      					}
      					else
      					{
      						auy = BL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MAYOR_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JGE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 69:
# line 795 "main.y"
      {
      					if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroBHUsado == FALSE)
      					{
      						auy = BH;
      					}
      					else
      					{
      						auy = BL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MENOR;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JL;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 70:
# line 845 "main.y"
      {
      					if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroBHUsado == FALSE)
      					{
      						auy = BH;
      					}
      					else
      					{
      						auy = BL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MENOR_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JLE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 71:
# line 895 "main.y"
      {
      					if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroBHUsado == FALSE)
      					{
      						auy = BH;
      					}
      					else
      					{
      						auy = BL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 72:
# line 945 "main.y"
      {
      					if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroBHUsado == FALSE)
      					{
      						auy = BH;
      					}
      					else
      					{
      						auy = BL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_DISTINTO;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JNE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					registroBHUsado = TRUE;
      				} break;
      case 73:
# line 996 "main.y"
      {
      				if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      				{
      					lanzarError("No se puede usar un tipo STRING en una expresion");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_SUMA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
				if(yypvt[-2] == PR_FLOAT || yypvt[-0] == PR_FLOAT)
      				{
      					yyval = PR_FLOAT;
      				}
      				else
      				{
      					yyval = PR_INT;
      				}
      			} break;
      case 74:
# line 1023 "main.y"
      {
      				if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      				{
      					lanzarError("No se puede usar un tipo STRING en una expresion");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_RESTA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
				if(yypvt[-2] == PR_FLOAT || yypvt[-0] == PR_FLOAT)
      				{
      					yyval = PR_FLOAT;
      				}
      				else
      				{
      					yyval = PR_INT;
      				}
      			} break;
      case 75:
# line 1050 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 76:
# line 1056 "main.y"
      {
      				if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      				{
      					lanzarError("No se puede usar un tipo STRING en una expresion");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_MULTIPLICACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
				if(yypvt[-2] == PR_FLOAT || yypvt[-0] == PR_FLOAT)
      				{
      					yyval = PR_FLOAT;
      				}
      				else
      				{
      					yyval = PR_INT;
      				}
      			} break;
      case 77:
# line 1083 "main.y"
      {
      				if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      				{
      					lanzarError("No se puede usar un tipo STRING en una expresion");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_DIVISION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
				if(yypvt[-2] == PR_FLOAT || yypvt[-0] == PR_FLOAT)
      				{
      					yyval = PR_FLOAT;
      				}
      				else
      				{
      					yyval = PR_INT;
      				}
      			} break;
      case 78:
# line 1110 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 79:
# line 1116 "main.y"
      {
      			verificarDeclaracion(yypvt[-0]);
      
			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = TS[yypvt[-0]].tipo;
      		} break;
      case 80:
# line 1129 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_INT;
      		} break;
      case 81:
# line 1140 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_FLOAT;
      		} break;
      case 82:
# line 1151 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = popInt(pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = yypvt[-1];
      		} break;
      case 83:
# line 1162 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 85:
# line 1173 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.tipoDeX = VAR_FILTERC;
      				aux = crearTerceto(&tercetoAux);
      				pushInt(aux,pilaExpresiones);
      
				registroCHUsado = FALSE;
      
				vaciarPilaDeInt(&pilaExpresionesFilterc);
      				pilaExpresiones = &pilaExpresionesFilterc;
      				pushInt(aux,pilaExpresiones);
      				pushInt(aux,pilaExpresiones);
      			} break;
      case 86:
# line 1194 "main.y"
      {
      				tercetosInicialesFilterc[0] = cantTercetos;
      			} break;
      case 87:
# line 1203 "main.y"
      {
      				pilaExpresiones = &pilaExpresionesNormal;
      			} break;
      case 88:
# line 1209 "main.y"
      {
      					tipoCondicionFilterc = 0;
      				} break;
      case 89:
# line 1214 "main.y"
      {
      					tipoCondicionFilterc = PR_AND;
      				} break;
      case 90:
# line 1219 "main.y"
      {
      					tipoCondicionFilterc = PR_OR;
      				} break;
      case 91:
# line 1224 "main.y"
      {
      					tipoCondicionFilterc = PR_NOT;
      				} break;
      case 92:
# line 1232 "main.y"
      {
      					if(registroCHUsado == FALSE)
      					{
      						tercetosInicialesFilterc[1] = cantTercetos;
      					}
      					else
      					{
      						tercetosInicialesFilterc[2] = cantTercetos;
      					}
      				} break;
      case 93:
# line 1244 "main.y"
      {
      					if(yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroCHUsado == FALSE)
      					{
      						auy = CH;
      					}
      					else
      					{
      						auy = CL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MAYOR;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JG;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					if(registroCHUsado == FALSE)
      					{
      						auy = 1;
      					}
      					else
      					{
      						auy = 2;
      					}
      
					vaciarColaDeTercetos(&colasExpresionesFilterc[auy]);
      
					for(aux = tercetosInicialesFilterc[auy]; aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[auy]);
      					}
      
					cantTercetos = tercetosInicialesFilterc[auy];
      
					registroCHUsado = TRUE;
      				} break;
      case 94:
# line 1314 "main.y"
      {
      					if(registroCHUsado == FALSE)
      					{
      						tercetosInicialesFilterc[1] = cantTercetos;
      					}
      					else
      					{
      						tercetosInicialesFilterc[2] = cantTercetos;
      					}
      				} break;
      case 95:
# line 1326 "main.y"
      {
      					if(yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroCHUsado == FALSE)
      					{
      						auy = CH;
      					}
      					else
      					{
      						auy = CL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MAYOR_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JGE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					if(registroCHUsado == FALSE)
      					{
      						auy = 1;
      					}
      					else
      					{
      						auy = 2;
      					}
      
					vaciarColaDeTercetos(&colasExpresionesFilterc[auy]);
      
					for(aux = tercetosInicialesFilterc[auy]; aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[auy]);
      					}
      
					cantTercetos = tercetosInicialesFilterc[auy];
      
					registroCHUsado = TRUE;
      				} break;
      case 96:
# line 1396 "main.y"
      {
      					if(registroCHUsado == FALSE)
      					{
      						tercetosInicialesFilterc[1] = cantTercetos;
      					}
      					else
      					{
      						tercetosInicialesFilterc[2] = cantTercetos;
      					}
      				} break;
      case 97:
# line 1408 "main.y"
      {
      					if(yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroCHUsado == FALSE)
      					{
      						auy = CH;
      					}
      					else
      					{
      						auy = CL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MENOR;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JL;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					if(registroCHUsado == FALSE)
      					{
      						auy = 1;
      					}
      					else
      					{
      						auy = 2;
      					}
      
					vaciarColaDeTercetos(&colasExpresionesFilterc[auy]);
      
					for(aux = tercetosInicialesFilterc[auy]; aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[auy]);
      					}
      
					cantTercetos = tercetosInicialesFilterc[auy];
      
					registroCHUsado = TRUE;
      				} break;
      case 98:
# line 1478 "main.y"
      {
      					if(registroCHUsado == FALSE)
      					{
      						tercetosInicialesFilterc[1] = cantTercetos;
      					}
      					else
      					{
      						tercetosInicialesFilterc[2] = cantTercetos;
      					}
      				} break;
      case 99:
# line 1490 "main.y"
      {
      					if(yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroCHUsado == FALSE)
      					{
      						auy = CH;
      					}
      					else
      					{
      						auy = CL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_MENOR_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JLE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					if(registroCHUsado == FALSE)
      					{
      						auy = 1;
      					}
      					else
      					{
      						auy = 2;
      					}
      
					vaciarColaDeTercetos(&colasExpresionesFilterc[auy]);
      
					for(aux = tercetosInicialesFilterc[auy]; aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[auy]);
      					}
      
					cantTercetos = tercetosInicialesFilterc[auy];
      
					registroCHUsado = TRUE;
      				} break;
      case 100:
# line 1560 "main.y"
      {
      					if(registroCHUsado == FALSE)
      					{
      						tercetosInicialesFilterc[1] = cantTercetos;
      					}
      					else
      					{
      						tercetosInicialesFilterc[2] = cantTercetos;
      					}
      				} break;
      case 101:
# line 1572 "main.y"
      {
      					if(yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroCHUsado == FALSE)
      					{
      						auy = CH;
      					}
      					else
      					{
      						auy = CL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					if(registroCHUsado == FALSE)
      					{
      						auy = 1;
      					}
      					else
      					{
      						auy = 2;
      					}
      
					vaciarColaDeTercetos(&colasExpresionesFilterc[auy]);
      
					for(aux = tercetosInicialesFilterc[auy]; aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[auy]);
      					}
      
					cantTercetos = tercetosInicialesFilterc[auy];
      
					registroCHUsado = TRUE;
      				} break;
      case 102:
# line 1642 "main.y"
      {
      					if(registroCHUsado == FALSE)
      					{
      						tercetosInicialesFilterc[1] = cantTercetos;
      					}
      					else
      					{
      						tercetosInicialesFilterc[2] = cantTercetos;
      					}
      				} break;
      case 103:
# line 1654 "main.y"
      {
      					if(yypvt[-0] == PR_STRING)
      					{
      						lanzarError("No se puede usar un tipo STRING en una comparacion");
      					}
      
					if(registroCHUsado == FALSE)
      					{
      						auy = CH;
      					}
      					else
      					{
      						auy = CL;
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 1;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_DISTINTO;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.z = popInt(pilaExpresiones);
      					tercetoAux.tipoDeZ = NRO_TERCETO;
      					tercetoAux.y = popInt(pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = JNE;
      					tercetoAux.y = cantTercetos + 2;
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					crearTerceto(&tercetoAux);
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = OP_ASIGNACION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.tipoDeY = auy;
      					tercetoAux.z = 0;
      					tercetoAux.tipoDeZ = VALOR;
      					crearTerceto(&tercetoAux);
      
					if(registroCHUsado == FALSE)
      					{
      						auy = 1;
      					}
      					else
      					{
      						auy = 2;
      					}
      
					vaciarColaDeTercetos(&colasExpresionesFilterc[auy]);
      
					for(aux = tercetosInicialesFilterc[auy]; aux < cantTercetos; ++aux)
      					{
      						encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[auy]);
      					}
      
					cantTercetos = tercetosInicialesFilterc[auy];
      
					registroCHUsado = TRUE;
      				} break;
      case 104:
# line 1723 "main.y"
      {
      						if(yypvt[-0] == PR_STRING)
      						{
      							lanzarError("No se puede usar un tipo STRING en una comparacion");
      						}
      
						vaciarColaDeTercetos(&colasExpresionesFilterc[0]);
      					
      						for(aux = tercetosInicialesFilterc[0]; aux < cantTercetos; ++aux)
      						{
      							encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[0]);
      						}
      
						auy = cantTercetos - tercetosInicialesFilterc[1];
      
						ajustarTercetos(&colasExpresionesFilterc[1],auy);
      						 
      						for(aux = 0; aux < colasExpresionesFilterc[1].cantTercetos; ++aux)
      						{
      							crearTerceto(&colasExpresionesFilterc[1].contenedor[aux]);
      						}
      
						ajustarTercetos(&colasExpresionesFilterc[1],-auy);
      
						if(tipoCondicionFilterc == 0 || tipoCondicionFilterc == PR_NOT)
      						{
      							if(tipoCondicionFilterc == 0)
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = OP_ASIGNACION;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CL;
      								tercetoAux.z = 1;
      								tercetoAux.tipoDeZ = VALOR;
      								crearTerceto(&tercetoAux);
      
								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_AND;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      							else
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_NOT;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								crearTerceto(&tercetoAux);
      
								borrarTerceto(&tercetoAux);
      								tercetoAux.x = OP_ASIGNACION;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CL;
      								tercetoAux.z = 1;
      								tercetoAux.tipoDeZ = VALOR;
      								crearTerceto(&tercetoAux);
      
								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_AND;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      						}
      						else
      						{
      							auy = cantTercetos - tercetosInicialesFilterc[0];
      
							ajustarTercetos(&colasExpresionesFilterc[0],auy);
      
							while(colaDeTercetosEstaVacia(&colasExpresionesFilterc[0]) == FALSE)
      							{
      								crearTerceto(desencolarTerceto(&colasExpresionesFilterc[0]));
      							}
      
							auy = cantTercetos - tercetosInicialesFilterc[2];
      
							ajustarTercetos(&colasExpresionesFilterc[2],auy);
      						 
      							for(aux = 0; aux < colasExpresionesFilterc[2].cantTercetos; ++aux)
      							{
      								crearTerceto(&colasExpresionesFilterc[2].contenedor[aux]);
      							}
      
							ajustarTercetos(&colasExpresionesFilterc[2],-auy);
      
							if(tipoCondicionFilterc == PR_AND)
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_AND;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      							else
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_OR;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      						}
      
						borrarTerceto(&tercetoAux);
      						tercetoAux.tipoDeX = JZ;
      						tercetoAux.y = cantTercetos + 3;
      						tercetoAux.tipoDeY = NRO_TERCETO;
      						crearTerceto(&tercetoAux);
      							
      						borrarTerceto(&tercetoAux);
      						tercetoAux.x = OP_SUMA;
      						tercetoAux.tipoDeX = TOKEN;
      						tercetoAux.tipoDeY = VAR_FILTERC;
      						tercetoAux.z = 1,
      						tercetoAux.tipoDeZ = VALOR;
      						crearTerceto(&tercetoAux);
      
						borrarTerceto(&tercetoAux);
      						tercetoAux.x = OP_ASIGNACION;
      						tercetoAux.tipoDeX = TOKEN,
      						tercetoAux.tipoDeY = VAR_FILTERC;
      						tercetoAux.z = 1;
      						tercetoAux.tipoDeZ = VALOR;
      						crearTerceto(&tercetoAux);
      					} break;
      case 105:
# line 1856 "main.y"
      {
      						tercetosInicialesFilterc[0] = cantTercetos;
      					} break;
      case 107:
# line 1863 "main.y"
      {
      						if(yypvt[-0] == PR_STRING)
      						{
      							lanzarError("No se puede usar un tipo STRING en una comparacion");
      						}
      
						vaciarColaDeTercetos(&colasExpresionesFilterc[0]);
      					
      						for(aux = tercetosInicialesFilterc[0]; aux < cantTercetos; ++aux)
      						{
      							encolarTerceto(&listaDeTercetos[aux],&colasExpresionesFilterc[0]);
      						}
      
						auy = cantTercetos - tercetosInicialesFilterc[1];
      
						ajustarTercetos(&colasExpresionesFilterc[1],auy);
      						 
      						for(aux = 0; aux < colasExpresionesFilterc[1].cantTercetos; ++aux)
      						{
      							crearTerceto(&colasExpresionesFilterc[1].contenedor[aux]);
      						}
      
						ajustarTercetos(&colasExpresionesFilterc[1],-auy);
      
						if(tipoCondicionFilterc == 0 || tipoCondicionFilterc == PR_NOT)
      						{
      							if(tipoCondicionFilterc == 0)
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = OP_ASIGNACION;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CL;
      								tercetoAux.z = 1;
      								tercetoAux.tipoDeZ = VALOR;
      								crearTerceto(&tercetoAux);
      
								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_AND;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      							else
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_NOT;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								crearTerceto(&tercetoAux);
      
								borrarTerceto(&tercetoAux);
      								tercetoAux.x = OP_ASIGNACION;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CL;
      								tercetoAux.z = 1;
      								tercetoAux.tipoDeZ = VALOR;
      								crearTerceto(&tercetoAux);
      
								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_AND;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      						}
      						else
      						{
      							auy = cantTercetos - tercetosInicialesFilterc[0];
      
							ajustarTercetos(&colasExpresionesFilterc[0],auy);
      
							while(colaDeTercetosEstaVacia(&colasExpresionesFilterc[0]) == FALSE)
      							{
      								crearTerceto(desencolarTerceto(&colasExpresionesFilterc[0]));
      							}
      
							auy = cantTercetos - tercetosInicialesFilterc[2];
      
							ajustarTercetos(&colasExpresionesFilterc[2],auy);
      						 
      							for(aux = 0; aux < colasExpresionesFilterc[2].cantTercetos; ++aux)
      							{
      								crearTerceto(&colasExpresionesFilterc[2].contenedor[aux]);
      							}
      
							ajustarTercetos(&colasExpresionesFilterc[2],-auy);
      
							if(tipoCondicionFilterc == PR_AND)
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_AND;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      							else
      							{
      								borrarTerceto(&tercetoAux);
      								tercetoAux.x = PR_OR;
      								tercetoAux.tipoDeX = TOKEN;
      								tercetoAux.tipoDeY = CH;
      								tercetoAux.tipoDeZ = CL;
      								crearTerceto(&tercetoAux);
      							}
      						}
      
						borrarTerceto(&tercetoAux);
      						tercetoAux.tipoDeX = JZ;
      						tercetoAux.y = cantTercetos + 3;
      						tercetoAux.tipoDeY = NRO_TERCETO;
      						crearTerceto(&tercetoAux);
      							
      						borrarTerceto(&tercetoAux);
      						tercetoAux.x = OP_SUMA;
      						tercetoAux.tipoDeX = TOKEN;
      						tercetoAux.tipoDeY = VAR_FILTERC;
      						tercetoAux.z = 1,
      						tercetoAux.tipoDeZ = VALOR;
      						crearTerceto(&tercetoAux);
      
						borrarTerceto(&tercetoAux);
      						tercetoAux.x = OP_ASIGNACION;
      						tercetoAux.tipoDeX = TOKEN,
      						tercetoAux.tipoDeY = VAR_FILTERC;
      						tercetoAux.z = 1;
      						tercetoAux.tipoDeZ = VALOR;
      						crearTerceto(&tercetoAux);
      					} break;    }
    goto enstack;
}
