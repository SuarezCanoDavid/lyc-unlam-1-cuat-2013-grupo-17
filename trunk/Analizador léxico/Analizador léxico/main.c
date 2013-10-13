
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
#define PR_MAIN 298
#define PR_FUNCTION 299
#define PR_BEGINPROG 300
#define PR_ENDPROG 301
#define PR_RETURN 302
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
#define YYERRCODE 256

# line 1973 "main.y"



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
  -1, 66,
  280, 61,
  -2, 57,
  -1, 68,
  280, 60,
  -2, 78,
  -1, 207,
  261, 103,
  -2, 106,
  0,
};

#define YYNPROD 107
#define YYLAST 415

int yyact[] = {
      28,      28,     163,     143,      74,      85,      59,      42,
      15,      12,      39,      15,      97,       8,      72,      73,
      32,      32,      30,      30,       7,      29,      29,      31,
      31,      74,      54,       5,       8,      87,      88,      89,
      74,      97,      77,      72,      73,     110,     137,     104,
      68,      66,      72,      73,       5,      46,      15,      28,
      47,     147,     167,     168,     125,     126,      95,      77,
       2,      28,      48,      93,      94,      53,      77,      32,
      28,      30,      28,      15,      29,     108,      31,     101,
      28,      32,      15,      30,     218,     146,      29,      36,
      31,      37,      32,     112,      30,      91,      92,      29,
      32,      31,      30,      78,      36,      29,      37,      31,
      91,      92,     134,      56,     128,     129,     130,     131,
     132,     133,     170,     171,     172,     173,     174,     175,
     120,     139,     142,     140,     141,     118,     119,     107,
      41,     147,      61,      91,      92,     205,     195,     181,
     180,     181,     209,     122,     106,      84,      83,      84,
     169,      62,      44,      35,      10,     213,     166,     183,
     160,     135,      34,     214,     212,     198,     194,     178,
     124,      99,      58,      57,     127,     123,     121,      52,
      50,      49,      11,     105,      18,     162,     206,      22,
     161,       6,      70,     215,     145,      17,     103,      16,
      69,      76,      55,      13,     210,     192,      43,     191,
      14,      23,       4,      71,      38,       9,       3,     190,
      76,     189,      40,      23,     188,     187,     197,     144,
     102,      98,      75,      67,      63,      81,      45,      27,
      18,     149,      51,     211,      65,     193,      39,      60,
      64,     148,      79,     100,      13,      26,      25,      24,
      21,      80,      20,      86,      19,      74,      33,     138,
      50,      82,       3,       1,      86,      90,      96,       0,
       0,       0,       0,       0,       0,      43,       0,     205,
       0,       0,       0,       0,     181,       0,      63,      43,
     109,       0,       0,       0,     128,      55,     111,       0,
     124,       0,     117,       0,     113,     114,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       6,
       0,     115,     116,       0,       0,     136,       0,     109,
      70,       0,      92,      69,     154,     155,     156,     157,
     158,     159,     150,       0,     151,     152,     153,      94,
      71,       0,       0,       0,       0,       0,       0,       0,
      16,       0,       0,      43,       0,       0,     129,     130,
     131,     132,     133,     187,     214,       0,     126,     127,
     103,     179,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,     184,     185,     186,       0,
       0,       0,       0,     196,     161,     176,     177,     199,
     200,     201,     202,     203,     204,     168,     169,     145,
       0,     207,     162,     188,     189,     190,     191,     192,
     197,     164,       0,     105,       0,     208,       0,     165,
      22,     149,     102,     207,       0,      43,     217,       0,
     206,       0,       0,       0,     216,       0,      17,       0,
       0,       0,       0,     182,     143,     163,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,     138,
};

