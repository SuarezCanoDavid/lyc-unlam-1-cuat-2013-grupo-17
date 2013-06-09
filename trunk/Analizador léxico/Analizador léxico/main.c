
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

# line 825 "main.y"



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
  -1, 40,
  280, 75,
  -2, 70,
  -1, 42,
  279, 67,
  280, 74,
  -2, 106,
  -1, 96,
  261, 15,
  -2, 18,
  -1, 119,
  274, 129,
  275, 132,
  276, 135,
  277, 138,
  278, 141,
  -2, 126,
  -1, 218,
  261, 19,
  -2, 22,
  0,
};

#define YYNPROD 147
#define YYLAST 402

int yyact[] = {
     219,     220,     221,      48,      77,      25,     119,       7,
       7,     136,     137,      88,      68,      46,      47,       4,
     123,     124,      75,      35,     168,      24,      36,      22,
      25,     169,       7,     167,      23,      92,     166,     165,
      25,      50,     118,     164,      25,      25,     111,      76,
      24,      96,      22,     237,      29,       7,      30,      23,
      24,      97,      22,     119,      24,       7,      22,      23,
      48,       7,     182,      23,      48,      66,      67,     158,
      42,      40,      46,      47,      88,     195,      46,      47,
     105,      64,      65,      64,      65,     126,     127,     128,
     129,     130,     131,      64,      65,     232,      50,     120,
      51,      29,      50,      30,     103,     104,     222,     223,
     163,      62,      56,      33,      28,     135,     134,     231,
     226,     132,     114,     210,     199,     185,      58,     233,
     224,     214,     209,     196,     122,      90,      80,      79,
     150,     121,      89,      74,      72,      61,     112,      10,
     217,      95,      14,      45,      44,     229,     194,     117,
     144,      32,       9,      21,       6,     109,      43,       6,
      15,       5,      21,      39,      26,     193,       3,      37,
     143,     192,     142,     191,     141,     190,     140,     189,
      10,     139,     188,     138,     162,     161,      69,     228,
      91,     212,     200,      15,     160,     116,     106,      70,
      49,      69,      85,      84,      83,      82,     156,     155,
     154,     153,     152,     151,     175,     125,     149,     148,
      73,      86,      41,      55,      87,      38,      20,      63,
     147,      32,      81,     110,      93,      54,      34,      19,
     146,      32,     108,      53,      63,     230,     151,     208,
     100,     101,     145,     102,     147,      91,     107,      52,
     113,      98,      99,      18,      17,      31,      71,      60,
      85,      45,      59,      44,      16,      13,     208,      12,
      11,      83,      43,     159,     236,     227,     218,     159,
     115,     211,     183,     133,      78,     216,     198,     157,
      94,      57,      27,       3,       8,       2,       1,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,     176,     177,
     178,     179,     180,     181,       0,       0,     172,     170,
     171,     184,       0,       0,       0,       0,       0,       0,
       0,     186,     187,       0,      32,       0,       0,     152,
     153,     154,     155,     156,     189,       0,       0,     230,
     146,     109,      95,       0,     202,     203,     204,     205,
     206,     207,     162,     188,     201,       0,       0,     213,
       0,     190,     191,     192,     193,     194,     200,       0,
       0,     117,       0,     215,     229,       0,       0,       0,
       0,     225,      14,       0,       0,       0,       0,       0,
     173,     174,     236,       0,     234,       0,       0,       0,
       0,     112,       0,      32,       0,     238,       0,     217,
       0,     235,       0,       0,       0,       0,       9,       0,
       0,     149,     175,     197,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,     110,
};

