
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

# line 2164 "main.y"



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
  -1, 76,
  280, 69,
  -2, 65,
  -1, 78,
  280, 68,
  -2, 86,
  -1, 206,
  261, 110,
  -2, 113,
  0,
};

#define YYNPROD 114
#define YYLAST 378

int yyact[] = {
      33,      21,      42,       8,      97,      12,       3,      11,
      11,      69,       5,     150,      94,      95,      96,      42,
      37,      64,      35,      84,     107,      34,      33,      36,
     167,     168,      33,     109,       8,      82,      83,      57,
      33,     136,     137,     105,     106,      21,      37,     149,
      35,      33,     122,      34,      68,      36,     115,      33,
      37,      86,      35,     217,      71,      34,     118,      36,
      38,      37,     123,      35,     145,      84,      34,      37,
      36,      35,      84,     150,      34,     109,      36,      82,
      83,     204,      78,      76,      82,      83,      55,     103,
     104,      56,     121,     139,     140,     141,     142,     143,
     144,     103,     104,      86,     131,      87,      62,      88,
      86,     170,     171,     172,     173,     174,     175,     103,
     104,      48,      51,      49,      50,      89,      87,     133,
      88,     129,     130,     194,     181,     180,     181,     120,
     101,     100,     101,     169,      98,      72,      66,      53,
     212,     166,     208,     182,     163,     146,      44,     213,
     211,     197,     193,     178,     135,     113,     112,     111,
     138,     134,     132,      67,      61,      59,      58,     119,
      23,     165,     205,      27,     164,      79,      70,      81,
     148,      22,     117,     214,     209,      18,      28,       6,
     191,      80,      20,     190,     189,       9,     188,      52,
     187,       7,      19,     116,      41,      15,     186,      45,
      14,     196,     147,       6,      15,       9,      46,     110,
       7,      85,      77,      73,      92,      54,      32,     152,
      60,     210,     192,     151,     114,      23,      31,      65,
      40,      75,      63,      18,      30,      74,      29,      90,
      93,      39,      26,      25,      24,      43,      91,      16,
      47,      17,      10,      84,      99,      40,      59,      13,
      28,      39,      20,     102,     108,       4,       2,       1,
       0,       0,       0,       0,      52,       0,     204,     181,
       0,       0,       0,       0,       0,      73,       0,      52,
       0,       0,      70,       0,     139,     124,     125,       0,
     135,     126,     127,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,     128,       0,       0,
       0,       0,     104,      79,       0,       0,     106,      81,
       0,       0,       0,     157,     158,     159,     160,     161,
     162,     153,      80,     154,     155,     156,       0,       0,
       0,       0,      52,       0,       0,     140,     141,     142,
     143,     144,     186,     213,       0,     137,     138,     117,
     179,       0,       0,       0,       0,       0,       0,       0,
     183,     184,     185,     176,     177,       0,       0,     195,
     164,       0,     198,     199,     200,     201,     202,     203,
     168,     169,     148,       0,     206,     165,     187,     188,
     189,     190,     191,     196,     152,     116,     119,       0,
     207,       0,       0,      27,       0,       0,     206,       0,
      52,     216,       0,     205,       0,       0,       0,     215,
       0,      22,
};

int yypact[] = {
    -293,   -1000,   -1000,    -290,    -295,   -1000,    -291,    -291,
   -1000,   -1000,    -265,    -209,   -1000,    -271,    -291,   -1000,
    -125,    -301,   -1000,    -218,   -1000,    -160,    -218,    -135,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
    -204,    -248,    -107,    -108,   -1000,    -109,    -169,    -218,
    -284,    -136,    -110,    -250,    -213,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,    -137,   -1000,   -1000,   -1000,
   -1000,    -191,    -156,    -239,    -218,   -1000,    -283,    -297,
   -1000,    -138,   -1000,    -173,   -1000,    -213,    -139,   -1000,
   -1000,    -218,   -1000,    -180,   -1000,   -1000,    -248,    -236,
    -260,   -1000,   -1000,   -1000,    -196,   -1000,   -1000,    -115,
    -116,    -117,   -1000,    -243,    -238,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -141,    -181,    -223,    -224,    -196,
    -196,    -196,    -196,    -152,    -166,   -1000,    -111,   -1000,
   -1000,   -1000,    -151,    -112,    -118,    -257,    -113,    -190,
    -203,    -126,   -1000,   -1000,    -236,    -236,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,    -253,   -1000,   -1000,    -218,
    -196,    -196,    -196,    -196,    -196,    -196,    -196,    -196,
    -196,    -127,    -283,    -132,    -266,    -134,    -176,    -238,
    -238,    -218,   -1000,   -1000,    -119,    -180,    -180,    -180,
    -180,    -180,    -180,    -283,    -143,   -1000,    -128,    -197,
    -197,    -197,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -120,   -1000,    -145,   -1000,    -283,   -1000,   -1000,
   -1000,    -121,    -196,    -196,    -196,    -196,    -196,    -196,
    -189,   -1000,   -1000,   -1000,    -196,   -1000,    -180,    -180,
    -180,    -180,    -180,    -180,    -239,    -130,    -180,   -1000,
    -122,    -133,    -123,   -1000,   -1000,    -218,    -196,    -233,
   -1000,   -1000,
};

