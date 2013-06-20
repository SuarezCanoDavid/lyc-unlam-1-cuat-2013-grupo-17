
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

# line 1079 "main.y"



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
  -1, 53,
  280, 50,
  -2, 46,
  -1, 55,
  280, 49,
  -2, 67,
  -1, 133,
  290, 81,
  291, 84,
  -2, 80,
  -1, 135,
  274, 94,
  275, 97,
  276, 100,
  277, 103,
  278, 106,
  -2, 91,
  -1, 192,
  261, 109,
  -2, 77,
  0,
};

#define YYNPROD 113
#define YYLAST 340

int yyact[] = {
     129,     130,     131,      61,      44,      20,     135,     158,
       7,     102,     103,      78,      37,      59,      60,       4,
     157,      35,       7,      76,      36,      24,     165,      22,
      20,     164,       7,     163,      23,      81,     162,     161,
      20,      63,     134,     160,      20,      20,      84,      43,
      24,      46,      22,     198,      29,       7,      30,      23,
      24,      89,      22,     135,      24,       7,      22,      23,
      61,       7,     111,      23,      61,      74,      75,      87,
      55,      53,      59,      60,      78,     166,      59,      60,
      97,      72,      73,      72,      73,     105,     106,     107,
     108,     109,     110,      72,      73,     200,      63,      99,
      38,      29,      63,      30,      95,      96,      49,      42,
      33,      28,     156,     141,      70,     199,     154,     140,
      86,      69,     178,     126,     112,      27,     203,     194,
     191,     153,     138,     101,      64,      48,      47,     159,
     104,     100,      98,      41,      39,      31,      85,      10,
     127,       9,      58,      57,     202,     197,     176,     151,
      56,      32,      82,      21,       6,      15,       5,       6,
     175,      25,      21,       3,     150,     174,     149,     173,
     148,      83,     172,      14,     147,     133,     171,     146,
      10,     190,     170,     145,      52,     169,     144,      15,
     168,     143,      66,      45,     201,     196,     192,     189,
     167,     142,     132,      61,      71,     113,      79,      62,
      54,      50,      67,      34,      77,      19,     115,      40,
     188,      51,      32,      65,      50,     152,     114,      32,
      80,     101,      18,      17,     105,      92,      93,      39,
      94,      90,      91,      16,      13,      12,      11,     128,
      68,      26,       3,       8,       2,     166,       1,      75,
      58,       0,      57,      73,      56,       0,       0,     116,
     120,     121,     122,     123,     124,     125,       0,       0,
      70,       0,      88,       0,      32,       0,     191,     106,
     107,     108,     109,     110,     171,     136,     137,     139,
     117,     118,     119,       0,      45,       0,       0,       0,
       0,       0,       0,       0,     141,       0,     155,     103,
     104,      83,       0,     115,      82,       0,       0,       0,
       0,       0,       0,       0,     127,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,     182,     183,     184,     185,     186,     187,
       0,       0,       0,       0,       0,     172,     173,     174,
     175,     176,     189,       0,     193,       0,       0,       0,
     202,     195,     177,      32,       0,       9,     179,     180,
     181,     204,       0,      85,       0,      14,       0,       0,
       0,     169,     170,     133,
};

int yypact[] = {
    -278,   -1000,    -229,    -268,    -150,    -165,    -222,    -132,
   -1000,    -229,    -166,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -265,    -267,    -177,    -133,   -1000,
    -134,    -167,    -255,    -224,   -1000,    -140,    -141,   -1000,
    -168,   -1000,   -1000,   -1000,   -1000,    -201,    -142,    -228,
    -229,   -1000,   -1000,   -1000,    -224,    -155,    -161,   -1000,
   -1000,   -1000,    -229,   -1000,    -194,   -1000,   -1000,    -267,
    -210,    -261,   -1000,   -1000,   -1000,    -197,   -1000,   -1000,
   -1000,   -1000,    -260,    -254,    -156,    -200,    -224,    -233,
    -197,    -197,    -197,    -197,    -173,    -186,   -1000,    -135,
    -175,    -136,    -143,    -281,    -137,    -196,    -205,    -151,
   -1000,   -1000,    -210,    -210,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -229,    -197,    -197,
    -197,    -197,    -197,    -197,    -197,    -197,    -197,    -152,
    -295,    -258,    -254,    -254,    -229,   -1000,   -1000,    -144,
    -194,    -194,    -194,    -194,    -194,    -194,    -295,    -157,
    -162,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -145,   -1000,    -158,   -1000,    -295,    -163,    -274,
    -284,    -138,    -238,    -243,    -245,    -249,    -252,    -256,
    -193,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -228,    -153,
    -213,    -213,    -213,    -197,    -197,    -197,    -197,    -197,
    -197,   -1000,   -1000,   -1000,   -1000,   -1000,    -194,    -194,
    -194,    -194,    -194,    -194,    -146,    -197,    -147,    -229,
   -1000,    -194,   -1000,    -241,    -159,    -176,   -1000,   -1000,
   -1000,    -148,    -197,   -1000,    -194,
};

