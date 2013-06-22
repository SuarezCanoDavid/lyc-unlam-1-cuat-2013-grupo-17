
# line 2 "main.y"
#include "AnalizadorLexico.h"
#include "Pila.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *salidaAS;
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
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
#define YYERRCODE 256

# line 1601 "main.y"



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

	for(i = pilaExpresiones->tope-1; i >= 0; --i)
	{
		printf("%d\n",pilaExpresiones->contenedor[i]);
	}
	/*
	printf("%d\n",pilaExpresiones.tope);
	printf("%d\n",pilaCondiciones.tope);
	printf("%d\n",pilaColasIncrementos.tope);
	printf("%d\n",pilaDesplazamientos.tope);
	*/
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
  -1, 53,
  280, 50,
  -2, 46,
  -1, 55,
  280, 49,
  -2, 67,
  -1, 175,
  261, 91,
  -2, 94,
  0,
};

#define YYNPROD 95
#define YYLAST 322

int yyact[] = {
     132,     133,     134,      61,      44,      20,     116,       7,
       7,     136,     137,      78,      76,      59,      60,       4,
     102,     103,      37,      35,      20,      24,      36,      22,
      20,      46,       7,     116,      23,      81,      72,      73,
      20,      63,     115,      29,      20,      30,      84,      43,
      24,     111,      22,     186,      87,       7,     173,      23,
      24,      89,      22,      99,      24,       7,      22,      23,
      61,       7,      49,      23,      61,      74,      75,      42,
      55,      53,      59,      60,      78,      33,      59,      60,
      97,      72,      73,      95,      96,     105,     106,     107,
     108,     109,     110,      72,      73,      28,      63,     181,
     150,     135,      63,     139,     140,     141,     142,     143,
     144,      38,      29,      70,      30,     177,     163,     149,
      86,      69,     151,     129,     112,      27,     182,     180,
     166,     162,     147,     101,      64,      48,      47,     138,
     104,     100,      98,      41,      39,      31,      85,      10,
     130,     174,      14,      58,      57,     183,     178,     114,
       9,      32,      83,      56,      21,       6,     160,     159,
       6,      15,       5,      21,     158,      25,      45,       3,
     157,     156,     155,     165,     113,      79,      62,      54,
      10,      50,      82,      67,      52,      34,      19,     118,
      51,      40,      65,      15,     179,     161,     117,      80,
      18,      66,      17,      61,      16,      13,      39,      12,
      11,     131,      26,      71,      77,       3,       8,       2,
       1,       0,      32,      68,     173,       0,       0,      32,
       0,       0,       0,      50,     105,       0,      92,      93,
     101,      94,       0,       0,      90,      91,       0,       0,
       0,       0,       0,      70,       0,      88,       0,       0,
      75,      58,       0,      57,       0,       0,      73,      56,
     123,     124,     125,     126,     127,     128,     119,      45,
       0,     120,     121,     122,       0,       0,       0,      32,
       0,       0,     106,     107,     108,     109,     110,     155,
     182,       0,     148,     103,     104,      83,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
     152,     153,     154,       0,       0,     150,       0,     164,
     145,     146,     167,     168,     169,     170,     171,     172,
     137,     138,     114,       0,     175,     130,     156,     157,
     158,     159,     160,     165,       0,       0,      85,       0,
     176,       0,       0,      14,     118,      82,     175,       0,
      32,     185,       0,     174,       0,       0,       0,     184,
       0,       9,
};

int yypact[] = {
    -278,   -1000,    -229,    -279,    -150,    -177,    -231,    -132,
   -1000,    -229,    -193,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -263,    -261,    -168,    -133,   -1000,
    -134,    -199,    -255,    -240,   -1000,    -140,    -141,   -1000,
    -204,   -1000,   -1000,   -1000,   -1000,    -201,    -142,    -245,
    -229,   -1000,   -1000,   -1000,    -240,    -155,    -162,   -1000,
   -1000,   -1000,    -229,   -1000,    -239,   -1000,   -1000,    -261,
    -210,    -268,   -1000,   -1000,   -1000,    -197,   -1000,   -1000,
   -1000,   -1000,    -260,    -254,    -156,    -219,    -240,    -233,
    -197,    -197,    -197,    -197,    -190,    -186,   -1000,    -135,
    -211,    -136,    -143,    -274,    -137,    -196,    -222,    -151,
   -1000,   -1000,    -210,    -210,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -258,   -1000,   -1000,    -229,    -197,    -197,
    -197,    -197,    -197,    -197,    -197,    -197,    -197,    -152,
    -295,    -172,    -281,    -138,    -182,    -254,    -254,    -229,
   -1000,   -1000,    -144,    -239,    -239,    -239,    -239,    -239,
    -239,    -295,    -157,    -173,   -1000,   -1000,   -1000,    -153,
    -237,    -237,    -237,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -145,   -1000,    -158,   -1000,    -295,   -1000,
   -1000,   -1000,    -146,    -197,    -197,    -197,    -197,    -197,
    -197,    -216,   -1000,   -1000,   -1000,    -197,   -1000,    -239,
    -239,    -239,    -239,    -239,    -239,    -245,    -159,    -239,
   -1000,    -147,    -174,    -148,   -1000,   -1000,    -229,    -197,
    -241,   -1000,   -1000,
};