int yypact[] = {
    -278,   -1000,    -229,    -279,   -1000,    -162,    -222,   -1000,
   -1000,    -229,    -163,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,    -263,    -201,    -177,   -1000,   -1000,
   -1000,   -1000,    -164,    -149,   -1000,   -1000,   -1000,    -132,
    -165,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -194,
   -1000,   -1000,   -1000,    -210,    -268,   -1000,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,    -133,    -229,    -134,    -261,
   -1000,    -255,   -1000,    -139,    -140,   -1000,   -1000,    -229,
   -1000,   -1000,   -1000,   -1000,   -1000,    -197,    -135,    -141,
   -1000,    -260,   -1000,   -1000,   -1000,   -1000,    -224,   -1000,
   -1000,    -233,    -197,    -197,    -197,    -197,    -173,    -186,
   -1000,   -1000,   -1000,    -228,   -1000,    -254,    -224,    -154,
   -1000,   -1000,    -210,    -210,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -258,    -175,    -136,    -142,    -274,   -1000,
    -196,    -155,   -1000,    -159,    -160,    -281,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -137,   -1000,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,    -200,   -1000,   -1000,
   -1000,   -1000,    -161,    -238,    -243,    -245,    -249,    -257,
    -253,    -254,    -254,    -229,    -197,    -197,   -1000,    -197,
    -197,    -197,    -197,    -197,    -197,    -205,   -1000,    -224,
    -150,    -213,    -213,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -193,    -143,    -229,   -1000,   -1000,    -197,
    -194,    -194,    -194,    -194,    -194,    -194,   -1000,    -151,
   -1000,   -1000,   -1000,   -1000,    -213,    -197,    -197,    -197,
    -197,    -197,    -197,   -1000,   -1000,    -144,    -152,   -1000,
    -197,    -145,    -194,    -194,    -194,    -194,    -194,    -194,
    -228,   -1000,   -1000,    -295,    -166,    -194,   -1000,    -146,
    -295,    -156,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,    -157,   -1000,    -176,    -147,    -197,    -229,   -1000,
   -1000,   -1000,    -194,    -241,    -295,   -1000,   -1000,
};

int yypgo[] = {
       0,     262,     261,     260,     259,     258,     257,     256,
     129,     255,     254,     253,     128,     252,     251,     250,
     249,     248,     247,     246,     245,     244,     138,     127,
     240,     239,     237,     130,     144,     236,     139,     234,
     231,     230,     229,     228,     227,     223,     221,     218,
     141,     215,     213,     211,     210,     208,     207,     206,
     205,     204,     200,     199,     198,     197,     195,     126,
     194,     132,     193,     203,     191,     190,     189,     188,
     187,     186,     185,     184,     183,     182,     181,     142,
     180,     179,     131,     178,     177,     176,     175,     174,
     173,     172,     170,     169,     167,     135,     165,     164,
     163,     162,     161,     159,     158,     157,     156,     155,
     154,     153,     152,     149,     136,     134,     133,
};

int yyr1[] = {
       0,       1,       1,       5,       2,       7,       9,      10,
      11,       6,      13,      14,      15,      16,       6,      17,
      18,       8,       8,      20,      21,      12,      12,      19,
      19,      19,       3,      22,      22,      23,      23,      23,
      23,       4,       4,      24,      24,      31,      28,      32,
      28,      33,      29,      34,      30,      25,      25,      37,
      38,      39,      41,      42,      35,      43,      44,      45,
      36,      26,      48,      49,      50,      46,      47,      51,
      47,      27,      27,      54,      52,      53,      53,      53,
      58,      56,      57,      57,      40,      60,      40,      61,
      40,      62,      63,      40,      64,      59,      65,      59,
      66,      59,      67,      59,      68,      59,      69,      59,
      70,      55,      72,      55,      55,      73,      71,      75,
      71,      71,      74,      74,      74,      76,      74,      74,
      78,      79,      81,      82,      84,      77,      80,      86,
      80,      87,      80,      88,      89,      80,      90,      91,
      85,      92,      93,      85,      94,      95,      85,      96,
      97,      85,      98,      99,      85,     100,     101,      85,
     102,      83,      83,
};

