
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

/*Ambito actual*/
char ambitoActual[MAX_LONG_TOKEN+1] = "main";

int enDeclaracion = FALSE;
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

# line 2027 "main.y"



int main(int argc, char *argv[])
{
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

	if(TS[posicionEnTS].tipo == 0)
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
  -1, 80,
  280, 66,
  -2, 62,
  -1, 82,
  280, 65,
  -2, 83,
  -1, 210,
  261, 108,
  -2, 111,
  0,
};

#define YYNPROD 112
#define YYLAST 391

int yyact[] = {
      32,      88,      42,       9,     104,      75,      73,      50,
      12,     112,       8,      86,      87,      12,      54,      48,
      36,      55,      34,      88,      12,      33,      18,      35,
      88,     152,     110,     112,       9,      86,      87,      91,
      82,      80,      86,      87,     119,      42,      12,       5,
      16,      97,      12,     101,     102,     103,      32,     168,
     169,      91,     137,     138,       2,     151,      91,       5,
      32,      56,     108,     109,      12,      45,      36,      46,
      34,      12,      32,      33,      32,      35,     116,     148,
      36,      99,      34,     221,      96,      33,      32,      35,
     132,     152,      36,     124,      34,     106,     107,      33,
      43,      35,     165,     106,     107,     147,      36,     216,
      34,      92,      45,      33,      46,      35,     106,     107,
      70,     206,     140,     141,     142,     143,     144,     145,
     171,     172,     173,     174,     175,     176,      66,      69,
      67,      68,     130,     131,     207,     197,     196,     197,
     146,     123,     212,     134,     122,     123,     170,      76,
      52,      44,      14,     167,     183,     180,     166,      62,
     217,     215,     199,     194,     179,     136,     114,      72,
      71,     139,     135,     133,      60,      58,      57,      15,
     120,      22,     181,     209,      85,      26,      98,      84,
      39,     150,      21,     118,      83,       6,     218,       7,
     182,      20,     213,     192,     191,     190,      17,      51,
      90,      40,     189,      10,     188,      41,     117,      10,
      19,       7,      47,       7,      49,     187,     198,     149,
     113,      89,      19,      81,      90,      77,      95,      63,
      53,      31,     154,      59,     214,      22,       6,     193,
     153,      79,      64,      16,     115,      30,      78,      29,
      93,      18,      74,      40,      28,      25,      94,      24,
      39,      23,      61,      37,      65,      38,      11,       3,
      88,       1,      48,       0,      58,       0,       0,     100,
     105,     111,       0,       0,      20,       0,       0,       0,
      51,      27,       4,     206,       0,      13,       3,       0,
     121,      77,       0,      51,       0,       0,       0,      27,
     140,     127,     128,       0,     136,     197,     129,     125,
     126,       0,       0,       0,       0,       0,       0,       0,
      98,       0,       0,       0,     109,      85,       0,       0,
       0,      84,     107,      83,       0,     159,     160,     161,
     162,     163,     164,     155,       0,     156,     157,     158,
       0,       0,       0,       0,       0,      51,       0,       0,
     141,     142,     143,     144,     145,     187,     217,       0,
     138,     139,     118,       0,       0,       0,       0,       0,
       0,       0,     184,     185,     186,       0,       0,     195,
     177,     178,       0,       0,     200,     201,     202,     203,
     204,     205,     169,     170,     150,       0,     181,     210,
       0,     188,     189,     190,     191,     192,     198,       0,
     154,     117,       0,     120,     211,       0,     208,       0,
       0,      26,       0,     210,       0,      51,     220,       0,
     209,     182,       0,       0,     219,       0,      21,
};

