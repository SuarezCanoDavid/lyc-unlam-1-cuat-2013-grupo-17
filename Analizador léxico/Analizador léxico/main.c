
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

# line 1980 "main.y"



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
  -1, 68,
  280, 62,
  -2, 58,
  -1, 70,
  280, 61,
  -2, 79,
  -1, 212,
  261, 104,
  -2, 107,
  0,
};

#define YYNPROD 108
#define YYLAST 428

int yyact[] = {
      27,      27,     168,     148,      76,      94,      93,      63,
      42,      40,      39,      14,     102,      11,      74,      75,
      31,      31,      29,      29,       7,      28,      28,      30,
      30,      10,       2,       5,       7,      88,      89,      90,
      76,      55,      79,      10,      76,     115,     142,     109,
      70,      68,      74,      75,     102,      10,      74,      75,
       7,     172,     173,      27,     152,     130,     131,      46,
      11,     100,      47,      48,      27,      27,      79,      98,
      99,     113,      79,      31,      54,      29,      27,      57,
      28,      10,      30,     106,      31,      10,      29,     223,
     151,      28,      27,      30,     125,     152,      31,     117,
      29,      96,      97,      28,      37,      30,     210,      96,
      97,      59,      31,      60,      29,      80,      59,      28,
      60,      30,      96,      97,     139,     112,     133,     134,
     135,     136,     137,     138,     175,     176,     177,     178,
     179,     180,     144,     147,     145,     146,     123,     124,
      58,     200,     186,     185,     186,     214,     127,     111,
      86,      85,      86,     174,      64,      53,      44,      36,
     218,     171,     188,     165,     140,      34,     219,     217,
     203,     199,     183,     129,     104,      92,      91,     132,
     128,     126,      52,      50,      49,      38,     110,      17,
     167,     211,      21,     166,     220,      56,      73,     150,
      16,     108,       3,     215,     197,      12,      15,     196,
      43,      78,       8,     195,      72,       8,     194,      87,
     193,      71,      78,       4,     192,      41,     202,      13,
      22,       9,     149,     107,      35,     103,      77,      32,
      69,       8,       6,      17,      65,      83,      45,      67,
      26,     154,      51,      66,     216,      81,      61,      62,
      39,     198,     153,      87,      82,      84,     105,       4,
      25,      22,      76,      24,      23,      50,      20,     143,
      19,      18,      95,     101,      40,      87,      33,       6,
       1,       0,      43,       0,     210,       0,       0,       0,
     186,       0,      56,      65,       0,       0,       0,      43,
       0,       0,     114,       0,     133,       0,     116,       0,
     129,     120,     121,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,     122,     118,     119,       0,       3,       0,      99,
      73,     141,       0,     114,     159,     160,     161,     162,
     163,     164,     155,       0,     156,     157,     158,      72,
      97,      71,       0,       0,       0,       0,       0,       0,
       0,       0,       0,      43,      15,       0,     134,     135,
     136,     137,     138,     192,     219,       0,     131,     132,
     108,     184,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,     189,     190,     191,       0,
       0,       0,       0,     201,     166,     181,     182,     204,
     205,     206,     207,     208,     209,     173,     174,     150,
       0,     212,     167,     193,     194,     195,     196,     197,
     202,       0,       0,     110,       0,     213,     169,       0,
      21,     154,     107,     212,     170,      43,     222,       0,
     211,       0,       0,       0,     221,       0,      16,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
     187,     148,     168,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,     143,
};

int yypact[] = {
    -273,   -1000,    -245,    -263,    -289,    -183,    -230,    -110,
    -263,    -119,    -173,     -92,    -290,    -291,    -183,    -293,
    -183,    -120,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -227,    -220,     -93,     -94,   -1000,     -95,
    -121,    -226,    -194,   -1000,   -1000,    -135,    -169,    -183,
    -183,    -294,   -1000,    -122,   -1000,   -1000,   -1000,   -1000,
    -225,    -164,    -204,    -183,   -1000,   -1000,   -1000,    -194,
    -123,   -1000,    -266,    -100,    -101,    -295,    -296,   -1000,
   -1000,    -183,   -1000,    -180,   -1000,   -1000,    -220,    -208,
    -223,   -1000,   -1000,   -1000,    -221,   -1000,   -1000,   -1000,
    -102,   -1000,    -214,    -253,    -125,    -154,    -200,    -265,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -195,
    -221,    -221,    -221,    -221,    -139,    -174,   -1000,     -96,
   -1000,    -128,     -97,    -103,    -237,     -98,    -163,    -155,
    -111,   -1000,    -264,    -143,    -299,   -1000,    -208,    -208,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -212,   -1000,
   -1000,    -183,    -221,    -221,    -221,    -221,    -221,    -221,
    -221,    -221,    -221,    -112,    -266,    -300,    -143,   -1000,
   -1000,   -1000,   -1000,   -1000,    -143,    -116,    -241,    -118,
    -157,    -253,    -253,    -183,   -1000,   -1000,    -104,    -180,
    -180,    -180,    -180,    -180,    -180,    -266,    -129,   -1000,
    -143,   -1000,   -1000,    -113,    -179,    -179,    -179,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -105,   -1000,
    -131,   -1000,    -266,   -1000,   -1000,   -1000,   -1000,    -106,
    -221,    -221,    -221,    -221,    -221,    -221,    -168,   -1000,
   -1000,   -1000,    -221,   -1000,    -180,    -180,    -180,    -180,
    -180,    -180,    -204,    -127,    -180,   -1000,    -107,    -117,
    -108,   -1000,   -1000,    -183,    -221,    -205,   -1000,   -1000,
};