int yyr2[] = {
       2,       2,       1,       0,       4,       0,       0,       0,
       0,      12,       0,       0,       0,       0,      11,       0,
       0,       5,       1,       0,       0,       5,       1,       1,
       1,       1,       1,       3,       2,       1,       1,       1,
       1,       3,       2,       1,       1,       0,       4,       0,
       4,       0,       4,       0,       3,       1,       1,       0,
       0,       0,       0,       0,      15,       0,       0,       0,
       9,       2,       0,       0,       0,       8,       1,       0,
       4,       2,       2,       0,       3,       1,       1,       1,
       0,       4,       1,       1,       1,       0,       4,       0,
       4,       0,       0,       6,       0,       4,       0,       4,
       0,       4,       0,       4,       0,       4,       0,       4,
       0,       4,       0,       4,       1,       0,       4,       0,
       4,       1,       1,       1,       1,       0,       4,       1,
       0,       0,       0,       0,       0,      13,       1,       0,
       4,       0,       4,       0,       0,       6,       0,       0,
       5,       0,       0,       5,       0,       0,       5,       0,
       0,       5,       0,       0,       5,       0,       0,       5,
       0,       4,       1,
};

int yychk[] = {
   -1000,      -1,      -2,      -4,     293,     -28,     -30,     286,
      -3,     -22,     -23,     -24,     -25,     -26,     -27,     -28,
     -29,     -35,     -36,     -46,     -52,     -30,     283,     288,
     281,     265,     -28,      -5,     262,     266,     268,     -34,
     -23,     262,     -47,     282,     285,     -27,     -53,     -55,
     266,     -56,     265,     -71,     -57,     -74,     267,     268,
     257,     -77,     287,     265,     -37,     -43,     -48,     -54,
     262,      -6,     259,     -31,     -32,     257,     262,     -51,
     269,     270,     271,     272,     280,     -76,     -78,     -33,
     257,     -22,     257,     279,     294,     259,     -13,     258,
     258,     -22,     -70,     -72,     -73,     -75,     -58,     -55,
     265,     257,     258,     -38,     289,     -49,      -7,      -8,
     265,     282,     -71,     -71,     -74,     -74,     -57,     265,
     266,     258,     -79,     -27,     -44,     -40,     -59,     292,
     -55,      -8,     260,     -17,     -80,     -85,     292,     264,
     262,     257,     258,     290,     291,     -62,     273,     274,
     275,     276,     277,     278,     260,     -14,     261,     261,
     290,     291,     -88,     -90,     -92,     -94,     -96,     -98,
    -100,     -39,     -45,     -50,     -60,     -61,     257,     -64,
     -65,     -66,     -67,     -68,     -69,      -9,     263,     -18,
     -81,     -86,     -87,     257,     273,     274,     275,     276,
     277,     278,     -40,     -40,     -22,     -59,     -59,     -63,
     -55,     -55,     -55,     -55,     -55,     -55,     263,     -15,
      -8,     259,     -85,     -85,     -89,     -91,     -93,     -95,
     -97,     -99,    -101,     262,     258,     -59,     -10,     259,
     -82,     -85,     -55,     -55,     -55,     -55,     -55,     -55,
     -41,     258,     259,     -16,     -83,     -55,     258,     -27,
     -11,     -12,     -19,     295,     296,     297,     260,     261,
     258,     -12,     260,     -20,     -84,    -102,     -42,     260,
     261,     258,     -55,     -22,     -21,     284,     -12,
};