int yypact[] = {
    -247,   -1000,    -290,    -231,    -124,     -98,    -260,    -278,
    -187,   -1000,   -1000,    -265,    -177,    -125,   -1000,    -205,
    -187,    -285,    -187,   -1000,    -294,    -187,    -126,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -268,
    -222,     -99,    -100,   -1000,    -101,    -116,    -300,   -1000,
    -187,   -1000,    -147,    -159,   -1000,    -106,    -107,    -295,
    -187,    -296,   -1000,    -127,   -1000,   -1000,   -1000,   -1000,
    -233,    -168,    -197,    -187,   -1000,    -218,    -192,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -252,   -1000,
   -1000,   -1000,    -297,   -1000,   -1000,    -187,   -1000,    -184,
   -1000,   -1000,    -222,    -213,    -254,   -1000,   -1000,   -1000,
    -238,   -1000,   -1000,   -1000,    -108,   -1000,    -219,    -256,
   -1000,    -192,    -128,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -199,    -238,    -238,    -238,    -238,    -143,    -178,
   -1000,    -102,   -1000,    -131,    -103,    -109,    -240,    -104,
    -167,    -132,    -170,    -194,   -1000,    -213,    -213,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -239,   -1000,   -1000,
    -187,    -238,    -238,    -238,    -238,    -238,    -238,    -238,
    -238,    -238,    -173,    -117,   -1000,    -122,    -243,    -123,
    -161,    -256,    -256,    -187,   -1000,   -1000,    -110,    -184,
    -184,    -184,    -184,    -184,    -184,    -118,    -252,    -119,
    -183,    -183,    -183,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -111,   -1000,    -252,    -134,   -1000,   -1000,
   -1000,   -1000,    -112,    -238,    -238,    -238,    -238,    -238,
    -238,    -157,   -1000,    -136,   -1000,    -252,    -238,   -1000,
    -184,    -184,    -184,    -184,    -184,    -184,    -197,   -1000,
   -1000,    -130,    -184,   -1000,    -113,    -166,    -114,   -1000,
   -1000,    -187,    -238,    -209,   -1000,   -1000,
};

int yypgo[] = {
       0,     241,     173,     177,     175,     239,     184,     238,
     237,     168,     176,     236,     235,     234,     166,     162,
     170,     161,     233,     231,     229,     165,     257,     228,
     223,     221,     220,     216,     190,     215,     212,     211,
     210,     209,     208,     206,     205,     160,     203,     167,
     171,     172,     164,     201,     200,     199,     198,     163,
     169,     197,     188,     186,     181,     180,     179,     178,
     174,
};

int yyr1[] = {
       0,       1,       1,       1,       1,       1,       4,       4,
       6,       6,       7,       8,       8,       8,       9,      11,
      11,      11,      11,      12,       2,      13,      13,      14,
      14,      15,      15,      10,      10,      10,       3,      16,
      16,      17,      17,      17,      17,       5,       5,      18,
      18,      22,      22,      23,      19,      19,      26,      27,
      29,      30,      24,      31,      32,      25,      20,      35,
      33,      34,      36,      34,      21,      21,      21,      21,
      38,      39,      39,      28,      28,      28,      28,      40,
      40,      40,      40,      40,      40,      37,      37,      37,
      41,      41,      41,      42,      42,      42,      42,      42,
      42,      44,      46,      43,      45,      45,      45,      45,
      49,      48,      50,      48,      51,      48,      52,      48,
      53,      48,      54,      48,      55,      56,      47,      47,
};

int yyr2[] = {
       2,       5,       6,       5,       4,       1,       2,       1,
       3,       2,       4,       2,       1,       1,       2,       1,
       1,       1,       1,       0,       4,       8,       7,       3,
       1,       3,       1,       1,       1,       1,       1,       3,
       2,       1,       1,       1,       1,       3,       2,       1,
       1,       4,       4,       4,       1,       1,       0,       0,
       0,       0,      14,       0,       0,       8,       2,       0,
       6,       1,       0,       4,       3,       3,       3,       3,
       3,       1,       1,       1,       3,       3,       4,       3,
       3,       3,       3,       3,       3,       3,       3,       1,
       3,       3,       1,       1,       1,       1,       3,       1,
       1,       0,       0,      10,       1,       3,       3,       4,
       0,       4,       0,       4,       0,       4,       0,       4,
       0,       4,       0,       4,       0,       0,       5,       1,
};