int yypact[] = {
    -242,   -1000,    -280,    -259,    -122,     -95,    -291,    -193,
    -113,    -123,   -1000,    -187,    -193,    -290,    -288,    -145,
    -294,    -193,    -124,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -237,    -221,     -96,     -97,   -1000,
     -98,    -233,    -166,   -1000,    -103,    -104,    -295,    -193,
   -1000,    -141,   -1000,    -125,   -1000,   -1000,   -1000,   -1000,
    -225,    -174,    -201,    -193,   -1000,   -1000,    -166,    -126,
   -1000,   -1000,   -1000,   -1000,    -296,    -266,   -1000,    -193,
   -1000,    -184,   -1000,   -1000,    -221,    -212,    -226,   -1000,
   -1000,   -1000,    -232,   -1000,   -1000,   -1000,    -105,   -1000,
    -218,    -253,    -128,    -144,    -196,   -1000,    -265,   -1000,
   -1000,   -1000,    -199,    -232,    -232,    -232,    -232,    -148,
    -146,   -1000,     -99,   -1000,    -131,    -100,    -106,    -238,
    -101,    -173,    -165,    -114,   -1000,    -264,    -152,    -299,
   -1000,    -212,    -212,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -215,   -1000,   -1000,    -193,    -232,    -232,    -232,
    -232,    -232,    -232,    -232,    -232,    -232,    -115,    -266,
    -300,    -152,   -1000,   -1000,   -1000,   -1000,   -1000,    -152,
    -119,    -240,    -121,    -167,    -253,    -253,    -193,   -1000,
   -1000,    -107,    -184,    -184,    -184,    -184,    -184,    -184,
    -266,    -132,   -1000,    -152,   -1000,   -1000,    -116,    -143,
    -143,    -143,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -108,   -1000,    -134,   -1000,    -266,   -1000,   -1000,
   -1000,   -1000,    -109,    -232,    -232,    -232,    -232,    -232,
    -232,    -137,   -1000,   -1000,   -1000,    -232,   -1000,    -184,
    -184,    -184,    -184,    -184,    -184,    -201,    -130,    -184,
   -1000,    -110,    -120,    -111,   -1000,   -1000,    -193,    -232,
    -208,   -1000,   -1000,
};

int yypgo[] = {
       0,     235,     169,     175,     179,     234,     230,     178,
     168,     165,     177,     231,     173,     164,     228,     226,
     224,     167,     185,     223,     222,     221,     219,     217,
     200,     213,     211,     210,     209,     207,     206,     205,
     204,     163,     203,     170,     174,     176,     187,     202,
     201,     199,     198,     166,     172,     197,     196,     193,
     191,     183,     181,     180,     171,
};

int yyr1[] = {
       0,       1,       1,       1,       1,       2,       6,       6,
       7,       7,       8,       8,       9,       9,       9,       4,
       4,      10,      10,      10,      10,      11,      11,      11,
      11,       3,      12,      12,      13,      13,      13,      13,
       5,       5,      14,      14,      18,      18,      19,      15,
      15,      22,      23,      25,      26,      20,      27,      28,
      21,      16,      31,      29,      30,      32,      30,      17,
      17,      17,      17,      34,      35,      35,      24,      24,
      24,      24,      36,      36,      36,      36,      36,      36,
      33,      33,      33,      37,      37,      37,      38,      38,
      38,      38,      38,      38,      40,      42,      39,      41,
      41,      41,      41,      45,      44,      46,      44,      47,
      44,      48,      44,      49,      44,      50,      44,      51,
      52,      43,      43,
};

int yyr2[] = {
       2,       5,       6,       4,       1,       3,       8,       7,
       3,       1,       3,       1,       1,       1,       1,       2,
       1,       8,       6,       7,       7,       1,       1,       1,
       1,       1,       3,       2,       1,       1,       1,       1,
       3,       2,       1,       1,       4,       4,       4,       1,
       1,       0,       0,       0,       0,      14,       0,       0,
       8,       2,       0,       6,       1,       0,       4,       3,
       3,       3,       3,       3,       1,       1,       1,       3,
       3,       4,       3,       3,       3,       3,       3,       3,
       3,       3,       1,       3,       3,       1,       1,       1,
       1,       3,       1,       1,       0,       0,      10,       1,
       3,       3,       4,       0,       4,       0,       4,       0,
       4,       0,       4,       0,       4,       0,       4,       0,
       0,       5,       1,
};

