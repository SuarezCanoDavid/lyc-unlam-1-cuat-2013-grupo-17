
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

# line 730 "main.y"



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
  0,
};

#define YYNPROD 77
#define YYLAST 345

int yyact[] = {
     139,     140,     141,      54,      39,      19,     112,       6,
       6,     125,     126,      51,      86,      52,      53,       4,
      71,      72,      33,      19,      19,      18,     121,      23,
      93,     122,      22,      41,      24,      19,      33,      87,
     112,      56,     111,      18,     119,      23,      47,      38,
      22,      19,      24,      64,     108,      18,      92,      23,
     169,      19,      22,     134,      24,      82,      83,      80,
      81,      18,     157,      23,      80,      81,      22,      89,
      24,      18,      54,      23,     115,     116,      22,      54,
      24,      44,      61,      59,      52,      53,      42,      51,
      43,      52,      53,     124,      80,      81,     113,     114,
      74,      75,      76,      77,      78,      79,     164,     165,
      56,      62,      42,      37,      43,      56,     128,     129,
     130,     131,     132,     133,     162,     156,     155,     156,
      91,      67,      66,      67,     143,      31,      28,     136,
     120,      27,     167,     161,     160,     135,     123,      88,
      70,      69,      68,     127,      90,      85,      73,      35,
      34,      32,      29,      48,      45,      13,       8,     138,
      50,      40,     158,     137,      14,       5,       9,     110,
      25,     109,       3,      55,      60,      21,      20,      30,
      17,      16,      15,      12,      11,      10,      26,      36,
       3,       7,       2,       1,       0,      58,      14,      57,
       0,      63,       0,      70,       0,       9,       0,       0,
       0,      65,       0,      30,       0,       0,       0,       0,
       0,       0,      84,       0,       0,       0,      54,      35,
       0,       0,       0,       0,       0,     134,       0,       0,
       0,       0,       0,      40,       0,      94,      98,      99,
     100,     101,     102,     103,       0,       0,       0,       0,
      74,       0,       0,     106,     107,      49,     117,     118,
       0,       0,       0,       0,       0,     122,      75,      76,
      77,      78,      79,     128,       0,      30,      46,       0,
       0,       0,       0,      83,      50,       0,       0,       0,
       0,       0,      90,      45,       0,       0,       0,       0,
       0,     142,       0,       0,     147,     148,     149,     150,
     151,     152,       0,       0,     153,     144,     145,     146,
       0,       0,       0,     159,     154,     161,       0,     129,
     130,     131,     132,     133,     143,      30,       0,      13,
     126,     127,     110,     165,     163,       0,     137,       0,
     166,     168,       0,     138,      48,       8,     104,     105,
       0,       0,       0,       0,       0,      30,      95,      96,
      97,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,      81,      49,
       0,       0,       0,       0,       0,       0,      72,      73,
      46,
};

int yypact[] = {
    -278,   -1000,    -216,    -279,    -138,    -144,    -119,   -1000,
    -216,    -145,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -120,    -261,   -1000,   -1000,    -121,    -122,
    -216,    -163,    -255,    -238,   -1000,    -188,    -189,   -1000,
    -254,    -191,    -168,    -245,    -246,   -1000,   -1000,    -238,
    -146,   -1000,    -128,    -129,   -1000,    -130,    -274,    -123,
    -185,    -218,   -1000,   -1000,   -1000,   -1000,    -186,   -1000,
    -124,   -1000,    -209,    -268,   -1000,    -249,    -131,    -199,
    -125,    -148,    -217,    -241,   -1000,   -1000,    -216,    -186,
    -186,    -186,    -186,    -186,    -186,    -186,    -186,    -186,
    -186,    -186,    -186,    -186,    -214,    -258,    -179,    -197,
   -1000,    -254,    -254,    -227,    -139,   -1000,    -260,   -1000,
   -1000,    -132,    -209,    -209,    -209,    -209,    -209,    -209,
    -218,    -218,   -1000,   -1000,   -1000,    -178,    -281,    -126,
    -171,   -1000,   -1000,   -1000,   -1000,    -211,    -133,    -140,
    -295,   -1000,    -216,   -1000,    -143,    -232,    -232,    -232,
    -186,    -186,    -186,    -186,    -186,    -186,    -245,   -1000,
    -295,    -150,   -1000,   -1000,   -1000,   -1000,    -224,    -186,
   -1000,   -1000,    -134,    -209,    -209,    -209,    -209,    -209,
    -209,    -135,    -152,   -1000,    -295,   -1000,    -166,    -209,
   -1000,    -216,   -1000,   -1000,    -136,    -186,    -236,   -1000,
    -209,   -1000,
};