int yypgo[] = {
       0,     239,     238,     237,     233,     170,     231,     229,
     167,     177,     173,     226,     225,     165,     153,     224,
     223,     221,     158,     156,     161,     152,     220,     219,
     218,     155,     166,     214,     212,     206,     204,     203,
     179,     202,     201,     200,     199,     198,     197,     196,
     195,     151,     194,     169,     162,     157,     159,     193,
     191,     186,     185,     154,     160,     182,     176,     174,
     172,     171,     168,     164,     163,
};

int yyr1[] = {
       0,       1,       4,       2,       6,       2,       3,       3,
       3,       9,       9,      10,      10,      11,      12,      12,
      12,      13,      15,      15,      15,      15,      16,       8,
      17,      17,      18,      18,      19,      19,      14,      14,
      14,       5,      20,      20,      21,      21,      21,      21,
       7,       7,      22,      22,      26,      26,      27,      23,
      23,      30,      31,      33,      34,      28,      35,      36,
      29,      24,      39,      37,      38,      40,      38,      25,
      25,      25,      25,      42,      43,      43,      32,      32,
      32,      32,      44,      44,      44,      44,      44,      44,
      41,      41,      41,      45,      45,      45,      46,      46,
      46,      46,      46,      48,      50,      47,      49,      49,
      49,      49,      53,      52,      54,      52,      55,      52,
      56,      52,      57,      52,      58,      52,      59,      60,
      51,      51,
};

int yyr2[] = {
       2,       1,       0,       6,       0,       5,       1,       2,
       1,       2,       1,       3,       2,       4,       2,       1,
       1,       2,       1,       1,       1,       1,       0,       4,
       8,       7,       3,       1,       3,       1,       1,       1,
       1,       1,       3,       2,       1,       1,       1,       1,
       3,       2,       1,       1,       4,       4,       4,       1,
       1,       0,       0,       0,       0,      14,       0,       0,
       8,       2,       0,       6,       1,       0,       4,       3,
       3,       3,       3,       3,       1,       1,       1,       3,
       3,       4,       3,       3,       3,       3,       3,       3,
       3,       3,       1,       3,       3,       1,       1,       1,
       1,       3,       1,       0,       0,      10,       1,       3,
       3,       4,       0,       4,       0,       4,       0,       4,
       0,       4,       0,       4,       0,       4,       0,       0,
       5,       1,
};

int yychk[] = {
   -1000,      -1,      -2,     299,      -3,     300,      -8,      -9,
     293,     -10,     -11,     298,     300,      -6,      -9,     -10,
     -16,     -12,     -13,      -8,      -5,     302,     -20,     -21,
     -22,     -23,     -24,     -25,     -26,     -27,     -28,     -29,
     -37,     265,     286,     283,     288,     281,     265,      -4,
      -7,     -26,     286,     -17,     259,     -13,      -5,     -15,
     265,     267,     268,     266,     -21,     262,     -38,     282,
     285,     279,     257,     257,     -35,     257,     263,      -5,
     301,     -26,     262,     257,     294,     259,     -18,     265,
     262,     -40,     -25,     -41,     266,     -42,     265,     -45,
     -43,     -46,     267,     268,     257,     -47,     287,     266,
     268,     265,     -25,     -20,     -39,     -14,     295,     296,
     297,     301,     262,     -18,     260,     261,     -20,     269,
     270,     271,     272,     280,     -41,     265,     -48,     258,
     258,     258,     -30,     289,     -32,     -44,     292,     -41,
     260,     263,     265,     282,     -45,     -45,     -46,     -46,
     -43,     265,     266,     258,     257,     262,     257,     258,
     290,     291,     257,     273,     274,     275,     276,     277,
     278,     263,     259,     -49,     -52,     292,     264,     -31,
     -36,     -20,     -44,     -44,     -44,     -41,     -41,     -41,
     -41,     -41,     -41,     259,     -19,     -14,     261,     290,
     291,     257,     273,     274,     275,     276,     277,     278,
     -32,     -32,     258,     -19,     260,     261,     259,     -52,
     -52,     -52,     -53,     -54,     -55,     -56,     -57,     -58,
     -33,     258,     260,     -14,     -50,     258,     -41,     -41,
     -41,     -41,     -41,     -41,     262,     -51,     -41,     -25,
     260,     -59,     -34,     258,     261,     258,     -60,     -20,
     -51,     284,
};