int yypgo[] = {
       0,     248,     178,     182,     195,     247,     185,     168,
     239,     246,     173,     171,     176,     167,     241,     240,
     238,     170,     200,     236,     235,     232,     230,     226,
     203,     225,     220,     218,     217,     216,     214,     213,
     212,     166,     208,     188,     177,     193,     174,     206,
     205,     202,     198,     169,     175,     196,     192,     190,
     187,     183,     180,     179,     172,
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
       2,       5,       5,       4,       3,       1,       2,       1,
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
   -1000,      -1,     299,      -2,      -4,     300,      -5,     293,
      -6,     -18,     298,     286,      -2,      -4,     300,      -3,
     -12,     -13,     -14,     -15,     -16,     -17,     -18,     -19,
     -20,     -21,     -29,     265,     286,     283,     288,     281,
     -18,      -9,     259,      -4,     262,     265,     257,     300,
     300,      -3,     301,     -13,     262,     -30,     282,     285,
     279,     257,     257,     -27,     257,     262,     294,     259,
     -10,     265,     263,     266,     268,      -3,      -3,     301,
     262,     -32,     -17,     -33,     266,     -34,     265,     -37,
     -35,     -38,     267,     268,     257,     -39,      -6,     287,
     265,     -17,     -12,     -31,     -10,     260,     261,      -7,
     295,     296,     297,     258,     258,     301,     301,     -12,
     269,     270,     271,     272,     280,     -33,     265,     -40,
     258,     -22,     289,     -24,     -36,     292,     -33,     260,
     263,     265,      -2,     302,      -3,     282,     -37,     -37,
     -38,     -38,     -35,     265,     266,     258,     257,     262,
     257,     258,     290,     291,     257,     273,     274,     275,
     276,     277,     278,     263,     259,      -3,     302,      -8,
     265,     267,     268,     266,     302,     -41,     -44,     292,
     264,     -23,     -28,     -12,     -36,     -36,     -36,     -33,
     -33,     -33,     -33,     -33,     -33,     259,     -11,      -7,
     302,      -8,      -8,     261,     290,     291,     257,     273,
     274,     275,     276,     277,     278,     -24,     -24,     258,
     -11,     260,     261,      -8,     259,     -44,     -44,     -44,
     -45,     -46,     -47,     -48,     -49,     -50,     -25,     258,
     260,      -7,     -42,     258,     -33,     -33,     -33,     -33,
     -33,     -33,     262,     -43,     -33,     -17,     260,     -51,
     -26,     258,     261,     258,     -52,     -12,     -43,     284,
};