int yypgo[] = {
       0,     171,     170,     169,     168,     166,     145,     147,
     143,     142,     150,     165,     164,     163,     141,     148,
     162,     161,     160,     140,     158,     157,     139,     156,
     246,     229,     144,     155,     153,     146,     151,
};

int yyr1[] = {
       0,       1,       1,       2,       5,       5,       6,       6,
       7,       7,       8,       8,       8,       3,       9,       9,
      10,      10,      10,      10,       4,       4,      11,      11,
      15,      15,      16,      12,      12,      17,      18,      13,
      13,      14,      14,      14,      20,      21,      21,      23,
      23,      23,      23,      19,      19,      19,      19,      24,
      24,      24,      24,      24,      24,      22,      22,      22,
      25,      25,      25,      26,      26,      26,      26,      26,
      27,      28,      28,      28,      28,      30,      30,      30,
      30,      30,      30,      29,      29,
};

int yyr2[] = {
       2,       2,       1,       3,       8,       7,       3,       1,
       3,       1,       1,       1,       1,       1,       3,       2,
       1,       1,       1,       1,       3,       2,       1,       1,
       4,       4,       4,       1,       1,      10,       6,       6,
       8,       3,       1,       1,       3,       3,       3,       3,
       3,       3,       3,       1,       3,       3,       4,       3,
       3,       3,       3,       3,       3,       3,       3,       1,
       3,       3,       1,       1,       1,       1,       3,       1,
       8,       1,       3,       3,       4,       3,       3,       3,
       3,       3,       3,       3,       1,
};

int yychk[] = {
   -1000,      -1,      -2,      -4,     293,     -15,     286,      -3,
      -9,     -10,     -11,     -12,     -13,     -14,     -15,     -16,
     -17,     -18,     281,     265,     -20,     -21,     286,     283,
     288,     -15,      -5,     259,     262,     257,     -10,     262,
     257,     279,     257,     257,      -9,     262,     294,     259,
      -6,     265,     266,     268,     262,     -19,     -24,     292,
     -22,     -25,     -26,     265,     267,     268,     257,     -27,
     287,     -14,     -22,     266,     -23,     265,     265,     -14,
     289,      -6,     260,     261,     258,     258,     258,     290,
     291,     257,     273,     274,     275,     276,     277,     278,
     269,     270,     271,     272,     -22,     257,     280,     280,
     258,     262,     257,     260,     263,     265,      -9,     -24,
     -24,     -24,     -22,     -22,     -22,     -22,     -22,     -22,
     -25,     -25,     -26,     -26,     258,     -28,     -30,     292,
     264,     265,     266,     265,     266,     -19,     -19,     263,
     259,     282,     285,     258,     261,     290,     291,     257,
     273,     274,     275,     276,     277,     278,     262,     258,
     259,      -7,      -8,     295,     296,     297,      -9,     259,
     -30,     -30,     -30,     -22,     -22,     -22,     -22,     -22,
     -22,     -14,      -7,     260,     261,     282,     -29,     -22,
     258,     258,     260,      -8,     260,     261,      -9,     258,
     -22,     284,
};