int yypgo[] = {
       0,     192,     191,     190,     189,     186,     150,     128,
     185,     136,     127,     184,     183,     181,     130,     145,
     180,     140,     178,     176,     175,     174,     162,     173,
     172,     169,     167,     166,     165,     163,     161,     126,
     159,     132,     138,     139,     131,     158,     157,     156,
     155,     129,     135,     154,     153,     152,     148,     143,
     142,     134,     133,
};

int yyr1[] = {
       0,       1,       1,       2,       5,       5,       6,       6,
       7,       7,       8,       8,       8,       3,       9,       9,
      10,      10,      10,      10,       4,       4,      11,      11,
      15,      15,      16,      17,      12,      12,      20,      21,
      23,      24,      18,      25,      26,      19,      13,      29,
      27,      28,      30,      28,      14,      14,      14,      14,
      32,      33,      33,      22,      22,      22,      22,      34,
      34,      34,      34,      34,      34,      31,      31,      31,
      35,      35,      35,      36,      36,      36,      36,      36,
      38,      40,      37,      39,      39,      39,      39,      43,
      42,      44,      42,      45,      42,      46,      42,      47,
      42,      48,      42,      49,      50,      41,      41,
};

int yyr2[] = {
       2,       2,       1,       3,       8,       7,       3,       1,
       3,       1,       1,       1,       1,       1,       3,       2,
       1,       1,       1,       1,       3,       2,       1,       1,
       3,       3,       3,       2,       1,       1,       0,       0,
       0,       0,      14,       0,       0,       8,       2,       0,
       6,       1,       0,       4,       3,       3,       3,       3,
       3,       1,       1,       1,       3,       3,       4,       3,
       3,       3,       3,       3,       3,       3,       3,       1,
       3,       3,       1,       1,       1,       1,       3,       1,
       0,       0,      10,       1,       3,       3,       4,       0,
       4,       0,       4,       0,       4,       0,       4,       0,
       4,       0,       4,       0,       0,       5,       1,
};

int yychk[] = {
   -1000,      -1,      -2,      -4,     293,     -15,     -17,     286,
      -3,      -9,     -10,     -11,     -12,     -13,     -14,     -15,
     -16,     -18,     -19,     -27,     265,     -17,     283,     288,
     281,     -15,      -5,     259,     262,     266,     268,     257,
     -10,     262,     -28,     282,     285,     279,     265,     257,
     -25,     257,     262,     294,     259,      -6,     265,     258,
     258,     262,     -30,     -14,     -31,     266,     -32,     265,
     -35,     -33,     -36,     267,     268,     257,     -37,     287,
     258,     -14,      -9,     -29,      -6,     260,     261,      -9,
     269,     270,     271,     272,     280,     -31,     265,     -38,
     -20,     289,     -22,     -34,     292,     -31,     260,     263,
      -6,     282,     -35,     -35,     -36,     -36,     -33,     265,
     266,     258,     257,     262,     257,     258,     290,     291,
     257,     273,     274,     275,     276,     277,     278,     263,
     259,     -39,     -42,     292,     264,     -21,     -26,      -9,
     -34,     -34,     -34,     -31,     -31,     -31,     -31,     -31,
     -31,     259,      -7,      -8,     295,     296,     297,     261,
     290,     291,     257,     273,     274,     275,     276,     277,
     278,     -22,     -22,     258,      -7,     260,     261,     259,
     -42,     -42,     -42,     -43,     -44,     -45,     -46,     -47,
     -48,     -23,     258,     260,      -7,     -40,     258,     -31,
     -31,     -31,     -31,     -31,     -31,     262,     -41,     -31,
     -14,     260,     -49,     -24,     258,     261,     258,     -50,
      -9,     -41,     284,
};

int yydef[] = {
       0,      -2,       0,       2,       0,       0,       0,       0,
       1,      13,       0,      16,      17,      18,      19,      22,
      23,      28,      29,       0,       0,       0,       0,      35,
       0,       0,       0,       0,      21,       0,       0,      27,
       0,      15,      38,      41,      42,       0,       0,       0,
       0,      39,      20,       3,       0,       0,       7,      24,
      25,      14,       0,      44,      45,      -2,      47,      -2,
      63,       0,      66,      68,      69,       0,      71,      72,
      26,      30,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,      67,       0,
       0,       0,       0,      51,       0,       0,       0,       0,
       6,      43,      61,      62,      64,      65,      48,      49,
      50,      70,       0,      31,      36,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,      75,       0,       0,       0,       0,      40,
      52,      53,       0,      55,      56,      57,      58,      59,
      60,       0,       0,       9,      10,      11,      12,       0,
       0,       0,       0,      79,      81,      83,      85,      87,
      89,      32,       0,      54,       0,       5,       0,      73,
      76,      77,       0,       0,       0,       0,       0,       0,
       0,       0,      37,       4,       8,       0,      78,      80,
      82,      84,      86,      88,      90,       0,       0,      -2,
      33,       0,       0,       0,      74,      92,       0,       0,
       0,      93,      34,
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
# line 130 "main.y"
      {
      				GenerarAssembler();
      			} break;
      case 4:
# line 141 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 5:
# line 154 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 6:
# line 168 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-2];
      					} break;
      case 7:
# line 173 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 8:
# line 179 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-2];
      				} break;
      case 9:
# line 184 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 10:
# line 190 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 11:
# line 195 "main.y"
      {
      			yyval = PR_FLOAT;
      		} break;
      case 12:
# line 200 "main.y"
      {
      			yyval = PR_STRING;
      		} break;
      case 30:
# line 252 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 31:
# line 257 "main.y"
      {
      					registroBHUsado = FALSE;
      				} break;
      case 32:
# line 262 "main.y"
      {
      					pushInt(cantTercetos,&pilaDesplazamientos);
      				} break;
      case 33:
# line 269 "main.y"
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
      case 34:
# line 288 "main.y"
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
      case 35:
# line 313 "main.y"
      {
      						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 36:
# line 322 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 37:
# line 329 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 39:
# line 342 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 41:
# line 353 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 42:
# line 360 "main.y"
      {
      						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 43:
# line 372 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 44:
# line 380 "main.y"
      {
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = yypvt[-0];
      				tercetoAux.tipoDeZ = INDICE_TS; 
      				crearTerceto(&tercetoAux);
      
				yyval = yypvt[-2];
      			} break;
      case 45:
# line 393 "main.y"
      {
      				tercetoAux.x = OP_ASIGNACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.y = yypvt[-2];
      				tercetoAux.tipoDeY = INDICE_TS;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO; 
      				crearTerceto(&tercetoAux);
      
				yyval = yypvt[-2];
      			} break;
      case 51:
# line 417 "main.y"
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
      case 52:
# line 439 "main.y"
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
      case 53:
# line 453 "main.y"
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
      case 54:
# line 467 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = PR_NOT;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.tipoDeY = BH;
      				crearTerceto(&tercetoAux);
      
				borrarTerceto(&tercetoAux);
      				tercetoAux.tipoDeX = JZ;
      				pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      			} break;
      case 55:
# line 481 "main.y"
      {
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
      case 56:
# line 526 "main.y"
      {
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
      case 57:
# line 571 "main.y"
      {
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
      case 58:
# line 616 "main.y"
      {
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
      case 59:
# line 661 "main.y"
      {
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
      case 60:
# line 706 "main.y"
      {
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
      case 61:
# line 752 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_SUMA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      			} break;
      case 62:
# line 765 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_RESTA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      			} break;
      case 64:
# line 781 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_MULTIPLICACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      			} break;
      case 65:
# line 794 "main.y"
      {
      				borrarTerceto(&tercetoAux);
      				tercetoAux.x = OP_DIVISION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      			} break;
      case 67:
# line 810 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      		} break;
      case 68:
# line 819 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      		} break;
      case 69:
# line 828 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      		} break;
      case 70:
# line 837 "main.y"
      {
      			borrarTerceto(&tercetoAux);
      			tercetoAux.x = popInt(pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      
			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);
      		} break;
      case 72:
# line 849 "main.y"
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
      case 73:
# line 870 "main.y"
      {
      				tercetosInicialesFilterc[0] = cantTercetos;
      			} break;
      case 74:
# line 879 "main.y"
      {
      				pilaExpresiones = &pilaExpresionesNormal;
      			} break;
      case 75:
# line 885 "main.y"
      {
      					tipoCondicionFilterc = 0;
      				} break;
      case 76:
# line 890 "main.y"
      {
      					tipoCondicionFilterc = PR_AND;
      				} break;
      case 77:
# line 895 "main.y"
      {
      					tipoCondicionFilterc = PR_OR;
      				} break;
      case 78:
# line 900 "main.y"
      {
      					tipoCondicionFilterc = PR_NOT;
      				} break;
      case 79:
# line 908 "main.y"
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
      case 80:
# line 920 "main.y"
      {
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
      case 81:
# line 985 "main.y"
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
      case 82:
# line 997 "main.y"
      {
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
      case 83:
# line 1062 "main.y"
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
      case 84:
# line 1074 "main.y"
      {
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
      case 85:
# line 1139 "main.y"
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
      case 86:
# line 1151 "main.y"
      {
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
      case 87:
# line 1216 "main.y"
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
      case 88:
# line 1228 "main.y"
      {
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
      case 89:
# line 1293 "main.y"
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
      case 90:
# line 1305 "main.y"
      {
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
      case 91:
# line 1369 "main.y"
      {
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
      case 92:
# line 1482 "main.y"
      {
      						tercetosInicialesFilterc[0] = cantTercetos;
      					} break;
      case 94:
# line 1489 "main.y"
      {
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