int yydef[] = {
       0,      -2,       0,       0,       0,       0,       5,       0,
       7,       0,       0,       0,       0,       0,       0,       0,
      26,       0,      29,      30,      31,      32,      35,      36,
      40,      41,       0,       0,       0,       0,      47,       0,
       0,       0,       0,       6,      34,       0,       0,       0,
       0,       0,       4,       0,      28,      50,      53,      54,
       0,       0,       0,       0,      51,      33,      16,       0,
       0,      20,       0,       0,       0,       0,       0,       3,
      27,       0,      56,      57,      -2,      59,      -2,      75,
       0,      78,      80,      81,       0,      83,      84,      85,
       0,      42,       0,       0,       0,       0,       0,       0,
      23,      24,      25,      37,      38,       1,       2,       0,
       0,       0,       0,       0,       0,       0,      79,       0,
      39,       0,       0,       0,      63,       0,       0,       0,
       0,      19,       0,       0,       0,      55,      73,      74,
      76,      77,      60,      61,      62,      82,       0,      43,
      48,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       9,
      12,      13,      14,      15,       0,       0,      88,       0,
       0,       0,       0,      52,      64,      65,       0,      67,
      68,      69,      70,      71,      72,       0,       0,      22,
       0,      11,      10,       0,       0,       0,       0,      92,
      94,      96,      98,     100,     102,      44,       0,      66,
       0,      18,       0,       8,      86,      89,      90,       0,
       0,       0,       0,       0,       0,       0,       0,      49,
      17,      21,       0,      91,      93,      95,      97,      99,
     101,     103,       0,       0,      -2,      45,       0,       0,
       0,      87,     105,       0,       0,       0,     106,      46,
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
# line 137 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 2:
# line 143 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 3:
# line 148 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 4:
# line 154 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 5:
# line 160 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 17:
# line 180 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 18:
# line 191 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 19:
# line 203 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 20:
# line 208 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 21:
# line 214 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 22:
# line 219 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 23:
# line 225 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 24:
# line 230 "main.y"
      {
      			yyval = PR_FLOAT;
      		} break;
      case 25:
# line 235 "main.y"
      {
      			yyval = PR_STRING;
      		} break;
      case 37:
# line 270 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 38:
# line 280 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 39:
# line 291 "main.y"
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
# line 313 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 43:
# line 318 "main.y"
      {
      					registroBHUsado = FALSE;
      				} break;
      case 44:
# line 323 "main.y"
      {
      					pushInt(cantTercetos,&pilaDesplazamientos);
      				} break;
      case 45:
# line 330 "main.y"
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
# line 349 "main.y"
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
# line 374 "main.y"
      {
      						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 48:
# line 383 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 49:
# line 390 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 51:
# line 403 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 53:
# line 414 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 54:
# line 421 "main.y"
      {
      						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 55:
# line 433 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 56:
# line 441 "main.y"
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
# line 483 "main.y"
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
# line 524 "main.y"
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
# line 550 "main.y"
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
# line 576 "main.y"
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
# line 588 "main.y"
      {
      									verificarDeclaracion(yypvt[-0]);
      
									if(TS[yypvt[-0]].tipo != PR_STRING)
      									{
      										lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      									}
      								} break;
      case 63:
# line 601 "main.y"
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
# line 623 "main.y"
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
# line 637 "main.y"
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
# line 651 "main.y"
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
# line 680 "main.y"
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
# line 730 "main.y"
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
# line 780 "main.y"
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
# line 830 "main.y"
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
# line 880 "main.y"
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
# line 930 "main.y"
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
# line 981 "main.y"
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
# line 1008 "main.y"
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
# line 1035 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 76:
# line 1041 "main.y"
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
# line 1068 "main.y"
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
# line 1095 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 79:
# line 1101 "main.y"
      {
      			verificarDeclaracion(yypvt[-0]);
      
			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = TS[yypvt[-0]].tipo;
      		} break;
      case 80:
# line 1114 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_INT;
      		} break;
      case 81:
# line 1125 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_FLOAT;
      		} break;
      case 82:
# line 1136 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = popInt(pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = yypvt[-1];
      		} break;
      case 83:
# line 1147 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 85:
# line 1158 "main.y"
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
# line 1179 "main.y"
      {
      				tercetosInicialesFilterc[0] = cantTercetos;
      			} break;
      case 87:
# line 1188 "main.y"
      {
      				pilaExpresiones = &pilaExpresionesNormal;
      			} break;
      case 88:
# line 1194 "main.y"
      {
      					tipoCondicionFilterc = 0;
      				} break;
      case 89:
# line 1199 "main.y"
      {
      					tipoCondicionFilterc = PR_AND;
      				} break;
      case 90:
# line 1204 "main.y"
      {
      					tipoCondicionFilterc = PR_OR;
      				} break;
      case 91:
# line 1209 "main.y"
      {
      					tipoCondicionFilterc = PR_NOT;
      				} break;
      case 92:
# line 1217 "main.y"
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
# line 1229 "main.y"
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
# line 1299 "main.y"
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
# line 1311 "main.y"
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
# line 1381 "main.y"
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
# line 1393 "main.y"
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
# line 1463 "main.y"
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
# line 1475 "main.y"
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
# line 1545 "main.y"
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
# line 1557 "main.y"
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
# line 1627 "main.y"
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
# line 1639 "main.y"
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
# line 1708 "main.y"
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
# line 1841 "main.y"
      {
      						tercetosInicialesFilterc[0] = cantTercetos;
      					} break;
      case 107:
# line 1848 "main.y"
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