int yydef[] = {
       0,      -2,       0,       2,       0,       0,       0,       1,
      13,       0,      16,      17,      18,      19,      22,      23,
      27,      28,       0,       0,      34,      35,       0,       0,
       0,       0,       0,       0,      21,       0,       0,      15,
       0,       0,       0,       0,       0,      20,       3,       0,
       0,       7,       0,       0,      14,       0,      43,       0,
       0,      55,      58,      59,      60,      61,       0,      63,
       0,      33,      36,      37,      38,      59,       0,       0,
       0,       0,       0,       0,      24,      25,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
      26,       0,       0,       0,       0,       6,       0,      44,
      45,       0,      47,      48,      49,      50,      51,      52,
      53,      54,      56,      57,      62,       0,      65,       0,
       0,      41,      42,      39,      40,       0,       0,       0,
       0,      31,       0,      46,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,      30,
       0,       0,       9,      10,      11,      12,       0,       0,
      66,      67,       0,      69,      70,      71,      72,      73,
      74,       0,       0,       5,       0,      32,       0,      76,
      68,       0,       4,       8,       0,       0,       0,      64,
      75,      29,
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
# line 76 "main.y"
      {
      	fprintf(salidaAS,"01 programa: bloque_declaracion bloque_ejecucion\n");
      } break;
      case 2:
# line 82 "main.y"
      {
      	fprintf(salidaAS,"02 programa: lista_wprints_cte\n");
      } break;
      case 3:
# line 88 "main.y"
      {
      	fprintf(salidaAS,"03 bloque_declaracion: PR_VAR declaracion PR_ENDVAR\n");
      } break;
      case 4:
# line 94 "main.y"
      {
      	fprintf(salidaAS,"04 declaracion: declaracion COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA\n");
      	
      	equilibrarDeclaracion();
      	
      	asignarTipoDeDato();
      	
      	cantIDsEnTS += cantIDsEnDeclaracion;
      	cantIDsEnDeclaracion = 0;
      	cantTiposEnDeclaracion = 0;
      } break;
      case 5:
# line 107 "main.y"
      {
      	fprintf(salidaAS,"05 declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA\n");
      	
      	equilibrarDeclaracion();
      	
      	asignarTipoDeDato();
      	
      	cantIDsEnTS += cantIDsEnDeclaracion;
      	cantIDsEnDeclaracion = 0;
      	cantTiposEnDeclaracion = 0;
      } break;
      case 6:
# line 121 "main.y"
      {
      	fprintf(salidaAS,"06 lista_variables: lista_variables COMA ID\n");
      	
      	posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      } break;
      case 7:
# line 128 "main.y"
      {
      	fprintf(salidaAS,"07 lista_variables: ID\n");
      	
      	posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      } break;
      case 8:
# line 136 "main.y"
      {
      	fprintf(salidaAS,"08 lista_tipos: lista_tipos COMA tipo\n");
      	
      	tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      } break;
      case 9:
# line 143 "main.y"
      {
      	fprintf(salidaAS,"09 lista_tipos: tipo\n");
      	
      	tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      } break;
      case 10:
# line 151 "main.y"
      {
      	fprintf(salidaAS,"10 tipo: PR_INT\n");
      	
      	yyval = PR_INT;
      } break;
      case 11:
# line 158 "main.y"
      {
      	fprintf(salidaAS,"11 tipo: PR_FLOAT\n");
      	
      	yyval = PR_FLOAT;
      } break;
      case 12:
# line 165 "main.y"
      {
      	fprintf(salidaAS,"12 tipo: PR_STRING\n");
      	
      	yyval = PR_STRING;
      } break;
      case 13:
# line 173 "main.y"
      {
      	fprintf(salidaAS,"13 bloque_ejecucion: lista_sentencias\n");
      } break;
      case 14:
# line 179 "main.y"
      {
      	fprintf(salidaAS,"14 lista_sentencias: lista_sentencias sentencia PUNTO_COMA\n");
      } break;
      case 15:
# line 184 "main.y"
      {
      	fprintf(salidaAS,"15 lista_sentencias: sentencia PUNTO_COMA\n");
      } break;
      case 16:
# line 190 "main.y"
      {
      	fprintf(salidaAS,"16 sentencia: wprint\n");
      } break;
      case 17:
# line 195 "main.y"
      {
      	fprintf(salidaAS,"17 sentencia: iteracion\n");
      } break;
      case 18:
# line 200 "main.y"
      {
      	fprintf(salidaAS,"18 sentencia: decision\n");
      } break;
      case 19:
# line 205 "main.y"
      {
      	fprintf(salidaAS,"19 sentencia: asignacion\n");
      } break;
      case 20:
# line 211 "main.y"
      {
      	fprintf(salidaAS,"20 lista_wprints_cte: lista_wprints_cte wprint_cte PUNTO_COMA\n");
      } break;
      case 21:
# line 216 "main.y"
      {
      	fprintf(salidaAS,"21 lista_wprints_cte: wprint_cte PUNTO_COMA\n");
      } break;
      case 22:
# line 222 "main.y"
      {
      	fprintf(salidaAS,"22 wprint: wprint_cte\n");
      } break;
      case 23:
# line 227 "main.y"
      {
      	fprintf(salidaAS,"23 wprint: wprint_id\n");
      } break;
      case 24:
# line 233 "main.y"
      {
      	fprintf(salidaAS,"24 wprint_cte: PR_WPRINT PAR_ABRE CTE_STRING PAR_CIERRA\n");
      } break;
      case 25:
# line 238 "main.y"
      {
      	fprintf(salidaAS,"25 wprint_cte: PR_WPRINT PAR_ABRE CTE_REAL PAR_CIERRA\n");
      } break;
      case 26:
# line 244 "main.y"
      {
      	fprintf(salidaAS,"26 wprint_id: PR_WPRINT PAR_ABRE ID PAR_CIERRA\n");
      
	verificarDeclaracion(yypvt[-1]);
      } break;
      case 27:
# line 252 "main.y"
      {
      	fprintf(salidaAS,"27 iteracion: iteracion_for\n");
      } break;
      case 28:
# line 257 "main.y"
      {
      	fprintf(salidaAS,"28 iteracion: iteracion_dowhile\n");
      } break;
      case 29:
# line 263 "main.y"
      {
      	fprintf(salidaAS,"29 iteracion_for: PR_FOR PAR_ABRE asignacion PUNTO_COMA condicion PUNTO_COMA asignacion PAR_CIERRA lista_sentencias PR_ROF\n");
      } break;
      case 30:
# line 269 "main.y"
      {
      	fprintf(salidaAS,"30 iteracion_dowhile: PR_DO lista_sentencias PR_WHILE PAR_ABRE condicion PAR_CIERRA\n");
      } break;
      case 31:
# line 275 "main.y"
      {
      	fprintf(salidaAS,"31 decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_FI\n");
      } break;
      case 32:
# line 280 "main.y"
      {
      	fprintf(salidaAS,"32 decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_ELSE lista_sentencias PR_FI\n");
      } break;
      case 33:
# line 286 "main.y"
      {
      	fprintf(salidaAS,"33 asignacion: ID OP_ASIGNACION asignacion\n");
      
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
      } break;
      case 34:
# line 313 "main.y"
      {
      	fprintf(salidaAS,"34 asignacion: asignacion_num_o_id\n");
      
	yyval = yypvt[-0];
      } break;
      case 35:
# line 320 "main.y"
      {
      	fprintf(salidaAS,"35 asignacion: asignacion_string\n");
      
	yyval = yypvt[-0];
      } break;
      case 36:
# line 328 "main.y"
      {
      	fprintf(salidaAS,"36 asignacion_num_o_id: ID OP_ASIGNACION expresion\n");
      
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
      
	yyval = TS[yypvt[-2]].tipo;
      } break;
      case 37:
# line 358 "main.y"
      {
      	fprintf(salidaAS,"37 asignacion_string: ID OP_ASIGNACION CTE_STRING\n");
      
	verificarDeclaracion(yypvt[-2]);
      
	if(TS[yypvt[-2]].tipo == PR_INT)
      	{
      		lanzarError("No puede asignar un tipo STRING a un tipo INT");
      	}
      
	if(TS[yypvt[-2]].tipo == PR_FLOAT)
      	{
      		lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
      	}
      
	yyval = TS[yypvt[-2]].tipo;
      } break;
      case 38:
# line 377 "main.y"
      {
      	fprintf(salidaAS,"38 asignacion_string: ID OP_ASIGNACION concatenacion\n");
      
	verificarDeclaracion(yypvt[-2]);
      
	if(TS[yypvt[-2]].tipo == PR_INT)
      	{
      		lanzarError("No puede asignar un tipo STRING a un tipo INT");
      	}
      
	if(TS[yypvt[-2]].tipo == PR_FLOAT)
      	{
      		lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
      	}
      
	yyval = TS[yypvt[-2]].tipo;
      } break;
      case 39:
# line 397 "main.y"
      {
      	fprintf(salidaAS,"39 concatenacion: ID OP_CONCATENACION ID\n");
      
	verificarDeclaracion(yypvt[-2]);
      	verificarDeclaracion(yypvt[-0]);
      
	if(TS[yypvt[-2]].tipo != PR_STRING || TS[yypvt[-0]].tipo != PR_STRING)
      	{
      		lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      	}
      
	yyval = PR_STRING;
      } break;
      case 40:
# line 412 "main.y"
      {
      	fprintf(salidaAS,"40 concatenacion: ID OP_CONCATENACION CTE_STRING\n");
      
	verificarDeclaracion(yypvt[-2]);
      
	if(TS[yypvt[-2]].tipo != PR_STRING)
      	{
      		lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      	}
      
	yyval = PR_STRING;
      } break;
      case 41:
# line 426 "main.y"
      {
      	fprintf(salidaAS,"41 concatenacion: CTE_STRING OP_CONCATENACION ID\n");
      
	verificarDeclaracion(yypvt[-0]);
      
	if(TS[yypvt[-0]].tipo != PR_STRING)
      	{
      		lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
      	}
      
	yyval = PR_STRING;
      } break;
      case 42:
# line 440 "main.y"
      {
      	fprintf(salidaAS,"42 concatenacion: CTE_STRING OP_CONCATENACION CTE_STRING\n");
      
	yyval = PR_STRING;
      } break;
      case 43:
# line 448 "main.y"
      {
      	fprintf(salidaAS,"43 condicion: proposicion\n");
      } break;
      case 44:
# line 453 "main.y"
      {
      	fprintf(salidaAS,"44 condicion: proposicion PR_AND proposicion\n");
      } break;
      case 45:
# line 458 "main.y"
      {
      	fprintf(salidaAS,"45 condicion: proposicion PR_OR proposicion\n");
      } break;
      case 46:
# line 463 "main.y"
      {
      	fprintf(salidaAS,"46 condicion: PR_NOT PAR_ABRE proposicion PAR_CIERRA\n");
      } break;
      case 47:
# line 469 "main.y"
      {
      	fprintf(salidaAS,"47 proposicion: expresion OP_MAYOR expresion\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 48:
# line 479 "main.y"
      {
      	fprintf(salidaAS,"48 proposicion: expresion OP_MAYOR_IGUAL expresion\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 49:
# line 489 "main.y"
      {
      	fprintf(salidaAS,"49 proposicion: expresion OP_MENOR expresion\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 50:
# line 499 "main.y"
      {
      	fprintf(salidaAS,"50 proposicion: expresion OP_MENOR_IGUAL expresion\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 51:
# line 509 "main.y"
      {
      	fprintf(salidaAS,"51 proposicion: expresion OP_IGUAL expresion\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 52:
# line 519 "main.y"
      {
      	fprintf(salidaAS,"52 proposicion: expresion OP_DISTINTO expresion\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 53:
# line 530 "main.y"
      {
      	fprintf(salidaAS,"53 expresion: expresion OP_SUMA termino\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una expresion");
      	}
      
	yyval = yypvt[-2];
      } break;
      case 54:
# line 542 "main.y"
      {
      	fprintf(salidaAS,"54 expresion: expresion OP_RESTA termino\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una expresion");
      	}
      
	yyval = yypvt[-2];
      } break;
      case 55:
# line 554 "main.y"
      {
      	fprintf(salidaAS,"55 expresion: termino\n");
      
	yyval = yypvt[-0];
      } break;
      case 56:
# line 562 "main.y"
      {
      	fprintf(salidaAS,"56 termino: termino OP_MULTIPLICACION factor\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una expresion");
      	}
      
	yyval = yypvt[-2];
      } break;
      case 57:
# line 574 "main.y"
      {
      	fprintf(salidaAS,"57 termino: termino OP_DIVISION factor\n");
      
	if(yypvt[-2] == PR_STRING || yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una expresion");
      	}
      
	yyval = yypvt[-2];
      } break;
      case 58:
# line 586 "main.y"
      {
      	fprintf(salidaAS,"58 termino: factor\n");
      
	yyval = yypvt[-0];
      } break;
      case 59:
# line 594 "main.y"
      {
      	fprintf(salidaAS,"59 factor: ID\n");
      
	verificarDeclaracion(yypvt[-0]);
      
	yyval = TS[yypvt[-0]].tipo;
      } break;
      case 60:
# line 603 "main.y"
      {
      	fprintf(salidaAS,"60 factor: CTE_ENTERA\n");
      
	yyval = PR_INT;
      } break;
      case 61:
# line 610 "main.y"
      {
      	fprintf(salidaAS,"61 factor: CTE_REAL\n");
      
	yyval = PR_FLOAT;
      } break;
      case 62:
# line 617 "main.y"
      {
      	fprintf(salidaAS,"62 factor: PAR_ABRE expresion PAR_CIERRA\n");
      
	yyval = yypvt[-1];
      } break;
      case 63:
# line 624 "main.y"
      {
      	fprintf(salidaAS,"63 factor: filterc\n");
      
	yyval = yypvt[-0];
      } break;
      case 64:
# line 632 "main.y"
      {
      	fprintf(salidaAS,"64 filterc: PR_FILTERC PAR_ABRE condicion_f COMA COR_ABRE lista_expresiones COR_CIERRA PAR_CIERRA\n");
      
	yyval = PR_INT;
      } break;
      case 65:
# line 640 "main.y"
      {
      	fprintf(salidaAS,"65 condicion_f: proposicion_f\n");
      } break;
      case 66:
# line 645 "main.y"
      {
      	fprintf(salidaAS,"66 condicion_f: proposicion_f PR_AND proposicion_f\n");
      } break;
      case 67:
# line 650 "main.y"
      {
      	fprintf(salidaAS,"67 condicion_f: proposicion_f PR_OR proposicion_f\n");
      } break;
      case 68:
# line 655 "main.y"
      {
      	fprintf(salidaAS,"68 condicion_f: PR_NOT PAR_ABRE proposicion_f PAR_CIERRA\n");
      } break;
      case 69:
# line 661 "main.y"
      {
      	fprintf(salidaAS,"69 proposicion_f: GUION_BAJO OP_MAYOR expresion\n");
      
	if(yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 70:
# line 671 "main.y"
      {
      	fprintf(salidaAS,"70 proposicion_f: GUION_BAJO OP_MAYOR_IGUAL expresion\n");
      
	if(yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 71:
# line 681 "main.y"
      {
      	fprintf(salidaAS,"71 proposicion_f: GUION_BAJO OP_MENOR expresion\n");
      
	if(yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 72:
# line 691 "main.y"
      {
      	fprintf(salidaAS,"72 proposicion_f: GUION_BAJO OP_MENOR_IGUAL expresion\n");
      
	if(yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 73:
# line 701 "main.y"
      {
      	fprintf(salidaAS,"73 proposicion_f: GUION_BAJO OP_IGUAL expresion\n");
      
	if(yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 74:
# line 711 "main.y"
      {
      	fprintf(salidaAS,"74 proposicion_f: GUION_BAJO OP_DISTINTO expresion\n");
      
	if(yypvt[-0] == PR_STRING)
      	{
      		lanzarError("No se puede usar un tipo STRING en una comparacion");
      	}
      } break;
      case 75:
# line 722 "main.y"
      {
      	fprintf(salidaAS,"75 lista_expresiones: lista_expresiones COMA expresion\n");
      } break;
      case 76:
# line 727 "main.y"
      {
      	fprintf(salidaAS,"76 lista_expresiones: expresion\n");
      } break;    }
    goto enstack;
}