int yychk[] = {
   -1000,      -1,     298,      -5,     -18,     286,      -2,     300,
     293,     -18,     262,     257,     300,      -4,     -10,     299,
      -3,     -12,     -13,     -14,     -15,     -16,     -17,     -18,
     -19,     -20,     -21,     -29,     265,     286,     283,     288,
     281,      -6,     259,     262,     266,     268,      -3,     300,
      -4,     265,     301,     -13,     262,     -30,     282,     285,
     279,     257,     257,     -27,     257,     294,     259,      -7,
     265,     258,     258,     301,      -3,     263,     262,     -32,
     -17,     -33,     266,     -34,     265,     -37,     -35,     -38,
     267,     268,     257,     -39,     -10,     287,     265,     -17,
     -12,     -31,      -7,     260,     261,     301,      -9,     295,
     296,     297,     -12,     269,     270,     271,     272,     280,
     -33,     265,     -40,     258,     -22,     289,     -24,     -36,
     292,     -33,     260,     263,     265,      -2,     302,      -3,
     282,     -37,     -37,     -38,     -38,     -35,     265,     266,
     258,     257,     262,     257,     258,     290,     291,     257,
     273,     274,     275,     276,     277,     278,     263,     259,
      -3,     302,     -11,     265,     267,     268,     266,     302,
     -41,     -44,     292,     264,     -23,     -28,     -12,     -36,
     -36,     -36,     -33,     -33,     -33,     -33,     -33,     -33,
     259,      -8,      -9,     302,     -11,     -11,     261,     290,
     291,     257,     273,     274,     275,     276,     277,     278,
     -24,     -24,     258,      -8,     260,     261,     -11,     259,
     -44,     -44,     -44,     -45,     -46,     -47,     -48,     -49,
     -50,     -25,     258,     260,      -9,     -42,     258,     -33,
     -33,     -33,     -33,     -33,     -33,     262,     -43,     -33,
     -17,     260,     -51,     -26,     258,     261,     258,     -52,
     -12,     -43,     284,
};