int yydef[] = {
       0,      -2,       0,       2,       3,       0,       0,      43,
       1,      26,       0,      29,      30,      31,      32,      35,
      36,      45,      46,       0,       0,       0,      47,      53,
      58,      67,       0,       0,      34,      37,      39,       0,
       0,      28,      57,      62,      63,      65,      66,      69,
      -2,      71,      -2,     100,       0,     105,     107,     108,
     109,     111,     112,      41,       0,       0,       0,       0,
      33,       0,      10,       0,       0,      44,      27,       0,
      96,      98,     101,     103,      72,       0,       0,       0,
      48,       0,      59,      68,       4,       5,       0,      38,
      40,       0,       0,       0,       0,       0,       0,       0,
     106,     113,      42,       0,      54,       0,       0,       0,
      -2,      64,      97,      99,     102,     104,      73,      74,
      75,     110,       0,       0,       0,       0,      76,      81,
       0,       0,      11,       0,       0,     118,     123,      -2,
      49,      55,      60,      77,      79,       0,      84,      86,
      88,      90,      92,      94,       6,       0,      16,     114,
     119,     121,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,      82,       0,
       0,       0,       0,       0,       0,       0,      12,       0,
       0,       0,       0,     124,     127,     130,     133,     136,
     139,     142,       0,       0,      61,      78,      80,       0,
      85,      87,      89,      91,      93,      95,       7,       0,
      17,     115,     120,     122,       0,       0,       0,       0,
       0,       0,       0,      50,      56,       0,       0,      13,
       0,       0,     128,     131,     134,     137,     140,     143,
       0,      83,       8,       0,       0,     146,     125,       0,
       0,       0,      -2,      23,      24,      25,     116,     144,
      51,       0,      14,       0,       0,       0,       0,       9,
      20,     117,     145,       0,       0,      52,      21,
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
      
      case 3:
# line 81 "main.y"
      {
      						fprintf(salidaAS,"VAR\n");
      					} break;
      case 4:
# line 88 "main.y"
      {
      						fprintf(salidaAS,"ENDVAR\n");
      					} break;
      case 5:
# line 96 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 6:
# line 103 "main.y"
      {
      					fprintf(salidaAS,"]");
      				} break;
      case 7:
# line 108 "main.y"
      {
      					fprintf(salidaAS,":");
      				} break;
      case 8:
# line 113 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 9:
# line 120 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 10:
# line 133 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 11:
# line 140 "main.y"
      {
      					fprintf(salidaAS,"]");
      				} break;
      case 12:
# line 145 "main.y"
      {
      					fprintf(salidaAS,":");
      				} break;
      case 13:
# line 150 "main.y"
      {
      					fprintf(salidaAS,"[");
      				} break;
      case 14:
# line 157 "main.y"
      {
      					fprintf(salidaAS,"]\n");
      	
      					equilibrarDeclaracion();
      	
      					asignarTipoDeDato();
      	
      					cantIDsEnTS += cantIDsEnDeclaracion;
      					cantIDsEnDeclaracion = 0;
      					cantTiposEnDeclaracion = 0;
      				} break;
      case 15:
# line 171 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      	
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 16:
# line 178 "main.y"
      {
      						fprintf(salidaAS,",");
      					} break;
      case 18:
# line 185 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      	
      						posicionIDEnTS[cantIDsEnDeclaracion++] = yypvt[-0];
      					} break;
      case 19:
# line 193 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 20:
# line 198 "main.y"
      {
      					fprintf(salidaAS,",");
      				} break;
      case 22:
# line 205 "main.y"
      {
      					tipoDeID[cantTiposEnDeclaracion++] = yypvt[-0];
      				} break;
      case 23:
# line 211 "main.y"
      {
      			fprintf(salidaAS,"int");
      	
      			yyval = PR_INT;
      		} break;
      case 24:
# line 218 "main.y"
      {
      			fprintf(salidaAS,"float");
      	
      			yyval = PR_FLOAT;
      		} break;
      case 25:
# line 225 "main.y"
      {
      			fprintf(salidaAS,"string");
      	
      			yyval = PR_STRING;
      		} break;
      case 27:
# line 240 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 28:
# line 247 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 33:
# line 266 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 34:
# line 273 "main.y"
      {
      						fprintf(salidaAS,";\n");
      					} break;
      case 37:
# line 286 "main.y"
      {
      				fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      			} break;
      case 38:
# line 291 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 39:
# line 298 "main.y"
      {
      				fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			} break;
      case 40:
# line 303 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 41:
# line 311 "main.y"
      {
      				fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      			} break;
      case 42:
# line 316 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 43:
# line 321 "main.y"
      {
      					fprintf(salidaAS,"WPRINT");
      				} break;
      case 44:
# line 326 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 47:
# line 337 "main.y"
      {
      					fprintf(salidaAS,"FOR");
      				} break;
      case 48:
# line 342 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 49:
# line 349 "main.y"
      {
      					fprintf(salidaAS,";");
      				} break;
      case 50:
# line 356 "main.y"
      {
      					fprintf(salidaAS,";");
      				} break;
      case 51:
# line 363 "main.y"
      {
      					fprintf(salidaAS,")\n");
      				} break;
      case 52:
# line 370 "main.y"
      {
      					fprintf(salidaAS,"ROF");
      				} break;
      case 53:
# line 376 "main.y"
      {
      						fprintf(salidaAS,"DO\n");
      					} break;
      case 54:
# line 383 "main.y"
      {
      						fprintf(salidaAS,"WHILE");
      					} break;
      case 55:
# line 388 "main.y"
      {
      						fprintf(salidaAS,"(");
      					} break;
      case 56:
# line 395 "main.y"
      {
      						fprintf(salidaAS,")");
      					} break;
      case 58:
# line 403 "main.y"
      {
      						fprintf(salidaAS,"IF");
      					} break;
      case 59:
# line 408 "main.y"
      {
      						fprintf(salidaAS,"(");
      					} break;
      case 60:
# line 415 "main.y"
      {
      						fprintf(salidaAS,")\n");
      					} break;
      case 62:
# line 422 "main.y"
      {
      						fprintf(salidaAS,"FI");
      					} break;
      case 63:
# line 427 "main.y"
      {
      						fprintf(salidaAS,"ELSE\n");
      					} break;
      case 64:
# line 434 "main.y"
      {
      						fprintf(salidaAS,"FI");
      					} break;
      case 67:
# line 443 "main.y"
      {
      						fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      					} break;
      case 68:
# line 448 "main.y"
      {
      						fprintf(salidaAS," = ");
      					} break;
      case 69:
# line 453 "main.y"
      {
      						fprintf(salidaAS,"(=%d)",yypvt[-0]);
      					} break;
      case 70:
# line 458 "main.y"
      {
      						fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      					} break;
      case 72:
# line 468 "main.y"
      {
      					fprintf(salidaAS," ++ "); 
      				} break;
      case 74:
# line 475 "main.y"
      {
      									fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      								} break;
      case 75:
# line 480 "main.y"
      {
      									fprintf(salidaAS,"\"%s\"",TS[yypvt[-0]].valor);
      								} break;
      case 77:
# line 490 "main.y"
      {
      				fprintf(salidaAS," AND ");
      			} break;
      case 79:
# line 499 "main.y"
      {
      				fprintf(salidaAS," OR ");
      			} break;
      case 81:
# line 506 "main.y"
      {
      				fprintf(salidaAS," NOT ");
      			} break;
      case 82:
# line 511 "main.y"
      {
      				fprintf(salidaAS,"(");
      			} break;
      case 83:
# line 518 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 84:
# line 526 "main.y"
      {
      					fprintf(salidaAS," > ");
      				} break;
      case 86:
# line 535 "main.y"
      {
      					fprintf(salidaAS," >= ");
      				} break;
      case 88:
# line 544 "main.y"
      {
      					fprintf(salidaAS," < ");
      				} break;
      case 90:
# line 553 "main.y"
      {
      					fprintf(salidaAS," <= ");
      				} break;
      case 92:
# line 562 "main.y"
      {
      					fprintf(salidaAS," == ");
      				} break;
      case 94:
# line 571 "main.y"
      {
      					fprintf(salidaAS," != ");
      				} break;
      case 96:
# line 581 "main.y"
      {
      				fprintf(salidaAS," + ");
      			} break;
      case 97:
# line 586 "main.y"
      {
      				yyval = yypvt[-3] + yypvt[-0];
      			} break;
      case 98:
# line 593 "main.y"
      {
      				fprintf(salidaAS," - ");
      			} break;
      case 99:
# line 598 "main.y"
      {
      				yyval = yypvt[-3] - yypvt[-0];
      			} break;
      case 101:
# line 608 "main.y"
      {
      				fprintf(salidaAS," * ");
      			} break;
      case 102:
# line 613 "main.y"
      {
      				yyval = yypvt[-3] * yypvt[-0];
      			} break;
      case 103:
# line 620 "main.y"
      {
      				fprintf(salidaAS," / ");
      			} break;
      case 104:
# line 625 "main.y"
      {
      				yyval = yypvt[-3] / yypvt[-0];
      			} break;
      case 105:
# line 630 "main.y"
      {
      				yyval = yypvt[-0];
      			} break;
      case 106:
# line 636 "main.y"
      {
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].nombre);
      		} break;
      case 107:
# line 641 "main.y"
      {
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			yyval = atoi(TS[yypvt[-0]].valor);
      		} break;
      case 108:
# line 647 "main.y"
      {	
      			fprintf(salidaAS,"%s",TS[yypvt[-0]].valor);
      			yyval = atoi(TS[yypvt[-0]].valor);
      		} break;
      case 109:
# line 653 "main.y"
      {
      			fprintf(salidaAS,"(");
      		} break;
      case 110:
# line 660 "main.y"
      {
      			fprintf(salidaAS,")");
      			yyval = yypvt[-1];
      		} break;
      case 112:
# line 669 "main.y"
      {
      				fprintf(salidaAS,"FILTERC");
      			} break;
      case 113:
# line 674 "main.y"
      {
      				fprintf(salidaAS,"(");
      			} break;
      case 114:
# line 681 "main.y"
      {
      				fprintf(salidaAS,",");
      			} break;
      case 115:
# line 686 "main.y"
      {
      				fprintf(salidaAS,"[");
      			} break;
      case 116:
# line 693 "main.y"
      {
      				fprintf(salidaAS,"]");
      			} break;
      case 117:
# line 698 "main.y"
      {
      				fprintf(salidaAS,")");
      			} break;
      case 119:
# line 708 "main.y"
      {
      					fprintf(salidaAS," AND ");
      				} break;
      case 121:
# line 717 "main.y"
      {
      					fprintf(salidaAS," OR ");
      				} break;
      case 123:
# line 724 "main.y"
      {
      					fprintf(salidaAS,"NOT");
      				} break;
      case 124:
# line 729 "main.y"
      {
      					fprintf(salidaAS,"(");
      				} break;
      case 125:
# line 736 "main.y"
      {
      					fprintf(salidaAS,")");
      				} break;
      case 126:
# line 742 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 127:
# line 747 "main.y"
      {
      					fprintf(salidaAS," > ");
      				} break;
      case 129:
# line 754 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 130:
# line 759 "main.y"
      {
      					fprintf(salidaAS," >= ");
      				} break;
      case 132:
# line 766 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 133:
# line 771 "main.y"
      {
      					fprintf(salidaAS," < ");
      				} break;
      case 135:
# line 778 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 136:
# line 783 "main.y"
      {
      					fprintf(salidaAS," <= ");
      				} break;
      case 138:
# line 790 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 139:
# line 795 "main.y"
      {
      					fprintf(salidaAS," == ");
      				} break;
      case 141:
# line 802 "main.y"
      {
      					fprintf(salidaAS,"_");
      				} break;
      case 142:
# line 807 "main.y"
      {
      					fprintf(salidaAS," != ");
      				} break;
      case 144:
# line 817 "main.y"
      {
      						fprintf(salidaAS,",");
      					} break;    }
    goto enstack;
}