int yychk[] = {
   -1000,      -1,     299,      -5,     -22,     286,      -2,      -4,
     300,     293,      -6,      -7,     298,     -22,     262,     257,
     300,      -4,     300,      -6,      -3,     -16,     -17,     -18,
     -19,     -20,     -21,     -22,     -23,     -24,     -25,     -33,
     265,     286,     283,     288,     281,     -12,      -8,      -9,
      -2,      -3,     302,     265,     262,     266,     268,      -3,
     300,      -3,     301,     -17,     262,     -34,     282,     285,
     279,     257,     257,     -31,     257,     -13,     259,      -9,
      -3,     -11,     265,     267,     268,     266,     263,     258,
     258,     301,      -3,     301,     262,     -36,     -21,     -37,
     266,     -38,     265,     -41,     -39,     -42,     267,     268,
     257,     -43,      -6,     287,     265,     -21,     -16,     -35,
     294,     259,     -14,     265,     -10,     295,     296,     297,
     301,     -16,     269,     270,     271,     272,     280,     -37,
     265,     -44,     258,     -26,     289,     -28,     -40,     292,
     -37,     -14,     260,     261,     282,     -41,     -41,     -42,
     -42,     -39,     265,     266,     258,     257,     262,     257,
     258,     290,     291,     257,     273,     274,     275,     276,
     277,     278,     260,     263,     265,     -45,     -48,     292,
     264,     -27,     -32,     -16,     -40,     -40,     -40,     -37,
     -37,     -37,     -37,     -37,     -37,     263,     259,     261,
     290,     291,     257,     273,     274,     275,     276,     277,
     278,     -28,     -28,     258,     259,     -15,     -10,     259,
     -48,     -48,     -48,     -49,     -50,     -51,     -52,     -53,
     -54,     -29,     258,     -15,     260,     261,     -46,     258,
     -37,     -37,     -37,     -37,     -37,     -37,     262,     260,
     -10,     -47,     -37,     -21,     260,     -55,     -30,     258,
     261,     258,     -56,     -16,     -47,     284,
};

int yydef[] = {
       0,      -2,       0,       5,       0,       0,       0,       0,
       0,      19,       7,       0,       0,       0,      38,       0,
       0,       0,       0,       6,       0,      30,       0,      33,
      34,      35,      36,      39,      40,      44,      45,       0,
       0,       0,       0,      51,       0,       0,       0,       9,
      12,      13,       0,       0,      37,       0,       0,       0,
       0,       0,       4,       0,      32,      54,      57,      58,
       0,       0,       0,       0,      55,       0,       0,       8,
      11,      14,      15,      16,      17,      18,       0,      41,
      42,       1,       0,       3,      31,       0,      60,      61,
      -2,      63,      -2,      79,       0,      82,      84,      85,
       0,      87,      88,      89,       0,      46,       0,       0,
      20,       0,       0,      24,      10,      27,      28,      29,
       2,       0,       0,       0,       0,       0,       0,       0,
      83,       0,      43,       0,       0,       0,      67,       0,
       0,       0,       0,       0,      59,      77,      78,      80,
      81,      64,      65,      66,      86,       0,      47,      52,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,      23,       0,      92,       0,
       0,       0,       0,      56,      68,      69,       0,      71,
      72,      73,      74,      75,      76,       0,       0,       0,
       0,       0,       0,      96,      98,     100,     102,     104,
     106,      48,       0,      70,       0,       0,      26,      90,
      93,      94,       0,       0,       0,       0,       0,       0,
       0,       0,      53,       0,      22,       0,       0,      95,
      97,      99,     101,     103,     105,     107,       0,      21,
      25,       0,      -2,      49,       0,       0,       0,      91,
     109,       0,       0,       0,     110,      50,
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
# line 141 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
      			} break;
      case 2:
# line 147 "main.y"
      {
      				imprimirTercetos();
      				GenerarAssembler();
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
      case 10:
# line 176 "main.y"
      {
      					TS[yypvt[-2]].ambito[0] = '\0';
      
					strcpy_s(ambitoActual,MAX_LONG_TOKEN+1,TS[yypvt[-2]].nombre);
      
					TS[yypvt[-2]].tipo = PR_FUNCTION + yypvt[-0];
      
					yyval = yypvt[-0];
      				} break;
      case 14:
# line 191 "main.y"
      {
      					strcpy_s(ambitoActual,MAX_LONG_TOKEN+1,"main");
      
					yyval = yypvt[-0];
      				} break;
      case 15:
# line 199 "main.y"
      {
      						yyval = TS[yypvt[-0]].tipo;
      					} break;
      case 16:
# line 203 "main.y"
      {
      						yyval = PR_INT;
      					} break;
      case 17:
# line 207 "main.y"
      {
      						yyval = PR_FLOAT;
      					} break;
      case 18:
# line 211 "main.y"
      {
      						yyval = PR_STRING;
      					} break;
      case 19:
# line 216 "main.y"
      {
      						enDeclaracion = TRUE;
      					} break;
      case 20:
# line 221 "main.y"
      {
      						enDeclaracion = FALSE;
      					} break;
      case 21:
# line 227 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 22:
# line 238 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 23:
# line 250 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 24:
# line 255 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 25:
# line 261 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 26:
# line 266 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 27:
# line 272 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 28:
# line 277 "main.y"
      {
      			yyval = PR_FLOAT;
      		} break;
      case 29:
# line 282 "main.y"
      {
      			yyval = PR_STRING;
      		} break;
      case 41:
# line 317 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 42:
# line 327 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 43:
# line 338 "main.y"
      {
      				verificarDeclaracion(yypvt[-1]);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 46:
# line 360 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 47:
# line 365 "main.y"
      {
      					registroBHUsado = FALSE;
      				} break;
      case 48:
# line 370 "main.y"
      {
      					pushInt(cantTercetos,&pilaDesplazamientos);
      				} break;
      case 49:
# line 377 "main.y"
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
      case 50:
# line 396 "main.y"
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
      case 51:
# line 421 "main.y"
      {
      						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 52:
# line 430 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 53:
# line 437 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 55:
# line 450 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 57:
# line 461 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 58:
# line 468 "main.y"
      {
      						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 59:
# line 480 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 60:
# line 488 "main.y"
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
      case 61:
# line 530 "main.y"
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
      case 62:
# line 571 "main.y"
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
      case 63:
# line 597 "main.y"
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
      case 64:
# line 623 "main.y"
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
      case 65:
# line 635 "main.y"
      {
      									verificarDeclaracion(yypvt[-0]);
      
									if(TS[yypvt[-0]].tipo != PR_STRING)
      									{
      										lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      									}
      								} break;
      case 67:
# line 648 "main.y"
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
      case 68:
# line 670 "main.y"
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
      case 69:
# line 684 "main.y"
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
      case 70:
# line 698 "main.y"
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
      case 71:
# line 727 "main.y"
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
      case 72:
# line 777 "main.y"
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
      case 73:
# line 827 "main.y"
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
      case 74:
# line 877 "main.y"
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
      case 75:
# line 927 "main.y"
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
      case 76:
# line 977 "main.y"
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
      case 77:
# line 1028 "main.y"
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
      case 78:
# line 1055 "main.y"
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
      case 79:
# line 1082 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 80:
# line 1088 "main.y"
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
      case 81:
# line 1115 "main.y"
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
      case 82:
# line 1142 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 83:
# line 1148 "main.y"
      {
      			verificarDeclaracion(yypvt[-0]);
      
			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = TS[yypvt[-0]].tipo;
      		} break;
      case 84:
# line 1161 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_INT;
      		} break;
      case 85:
# line 1172 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_FLOAT;
      		} break;
      case 86:
# line 1183 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = popInt(pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = yypvt[-1];
      		} break;
      case 87:
# line 1194 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 89:
# line 1205 "main.y"
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
      case 90:
# line 1226 "main.y"
      {
      				tercetosInicialesFilterc[0] = cantTercetos;
      			} break;
      case 91:
# line 1235 "main.y"
      {
      				pilaExpresiones = &pilaExpresionesNormal;
      			} break;
      case 92:
# line 1241 "main.y"
      {
      					tipoCondicionFilterc = 0;
      				} break;
      case 93:
# line 1246 "main.y"
      {
      					tipoCondicionFilterc = PR_AND;
      				} break;
      case 94:
# line 1251 "main.y"
      {
      					tipoCondicionFilterc = PR_OR;
      				} break;
      case 95:
# line 1256 "main.y"
      {
      					tipoCondicionFilterc = PR_NOT;
      				} break;
      case 96:
# line 1264 "main.y"
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
# line 1276 "main.y"
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
      case 98:
# line 1346 "main.y"
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
# line 1358 "main.y"
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
      case 100:
# line 1428 "main.y"
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
# line 1440 "main.y"
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
      case 102:
# line 1510 "main.y"
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
# line 1522 "main.y"
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
      case 104:
# line 1592 "main.y"
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
      case 105:
# line 1604 "main.y"
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
      case 106:
# line 1674 "main.y"
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
      case 107:
# line 1686 "main.y"
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
      case 108:
# line 1755 "main.y"
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
      case 109:
# line 1888 "main.y"
      {
      						tercetosInicialesFilterc[0] = cantTercetos;
      					} break;
      case 111:
# line 1895 "main.y"
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