int yydef[] = {
       0,      -2,       0,       4,       0,       0,       0,       0,
       0,       0,      33,       0,       0,       0,      16,       0,
       0,      25,       0,      28,      29,      30,      31,      34,
      35,      39,      40,       0,       0,       0,       0,      46,
       0,       0,       0,      32,       0,       0,       0,       0,
      15,       0,       3,       0,      27,      49,      52,      53,
       0,       0,       0,       0,      50,       5,       0,       0,
       9,      36,      37,       1,       0,       0,      26,       0,
      55,      56,      -2,      58,      -2,      74,       0,      77,
      79,      80,       0,      82,      83,      84,       0,      41,
       0,       0,       0,       0,       0,       2,       0,      12,
      13,      14,       0,       0,       0,       0,       0,       0,
       0,      78,       0,      38,       0,       0,       0,      62,
       0,       0,       0,       0,       8,       0,       0,       0,
      54,      72,      73,      75,      76,      59,      60,      61,
      81,       0,      42,      47,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,      18,      21,      22,      23,      24,       0,
       0,      87,       0,       0,       0,       0,      51,      63,
      64,       0,      66,      67,      68,      69,      70,      71,
       0,       0,      11,       0,      20,      19,       0,       0,
       0,       0,      91,      93,      95,      97,      99,     101,
      43,       0,      65,       0,       7,       0,      17,      85,
      88,      89,       0,       0,       0,       0,       0,       0,
       0,       0,      48,       6,      10,       0,      90,      92,
      94,      96,      98,     100,     102,       0,       0,      -2,
      44,       0,       0,       0,      86,     104,       0,       0,
       0,     105,      45,
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
# line 135 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 2:
# line 141 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 3:
# line 147 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 4:
# line 153 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 6:
# line 164 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 7:
# line 175 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 8:
# line 187 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 9:
# line 192 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 10:
# line 198 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 11:
# line 203 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 12:
# line 209 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 13:
# line 214 "main.y"
      {
      			yyval = PR_FLOAT;
      		} break;
      case 14:
# line 219 "main.y"
      {
      			yyval = PR_STRING;
      		} break;
      case 36:
# line 263 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 37:
# line 273 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 38:
# line 284 "main.y"
      {
      				verificarDeclaracion(yypvt[-1]);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 41:
# line 306 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 42:
# line 311 "main.y"
      {
      					registroBHUsado = FALSE;
      				} break;
      case 43:
# line 316 "main.y"
      {
      					pushInt(cantTercetos,&pilaDesplazamientos);
      				} break;
      case 44:
# line 323 "main.y"
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
      case 45:
# line 342 "main.y"
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
      case 46:
# line 367 "main.y"
      {
      						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 47:
# line 376 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 48:
# line 383 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 50:
# line 396 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 52:
# line 407 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 53:
# line 414 "main.y"
      {
      						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 54:
# line 426 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 55:
# line 434 "main.y"
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
      case 56:
# line 476 "main.y"
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
      case 57:
# line 517 "main.y"
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
      case 58:
# line 543 "main.y"
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
      case 59:
# line 569 "main.y"
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
      case 60:
# line 581 "main.y"
      {
      									verificarDeclaracion(yypvt[-0]);
      
									if(TS[yypvt[-0]].tipo != PR_STRING)
      									{
      										lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      									}
      								} break;
      case 62:
# line 594 "main.y"
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
      case 63:
# line 616 "main.y"
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
      case 64:
# line 630 "main.y"
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
      case 65:
# line 644 "main.y"
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
      case 66:
# line 673 "main.y"
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
      case 67:
# line 723 "main.y"
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
      case 68:
# line 773 "main.y"
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
      case 69:
# line 823 "main.y"
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
      case 70:
# line 873 "main.y"
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
      case 71:
# line 923 "main.y"
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
      case 72:
# line 974 "main.y"
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
      case 73:
# line 1001 "main.y"
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
      case 74:
# line 1028 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 75:
# line 1034 "main.y"
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
      case 76:
# line 1061 "main.y"
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
      case 77:
# line 1088 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 78:
# line 1094 "main.y"
      {
      			verificarDeclaracion(yypvt[-0]);
      
			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = TS[yypvt[-0]].tipo;
      		} break;
      case 79:
# line 1107 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_INT;
      		} break;
      case 80:
# line 1118 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_FLOAT;
      		} break;
      case 81:
# line 1129 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = popInt(pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = yypvt[-1];
      		} break;
      case 82:
# line 1140 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 84:
# line 1151 "main.y"
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
      case 85:
# line 1172 "main.y"
      {
      				tercetosInicialesFilterc[0] = cantTercetos;
      			} break;
      case 86:
# line 1181 "main.y"
      {
      				pilaExpresiones = &pilaExpresionesNormal;
      			} break;
      case 87:
# line 1187 "main.y"
      {
      					tipoCondicionFilterc = 0;
      				} break;
      case 88:
# line 1192 "main.y"
      {
      					tipoCondicionFilterc = PR_AND;
      				} break;
      case 89:
# line 1197 "main.y"
      {
      					tipoCondicionFilterc = PR_OR;
      				} break;
      case 90:
# line 1202 "main.y"
      {
      					tipoCondicionFilterc = PR_NOT;
      				} break;
      case 91:
# line 1210 "main.y"
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
      case 92:
# line 1222 "main.y"
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
      case 93:
# line 1292 "main.y"
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
      case 94:
# line 1304 "main.y"
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
      case 95:
# line 1374 "main.y"
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
      case 96:
# line 1386 "main.y"
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
      case 97:
# line 1456 "main.y"
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
      case 98:
# line 1468 "main.y"
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
      case 99:
# line 1538 "main.y"
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
      case 100:
# line 1550 "main.y"
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
      case 101:
# line 1620 "main.y"
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
      case 102:
# line 1632 "main.y"
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
      case 103:
# line 1701 "main.y"
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
      case 104:
# line 1834 "main.y"
      {
      						tercetosInicialesFilterc[0] = cantTercetos;
      					} break;
      case 106:
# line 1841 "main.y"
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