int yypgo[] = {
       0,     222,     220,     219,     218,     217,     171,     128,
     215,     129,     127,     214,     213,     212,     155,     141,
     211,     139,     203,     202,     200,     198,     138,     197,
     192,     191,     190,     189,     187,     186,     185,     126,
     184,     131,     153,     136,     130,     183,     182,     181,
     178,     177,     176,     175,     174,     173,     172,     157,
     169,     168,     166,     165,     163,     162,     161,     159,
     158,     156,     154,     152,     151,     150,     149,     148,
     144,     135,     134,     133,     132,
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
      38,      39,      41,      42,      43,      45,      46,      37,
      40,      48,      49,      40,      50,      51,      40,      52,
      53,      54,      40,      55,      56,      47,      57,      58,
      47,      59,      60,      47,      61,      62,      47,      63,
      64,      47,      65,      66,      47,      67,      68,      44,
      44,
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
       0,       0,       0,       0,       0,       0,       0,      15,
       1,       0,       0,       5,       0,       0,       5,       0,
       0,       0,       7,       0,       0,       5,       0,       0,
       5,       0,       0,       5,       0,       0,       5,       0,
       0,       5,       0,       0,       5,       0,       0,       5,
       1,
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
     259,     -39,     -21,     -26,      -9,     -34,     -34,     -34,
     -31,     -31,     -31,     -31,     -31,     -31,     259,      -7,
      -8,     295,     296,     297,     -40,     -47,     292,     264,
     -22,     -22,     258,      -7,     260,     261,     -41,     -48,
     -50,     -52,     -55,     -57,     -59,     -61,     -63,     -65,
     -23,     258,     260,      -7,     261,     290,     291,     257,
     273,     274,     275,     276,     277,     278,     262,     -42,
     -49,     -51,     -53,     -56,     -58,     -60,     -62,     -64,
     -66,     -14,     259,     -47,     -47,     -47,     -31,     -31,
     -31,     -31,     -31,     -31,     -24,     -43,     -54,     258,
     -44,     -31,     258,      -9,     -45,     -67,     284,     260,
     261,     -46,     -68,     258,     -31,
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
      50,      70,      73,      31,      36,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,      40,      52,      53,       0,
      55,      56,      57,      58,      59,      60,       0,       0,
       9,      10,      11,      12,      74,      -2,      87,      -2,
      32,       0,      54,       0,       5,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,      37,       4,       8,      75,      82,      85,      88,
      92,      95,      98,     101,     104,     107,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,      33,      76,      83,      86,      89,      93,      96,
      99,     102,     105,     108,       0,       0,       0,       0,
      -2,     112,      90,       0,       0,       0,      34,      78,
     110,       0,       0,      79,     111,
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
      				GenerarAssembler();
      			} break;
      case 4:
# line 105 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 5:
# line 118 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 6:
# line 132 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-2];
      					} break;
      case 7:
# line 137 "main.y"
      {
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 8:
# line 143 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-2];
      				} break;
      case 9:
# line 148 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 10:
# line 154 "main.y"
      {
      			yyval = PR_INT;
      		} break;
      case 11:
# line 159 "main.y"
      {
      			yyval = PR_FLOAT;
      		} break;
      case 12:
# line 164 "main.y"
      {
      			yyval = PR_STRING;
      		} break;
      case 30:
# line 216 "main.y"
      {
      					pushInt(cantTercetos,&pilaCondiciones);
      				} break;
      case 31:
# line 221 "main.y"
      {
      					registroBHUsado = FALSE;
      				} break;
      case 32:
# line 226 "main.y"
      {
      					tercetoInicial = cantTercetos;
      				} break;
      case 33:
# line 233 "main.y"
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
      case 34:
# line 252 "main.y"
      {
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
      case 35:
# line 276 "main.y"
      {
      						pushInt(cantTercetos,&pilaCondiciones);
      					} break;
      case 36:
# line 285 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 37:
# line 292 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeX = JNZ;
      						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 39:
# line 305 "main.y"
      {
      						registroBHUsado = FALSE;
      					} break;
      case 41:
# line 316 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 42:
# line 323 "main.y"
      {
      						borrarTerceto(&tercetoAux);
      						aux = popInt(&pilaCondiciones);
      						tercetoAux.tipoDeX = JMP;
      						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 43:
# line 335 "main.y"
      {
      						aux = popInt(&pilaCondiciones);
      						listaDeTercetos[aux].y = cantTercetos;
      						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
      					} break;
      case 44:
# line 343 "main.y"
      {
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
      case 45:
# line 361 "main.y"
      {
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
      case 51:
# line 388 "main.y"
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
# line 410 "main.y"
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
# line 424 "main.y"
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
# line 438 "main.y"
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
# line 452 "main.y"
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
      					tercetoAux.x = OP_MAYOR;
      					tercetoAux.tipoDeX = TOKEN;
      					aux = popInt(&pilaExpresiones);
      					tercetoAux.y = popInt(&pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					tercetoAux.z = aux;
      					tercetoAux.tipoDeZ = NRO_TERCETO;
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
      case 56:
# line 503 "main.y"
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
      					tercetoAux.x = OP_MAYOR_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					aux = popInt(&pilaExpresiones);
      					tercetoAux.y = popInt(&pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					tercetoAux.z = aux;
      					tercetoAux.tipoDeZ = NRO_TERCETO;
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
      case 57:
# line 554 "main.y"
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
      					tercetoAux.x = OP_MENOR;
      					tercetoAux.tipoDeX = TOKEN;
      					aux = popInt(&pilaExpresiones);
      					tercetoAux.y = popInt(&pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					tercetoAux.z = aux;
      					tercetoAux.tipoDeZ = NRO_TERCETO;
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
      case 58:
# line 605 "main.y"
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
      					tercetoAux.x = OP_MENOR_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					aux = popInt(&pilaExpresiones);
      					tercetoAux.y = popInt(&pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					tercetoAux.z = aux;
      					tercetoAux.tipoDeZ = NRO_TERCETO;
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
      case 59:
# line 656 "main.y"
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
      					tercetoAux.x = OP_IGUAL;
      					tercetoAux.tipoDeX = TOKEN;
      					aux = popInt(&pilaExpresiones);
      					tercetoAux.y = popInt(&pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					tercetoAux.z = aux;
      					tercetoAux.tipoDeZ = NRO_TERCETO;
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
      case 60:
# line 707 "main.y"
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
      					tercetoAux.x = OP_DISTINTO;
      					tercetoAux.tipoDeX = TOKEN;
      					aux = popInt(&pilaExpresiones);
      					tercetoAux.y = popInt(&pilaExpresiones);
      					tercetoAux.tipoDeY = NRO_TERCETO;
      					tercetoAux.z = aux;
      					tercetoAux.tipoDeZ = NRO_TERCETO;
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
      case 61:
# line 759 "main.y"
      {
      				printf("2 EXPRESION -> EXPRESION + TERMINO\n");
      
				tercetoAux.x = OP_SUMA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 62:
# line 773 "main.y"
      {
      				tercetoAux.x = OP_RESTA;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 63:
# line 785 "main.y"
      {
      				printf("3 EXPRESION -> TERMINO\n");
      			} break;
      case 64:
# line 791 "main.y"
      {
      				printf("4 TERMINO -> TERMINO * FACTOR\n");
      
				tercetoAux.x = OP_MULTIPLICACION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 65:
# line 805 "main.y"
      {
      				tercetoAux.x = OP_DIVISION;
      				tercetoAux.tipoDeX = TOKEN;
      				tercetoAux.z = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeZ = NRO_TERCETO;
      				tercetoAux.y = popInt(&pilaExpresiones);
      				tercetoAux.tipoDeY = NRO_TERCETO; 
      
				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      			} break;
      case 66:
# line 817 "main.y"
      {
      				printf("5 TERMINO -> FACTOR\n");
      			} break;
      case 67:
# line 823 "main.y"
      {
      			printf("6 FACTOR -> ID (%s)\n",TS[yypvt[-0]].nombre);
      
			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 68:
# line 835 "main.y"
      {
      			printf("7 FACTOR -> CTE (%d)\n",yyval);
      
			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 69:
# line 847 "main.y"
      {
      			tercetoAux.x = yypvt[-0];
      			tercetoAux.tipoDeX = INDICE_TS;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 70:
# line 857 "main.y"
      {
      			printf("8 FACTOR -> ( EXPRESION )\n");
      
			tercetoAux.x = popInt(&pilaExpresiones);
      			tercetoAux.tipoDeX = NRO_TERCETO;
      			tercetoAux.tipoDeY = IGNORAR;
      			tercetoAux.tipoDeZ = IGNORAR;
      
			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
      		} break;
      case 72:
# line 872 "main.y"
      {
      				fprintf(salidaAS,"FILTERC");
      			} break;
      case 73:
# line 877 "main.y"
      {
      				fprintf(salidaAS,"(");
      			} break;
      case 74:
# line 882 "main.y"
      {
      				++aux;
      			} break;
      case 75:
# line 887 "main.y"
      {
      				fprintf(salidaAS,",");
      			} break;
      case 76:
# line 892 "main.y"
      {
      				fprintf(salidaAS,"[");
      			} break;
      case 77:
# line 897 "main.y"
      {
      				++aux;
      			} break;
      case 78:
# line 902 "main.y"
      {
      				fprintf(salidaAS,"]");
      			} break;
      case 79:
# line 907 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 80:
# line 913 "main.y"
      {
      					++aux;
      				} break;
      case 81:
# line 918 "main.y"
      {
      					++aux;
      				} break;
      case 82:
# line 923 "main.y"
      {
      					fprintf(salidaAS," AND ");
      				} break;
      case 83:
# line 928 "main.y"
      {
      					++aux;
      				} break;
      case 84:
# line 933 "main.y"
      {
      					++aux;
      				} break;
      case 85:
# line 938 "main.y"
      {
      					fprintf(salidaAS," OR ");
      				} break;
      case 86:
# line 943 "main.y"
      {
      					++aux;
      				} break;
      case 87:
# line 948 "main.y"
      {
      					fprintf(salidaAS,"NOT");
      				} break;
      case 88:
# line 953 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 89:
# line 958 "main.y"
      {
      					++aux;
      				} break;
      case 90:
# line 963 "main.y"
      {
      					fprintf(salidaAS,")");
      				} break;
      case 91:
# line 969 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 92:
# line 974 "main.y"
      {
      					fprintf(salidaAS," > ");
      				} break;
      case 93:
# line 979 "main.y"
      {
      					++aux;
      				} break;
      case 94:
# line 984 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 95:
# line 989 "main.y"
      {
      					fprintf(salidaAS," >= ");
      				} break;
      case 96:
# line 994 "main.y"
      {
      					++aux;
      				} break;
      case 97:
# line 999 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 98:
# line 1004 "main.y"
      {
      					fprintf(salidaAS," < ");
      				} break;
      case 99:
# line 1009 "main.y"
      {
      					++aux;
      				} break;
      case 100:
# line 1014 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 101:
# line 1019 "main.y"
      {
      					fprintf(salidaAS," <= ");
      				} break;
      case 102:
# line 1024 "main.y"
      {
      					++aux;
      				} break;
      case 103:
# line 1029 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 104:
# line 1034 "main.y"
      {
      					fprintf(salidaAS," == ");
      				} break;
      case 105:
# line 1039 "main.y"
      {
      					++aux;
      				} break;
      case 106:
# line 1044 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 107:
# line 1049 "main.y"
      {
      					fprintf(salidaAS," != ");
      				} break;
      case 108:
# line 1054 "main.y"
      {
      					++aux;
      				} break;
      case 109:
# line 1060 "main.y"
      {
      						++aux;
      					} break;
      case 110:
# line 1065 "main.y"
      {
      						fprintf(salidaAS,",");
      					} break;
      case 111:
# line 1070 "main.y"
      {
      						++aux;
      					} break;
      case 112:
# line 1075 "main.y"
      {
      						++aux;
      					} break;    }
    goto enstack;
}