int yydef[] = {
       0,      -2,       1,       0,       0,       4,       6,       8,
      22,      10,       0,       0,       2,       0,       7,       9,
       0,       0,      12,      15,      16,       0,      33,       0,
      36,      37,      38,      39,      42,      43,      47,      48,
       0,       0,       0,       0,      54,       0,       0,       0,
       0,       0,       0,       0,       0,      11,      14,      17,
      18,      19,      20,      21,       0,      35,      57,      60,
      61,       0,       0,       0,       0,      58,       0,       0,
       5,       0,      41,       0,      23,       0,       0,      27,
      34,       0,      63,      64,      -2,      66,      -2,      82,
       0,      85,      87,      88,       0,      90,      91,       0,
       0,       0,      49,       0,       0,      13,      30,      31,
      32,       3,      40,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,      86,       0,      44,
      45,      46,       0,       0,       0,      70,       0,       0,
       0,       0,      26,      62,      80,      81,      83,      84,
      67,      68,      69,      89,       0,      50,      55,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,      94,       0,       0,       0,
       0,      59,      71,      72,       0,      74,      75,      76,
      77,      78,      79,       0,       0,      29,       0,       0,
       0,       0,      98,     100,     102,     104,     106,     108,
      51,       0,      73,       0,      25,       0,      92,      95,
      96,       0,       0,       0,       0,       0,       0,       0,
       0,      56,      24,      28,       0,      97,      99,     101,
     103,     105,     107,     109,       0,       0,      -2,      52,
       0,       0,       0,      93,     111,       0,       0,       0,
     112,      53,
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
# line 148 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_BEGINPROG;
      				tercetoAux.tipoDeX = TOKEN;
      				crearTerceto(&tercetoAux);
      			} break;
      case 3:
# line 156 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_ENDPROG;
      				tercetoAux.tipoDeX = TOKEN;
      				crearTerceto(&tercetoAux);
      			} break;
      case 4:
# line 165 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_BEGINPROG;
      				tercetoAux.tipoDeX = TOKEN;
      				crearTerceto(&tercetoAux);
      			} break;
      case 5:
# line 173 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_ENDPROG;
      				tercetoAux.tipoDeX = TOKEN;
      				crearTerceto(&tercetoAux);
      			} break;
      case 11:
# line 188 "main.y"
      {
      				if((TS[yypvt[-2]].tipo-PR_FUNCTION == PR_STRING && TS[yypvt[-0]].tipo != PR_STRING) || (TS[yypvt[-2]].tipo-PR_FUNCTION != PR_STRING && TS[yypvt[-0]].tipo == PR_STRING))
      				{
      					lanzarError("El tipo retornado no es compatible con el declarado");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = yypvt[-0];
      				tercetoAux.tipoDeZ = INDICE_TS; 
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_RETURN;
      				tercetoAux.tipoDeX = TOKEN;
      				crearTerceto(&tercetoAux);
      			} break;
      case 12:
# line 210 "main.y"
      {
      				if((TS[yypvt[-1]].tipo-PR_FUNCTION == PR_STRING && TS[yypvt[-0]].tipo != PR_STRING) || (TS[yypvt[-1]].tipo-PR_FUNCTION != PR_STRING && TS[yypvt[-0]].tipo == PR_STRING))
      				{
      					lanzarError("El tipo retornado no es compatible con el declarado");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = yypvt[-0];
      				tercetoAux.tipoDeZ = INDICE_TS; 
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_RETURN;
      				tercetoAux.tipoDeX = TOKEN;
      				crearTerceto(&tercetoAux);
      			} break;
      case 13:
# line 232 "main.y"
      {
      					TS[yypvt[-2]].ambito[0] = '\0';
      
					strcpy_s(ambitoActual,MAX_LONG_TOKEN+1,TS[yypvt[-2]].nombre);
      
					if(TS[yypvt[-2]].tipo != 0)
      					{
      						lanzarError("La funcion no puede tener el nombre de una variable ya declarada");
      					}
      
					TS[yypvt[-2]].tipo = PR_FUNCTION + yypvt[-0];
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.x = PR_FUNCTION;
      					tercetoAux.tipoDeX = TOKEN;
      					tercetoAux.y = yypvt[-2];
      					tercetoAux.tipoDeY = INDICE_TS;
      					crearTerceto(&tercetoAux);
      
					yyval = yypvt[-2];
      				} break;
      case 17:
# line 259 "main.y"
      {
      					strcpy_s(ambitoActual,MAX_LONG_TOKEN+1,"main");
      
					yyval = yypvt[-0];
      				} break;
      case 18:
# line 267 "main.y"
      {
      						yyval = yypvt[-0];
      					} break;
      case 19:
# line 271 "main.y"
      {
      						yyval = yypvt[-0];
      					} break;
      case 20:
# line 275 "main.y"
      {
      						yyval = yypvt[-0];
      					} break;
      case 21:
# line 279 "main.y"
      {
      						yyval = yypvt[-0];
      					} break;
      case 22:
# line 284 "main.y"
      {
      						enDeclaracion = TRUE;
      					} break;
      case 23:
# line 289 "main.y"
      {
      						enDeclaracion = FALSE;
      					} break;
      case 24:
# line 295 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 25:
# line 306 "main.y"
      {
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 26:
# line 318 "main.y"
      {
      						if(strcmp(TS[yypvt[-0]].nombre,ambitoActual) == 0)
      						{
      							lanzarError("Nombre de la variable y de funcion deben ser distintas");
      						}
      
						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 27:
# line 328 "main.y"
      {
      						if(strcmp(TS[yypvt[-0]].nombre,ambitoActual) == 0)
      						{
      							lanzarError("Nombre de la variable y de funcion deben ser distintas");
      						}
      
						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 28:
# line 339 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 29:
# line 344 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 30:
# line 350 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 31:
# line 355 "main.y"
      {
      			yyval = PR_FLOAT;
      		} break;
      case 32:
# line 360 "main.y"
      {
      			yyval = PR_STRING;
      		} break;
      case 44:
# line 393 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 45:
# line 403 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 46:
# line 414 "main.y"
      {
      				verificarDeclaracion(yypvt[-1]);
      
				if(TS[yypvt[-1]].tipo-PR_INT == PR_FUNCTION || TS[yypvt[-1]].tipo-PR_FLOAT == PR_FUNCTION || TS[yypvt[-1]].tipo-PR_STRING == PR_FUNCTION)
      				{
      					if(strcmp(ambitoActual,"main") != 0)
      					{
      						lanzarError("No se puede llamar a una funcion dentro de otra");
      					}
      
					borrarTerceto(&tercetoAux);
      					tercetoAux.tipoDeX = CALL;
      					tercetoAux.y = yypvt[-1];
      					tercetoAux.tipoDeY = INDICE_TS;
      					crearTerceto(&tercetoAux);
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_WPRINT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-1];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      			} break;
      case 49:
# line 450 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 50:
# line 455 "main.y"
      {
      					registroBHUsado = FALSE;
      				} break;
      case 51:
# line 460 "main.y"
      {
      					pushInt(cantTercetos,&pilaDesplazamientos);
      				} break;
      case 52:
# line 467 "main.y"
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
      case 53:
# line 486 "main.y"
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
      case 54:
# line 511 "main.y"
      {
      						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 55:
# line 520 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 56:
# line 527 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 58:
# line 540 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 60:
# line 551 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 61:
# line 558 "main.y"
      {
      						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 62:
# line 570 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 63:
# line 578 "main.y"
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
      
				if(TS[yypvt[-2]].tipo-PR_INT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_FLOAT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_STRING == PR_FUNCTION)
      				{
      					lanzarError("No puede asignar un valor a una funcion");
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
      case 64:
# line 625 "main.y"
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
      
				if(TS[yypvt[-2]].tipo-PR_INT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_FLOAT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_STRING == PR_FUNCTION)
      				{
      					lanzarError("No puede asignar un valor a una funcion");
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
      case 65:
# line 671 "main.y"
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
      
				if(TS[yypvt[-2]].tipo-PR_INT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_FLOAT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_STRING == PR_FUNCTION)
      				{
      					lanzarError("No puede asignar un valor a una funcion");
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
      case 66:
# line 702 "main.y"
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
      
				if(TS[yypvt[-2]].tipo-PR_INT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_FLOAT == PR_FUNCTION || TS[yypvt[-2]].tipo-PR_STRING == PR_FUNCTION)
      				{
      					lanzarError("No puede asignar un valor a una funcion");
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
      case 67:
# line 733 "main.y"
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
      case 68:
# line 745 "main.y"
      {
      									verificarDeclaracion(yypvt[-0]);
      
									if(TS[yypvt[-0]].tipo != PR_STRING && TS[yypvt[-0]].tipo-PR_FUNCTION != PR_STRING)
      									{
      										lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      									}
      
									if(TS[yypvt[-0]].tipo-PR_STRING == PR_FUNCTION)
      									{
      										if(strcmp(ambitoActual,"main") != 0)
      										{
      											lanzarError("No se puede llamar a una funcion dentro de otra");
      										}
      
										borrarTerceto(&tercetoAux);
      										tercetoAux.tipoDeX = CALL;
      										tercetoAux.y = yypvt[-0];
      										tercetoAux.tipoDeY = INDICE_TS;
      										crearTerceto(&tercetoAux);
      									}
      								} break;
      case 70:
# line 772 "main.y"
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
      case 71:
# line 794 "main.y"
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
      case 72:
# line 808 "main.y"
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
      case 73:
# line 822 "main.y"
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
      case 74:
# line 851 "main.y"
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
      case 75:
# line 901 "main.y"
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
      case 76:
# line 951 "main.y"
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
      case 77:
# line 1001 "main.y"
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
      case 78:
# line 1051 "main.y"
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
      case 79:
# line 1101 "main.y"
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
      case 80:
# line 1152 "main.y"
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
      case 81:
# line 1179 "main.y"
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
      case 82:
# line 1206 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 83:
# line 1212 "main.y"
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
      case 84:
# line 1239 "main.y"
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
      case 85:
# line 1266 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 86:
# line 1272 "main.y"
      {
      			verificarDeclaracion(yypvt[-0]);
      
			if(TS[yypvt[-0]].tipo-PR_INT == PR_FUNCTION || TS[yypvt[-0]].tipo-PR_FLOAT == PR_FUNCTION || TS[yypvt[-0]].tipo-PR_STRING == PR_FUNCTION)
      			{
      				if(strcmp(ambitoActual,"main") != 0)
      				{
      					lanzarError("No se puede llamar a una funcion dentro de otra");
      				}
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.tipoDeX = CALL;
      				tercetoAux.y = yypvt[-0];
      				tercetoAux.tipoDeY = INDICE_TS;
      				crearTerceto(&tercetoAux);
      
				yyval = TS[yypvt[-0]].tipo-PR_FUNCTION;
      			}
      			else
      			{
      				yyval = TS[yypvt[-0]].tipo;
      			}
      
			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      		} break;
      case 87:
# line 1303 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_INT;
      		} break;
      case 88:
# line 1314 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = PR_FLOAT;
      		} break;
      case 89:
# line 1325 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = popInt(pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      
			yyval = yypvt[-1];
      		} break;
      case 90:
# line 1336 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 91:
# line 1342 "main.y"
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
      case 92:
# line 1363 "main.y"
      {
      				tercetosInicialesFilterc[0] = cantTercetos;
      			} break;
      case 93:
# line 1372 "main.y"
      {
      				pilaExpresiones = &pilaExpresionesNormal;
      			} break;
      case 94:
# line 1378 "main.y"
      {
      					tipoCondicionFilterc = 0;
      				} break;
      case 95:
# line 1383 "main.y"
      {
      					tipoCondicionFilterc = PR_AND;
      				} break;
      case 96:
# line 1388 "main.y"
      {
      					tipoCondicionFilterc = PR_OR;
      				} break;
      case 97:
# line 1393 "main.y"
      {
      					tipoCondicionFilterc = PR_NOT;
      				} break;
      case 98:
# line 1401 "main.y"
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
# line 1413 "main.y"
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
      case 100:
# line 1483 "main.y"
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
# line 1495 "main.y"
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
      case 102:
# line 1565 "main.y"
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
# line 1577 "main.y"
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
      case 104:
# line 1647 "main.y"
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
# line 1659 "main.y"
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
      case 106:
# line 1729 "main.y"
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
# line 1741 "main.y"
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
      case 108:
# line 1811 "main.y"
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
      case 109:
# line 1823 "main.y"
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
      case 110:
# line 1892 "main.y"
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
      case 111:
# line 2025 "main.y"
      {
      						tercetosInicialesFilterc[0] = cantTercetos;
      					} break;
      case 113:
# line 2032 "main.y"
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
