
# line 2 "miCompi.y"
#include "AnalizadorLexico.h"
#include <stdio.h>

FILE *salidaAS;
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

# line 462 "miCompi.y"



int main()
{
    FILE *archivo;
    int tipoToken;

    fopen_s(&archivo,"fuente.txt","r");
	fopen_s(&salidaAS,"reglas_aplidas.txt","w");
	
    if(archivo == NULL || salidaAS == NULL)
    {
        printf("No se pudo abrir archivos");
    }
    else
    {
        inicializarAL(archivo);

        while(!feof(archivo))
        {
            yyparse();
        }

        fclose(archivo);
		fclose(salidaAS);
    }

    return 0;
}

int yyerror(char *mensaje)
{
	printf("%s\n",mensaje);

	return 1;
}FILE *yytfilep;
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
#define YYLAST 332

int yyact[] = {
     134,     135,     136,      53,      19,      38,     121,     122,
       6,      69,      70,      50,       6,      51,      52,       4,
      19,      84,     109,      33,      18,     117,      23,      19,
     118,      22,      19,      24,      33,      85,      80,      81,
      18,      55,      23,      19,      90,      22,      46,      24,
      63,      19,      18,      40,      23,     163,     108,      22,
      41,      24,      42,      18,     151,      23,     109,      53,
      22,      18,      24,      23,      53,     130,      22,      50,
      24,      51,      52,      87,      60,      58,      51,      52,
     105,      78,      79,      78,      79,      72,      73,      74,
      75,      76,      77,      78,      79,      55,      61,      41,
      89,      42,      55,     124,     125,     126,     127,     128,
     129,     112,     113,     110,     111,     158,     159,     149,
     150,      64,      65,     123,      43,      37,      31,      28,
     120,      27,     138,     116,     161,     155,     154,     131,
     119,      86,      68,      67,      66,      88,      83,      71,
      35,      34,      32,      29,      47,      26,      44,       8,
     133,      49,     152,      13,     106,      48,      54,       9,
      14,       5,      59,      21,      25,      20,       3,      17,
      30,      16,      15,      12,      11,      10,     132,      39,
      36,       3,       7,       2,       1,       0,      57,       0,
       0,       0,      14,       0,      68,      56,       9,      62,
     107,       0,       0,       0,      30,       0,       0,       0,
       0,       0,      82,       0,       0,       0,      53,       0,
       0,       0,       0,       0,      35,       0,       0,       0,
       0,       0,     130,       0,      91,      95,      96,      97,
      98,      99,     100,       0,       0,       0,       0,      72,
       0,       0,     103,     104,     101,     102,     114,     115,
       0,       0,       0,     118,      73,      74,      75,      76,
      77,     124,       0,      30,      45,       0,       0,       0,
       0,      81,      49,      79,      48,       0,       0,       0,
       0,      88,      44,       0,       0,       0,     137,       0,
       0,     142,     143,     144,     145,     146,     147,       0,
       0,       0,       0,       0,       0,       0,     148,     153,
       0,     155,       0,     125,     126,     127,     128,     129,
     138,      30,       0,     156,       0,       0,     159,     157,
      13,       0,      26,     160,     162,       0,     133,      47,
       8,       0,     139,     140,     141,       0,       0,       0,
      30,       0,      92,      93,      94,       0,       0,       0,
       0,       0,       0,       0,     122,     123,     107,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,      70,      71,      45,
};

int yypact[] = {
    -278,   -1000,    -224,    -274,    -146,    -151,    -126,   -1000,
    -224,    -152,   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,
   -1000,   -1000,    -127,    -260,   -1000,   -1000,    -128,    -129,
    -224,    -153,    -289,    -222,   -1000,    -218,    -154,   -1000,
    -254,    -197,    -179,    -242,    -249,   -1000,   -1000,    -155,
   -1000,    -134,    -135,   -1000,    -136,    -281,    -130,    -196,
    -241,   -1000,   -1000,   -1000,   -1000,    -202,   -1000,    -131,
   -1000,    -194,    -263,   -1000,    -251,    -137,    -195,    -132,
    -175,    -229,   -1000,   -1000,    -224,    -202,    -202,    -202,
    -202,    -202,    -202,    -202,    -202,    -202,    -202,    -202,
    -202,    -202,    -186,    -246,    -166,    -168,   -1000,    -254,
    -254,    -144,   -1000,    -261,   -1000,   -1000,    -138,    -194,
    -194,    -194,    -194,    -194,    -194,    -241,    -241,   -1000,
   -1000,   -1000,    -149,    -284,    -150,    -182,   -1000,   -1000,
   -1000,   -1000,    -201,    -139,    -295,   -1000,    -224,   -1000,
    -145,    -210,    -210,    -210,    -202,    -202,    -202,    -202,
    -202,    -202,    -242,   -1000,    -157,   -1000,   -1000,   -1000,
   -1000,    -230,    -202,   -1000,   -1000,    -140,    -194,    -194,
    -194,    -194,    -194,    -194,    -141,    -146,    -295,   -1000,
    -159,    -194,   -1000,    -224,   -1000,   -1000,    -142,    -202,
    -239,   -1000,    -194,   -1000,
};

int yypgo[] = {
       0,     164,     163,     162,     161,     133,     159,     158,
     136,     135,     143,     157,     156,     155,     139,     144,
     154,     153,     151,     134,     149,     147,     132,     146,
     236,     141,     137,     142,     140,     138,     176,
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
     257,     279,     257,     257,      -9,     262,     294,      -6,
     265,     266,     268,     262,     -19,     -24,     292,     -22,
     -25,     -26,     265,     267,     268,     257,     -27,     287,
     -14,     -22,     266,     -23,     265,     265,     -14,     289,
     260,     261,     258,     258,     258,     290,     291,     257,
     273,     274,     275,     276,     277,     278,     269,     270,
     271,     272,     -22,     257,     280,     280,     258,     262,
     257,     263,     265,      -9,     -24,     -24,     -24,     -22,
     -22,     -22,     -22,     -22,     -22,     -25,     -25,     -26,
     -26,     258,     -28,     -30,     292,     264,     265,     266,
     265,     266,     -19,     -19,     259,     282,     285,     258,
     261,     290,     291,     257,     273,     274,     275,     276,
     277,     278,     262,     258,      -7,      -8,     295,     296,
     297,      -9,     259,     -30,     -30,     -30,     -22,     -22,
     -22,     -22,     -22,     -22,     -14,     260,     261,     282,
     -29,     -22,     258,     258,      -5,      -8,     260,     261,
      -9,     258,     -22,     284,
};

int yydef[] = {
       0,      -2,       0,       2,       0,       0,       0,       1,
      13,       0,      16,      17,      18,      19,      22,      23,
      27,      28,       0,       0,      34,      35,       0,       0,
       0,       0,       0,       0,      21,       0,       0,      15,
       0,       0,       0,       0,       0,      20,       3,       0,
       7,       0,       0,      14,       0,      43,       0,       0,
      55,      58,      59,      60,      61,       0,      63,       0,
      33,      36,      37,      38,      59,       0,       0,       0,
       0,       0,      24,      25,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,      26,       0,
       0,       0,       6,       0,      44,      45,       0,      47,
      48,      49,      50,      51,      52,      53,      54,      56,
      57,      62,       0,      65,       0,       0,      41,      42,
      39,      40,       0,       0,       0,      31,       0,      46,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,      30,       0,       9,      10,      11,
      12,       0,       0,      66,      67,       0,      69,      70,
      71,      72,      73,      74,       0,       5,       0,      32,
       0,      76,      68,       0,       4,       8,       0,       0,
       0,      64,      75,      29,
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
# line 54 "miCompi.y"
      {
      	fprintf(salidaAS,"programa: bloque_declaracion bloque_ejecucion\n");
      } break;
      case 2:
# line 60 "miCompi.y"
      {
      	fprintf(salidaAS,salidaAS,"programa: lista_wprints_cte\n");
      } break;
      case 3:
# line 66 "miCompi.y"
      {
      	fprintf(salidaAS,"bloque_declaracion: PR_VAR declaracion PR_ENDVAR\n");
      } break;
      case 4:
# line 72 "miCompi.y"
      {
      	fprintf(salidaAS,"declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA declaracion\n");
      } break;
      case 5:
# line 77 "miCompi.y"
      {
      	fprintf(salidaAS,"declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA\n");
      } break;
      case 6:
# line 83 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_variables: lista_variables COMA ID\n");
      } break;
      case 7:
# line 88 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_variables: ID\n");
      } break;
      case 8:
# line 94 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_tipos: lista_tipos COMA tipo\n");
      } break;
      case 9:
# line 99 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_tipos: tipo\n");
      } break;
      case 10:
# line 105 "miCompi.y"
      {
      	fprintf(salidaAS,"tipo: PR_INT\n");
      } break;
      case 11:
# line 110 "miCompi.y"
      {
      	fprintf(salidaAS,"tipo: PR_FLOAT\n");
      } break;
      case 12:
# line 115 "miCompi.y"
      {
      	fprintf(salidaAS,"tipo: PR_STRING\n");
      } break;
      case 13:
# line 121 "miCompi.y"
      {
      	fprintf(salidaAS,"bloque_ejecucion: lista_sentencias\n");
      } break;
      case 14:
# line 127 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_sentencias: lista_sentencias sentencia PUNTO_COMA\n");
      } break;
      case 15:
# line 132 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_sentencias: sentencia PUNTO_COMA\n");
      } break;
      case 16:
# line 138 "miCompi.y"
      {
      	fprintf(salidaAS,"sentencia: wprint\n");
      } break;
      case 17:
# line 143 "miCompi.y"
      {
      	fprintf(salidaAS,"sentencia: iteracion\n");
      } break;
      case 18:
# line 148 "miCompi.y"
      {
      	fprintf(salidaAS,"sentencia: decision\n");
      } break;
      case 19:
# line 153 "miCompi.y"
      {
      	fprintf(salidaAS,"sentencia: asignacion\n");
      } break;
      case 20:
# line 159 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_wprints_cte: lista_wprints_cte wprint_cte PUNTO_COMA\n");
      } break;
      case 21:
# line 164 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_wprints_cte: wprint_cte PUNTO_COMA\n");
      } break;
      case 22:
# line 170 "miCompi.y"
      {
      	fprintf(salidaAS,"wprint: wprint_cte\n");
      } break;
      case 23:
# line 175 "miCompi.y"
      {
      	fprintf(salidaAS,"wprint: wprint_id\n");
      } break;
      case 24:
# line 181 "miCompi.y"
      {
      	fprintf(salidaAS,"wprint_cte: PR_WPRINT PAR_ABRE CTE_STRING PAR_CIERRA\n");
      } break;
      case 25:
# line 186 "miCompi.y"
      {
      	fprintf(salidaAS,"wprint_cte: PR_WPRINT PAR_ABRE CTE_REAL PAR_CIERRA\n");
      } break;
      case 26:
# line 192 "miCompi.y"
      {
      	fprintf(salidaAS,"wprint_id: PR_WPRINT PAR_ABRE ID PAR_CIERRA\n");
      } break;
      case 27:
# line 198 "miCompi.y"
      {
      	fprintf(salidaAS,"iteracion: iteracion_for\n");
      } break;
      case 28:
# line 203 "miCompi.y"
      {
      	fprintf(salidaAS,"iteracion: iteracion_dowhile\n");
      } break;
      case 29:
# line 209 "miCompi.y"
      {
      	fprintf(salidaAS,"iteracion_for: PR_FOR PAR_ABRE asignacion PUNTO_COMA condicion PUNTO_COMA asignacion PAR_CIERRA lista_sentencias PR_ROF\n");
      } break;
      case 30:
# line 215 "miCompi.y"
      {
      	fprintf(salidaAS,"iteracion_dowhile: PR_DO lista_sentencias PR_WHILE PAR_ABRE condicion PAR_CIERRA\n");
      } break;
      case 31:
# line 221 "miCompi.y"
      {
      	fprintf(salidaAS,"decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_FI\n");
      } break;
      case 32:
# line 226 "miCompi.y"
      {
      	fprintf(salidaAS,"decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_ELSE lista_sentencias PR_FI\n");
      } break;
      case 33:
# line 232 "miCompi.y"
      {
      	fprintf(salidaAS,"asignacion: ID OP_ASIGNACION asignacion\n");
      } break;
      case 34:
# line 237 "miCompi.y"
      {
      	fprintf(salidaAS,"asignacion: asignacion_num_o_id\n");
      } break;
      case 35:
# line 242 "miCompi.y"
      {
      	fprintf(salidaAS,"asignacion: asignacion_string\n");
      } break;
      case 36:
# line 248 "miCompi.y"
      {
      	fprintf(salidaAS,"asignacion_num_o_id: ID OP_ASIGNACION expresion\n");
      } break;
      case 37:
# line 254 "miCompi.y"
      {
      	fprintf(salidaAS,"\n");
      } break;
      case 38:
# line 259 "miCompi.y"
      {
      	fprintf(salidaAS,"asignacion_string: ID OP_ASIGNACION CTE_STRING\n");
      } break;
      case 39:
# line 265 "miCompi.y"
      {
      	fprintf(salidaAS,"concatenacion: ID OP_CONCATENACION ID\n");
      } break;
      case 40:
# line 270 "miCompi.y"
      {
      	fprintf(salidaAS,"concatenacion: ID OP_CONCATENACION CTE_STRING\n");
      } break;
      case 41:
# line 275 "miCompi.y"
      {
      	fprintf(salidaAS,"concatenacion: CTE_STRING OP_CONCATENACION ID\n");
      } break;
      case 42:
# line 280 "miCompi.y"
      {
      	fprintf(salidaAS,"concatenacion: CTE_STRING OP_CONCATENACION CTE_STRING\n");
      } break;
      case 43:
# line 286 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion: proposicion\n");
      } break;
      case 44:
# line 291 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion: proposicion PR_AND proposicion\n");
      } break;
      case 45:
# line 296 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion: proposicion PR_OR proposicion\n");
      } break;
      case 46:
# line 301 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion: PR_NOT PAR_ABRE proposicion PAR_CIERRA\n");
      } break;
      case 47:
# line 307 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion: expresion OP_MAYOR expresion\n");
      } break;
      case 48:
# line 312 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion: expresion OP_MAYOR_IGUAL expresion\n");
      } break;
      case 49:
# line 317 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion: expresion OP_MENOR expresion\n");
      } break;
      case 50:
# line 322 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion: expresion OP_MENOR_IGUAL expresion\n");
      } break;
      case 51:
# line 327 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion: expresion OP_IGUAL expresion\n");
      } break;
      case 52:
# line 332 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion: expresion OP_DISTINTO expresion\n");
      } break;
      case 53:
# line 338 "miCompi.y"
      {
      	fprintf(salidaAS,"expresion: expresion OP_SUMA termino\n");
      } break;
      case 54:
# line 343 "miCompi.y"
      {
      	fprintf(salidaAS,"expresion: expresion OP_RESTA termino\n");
      } break;
      case 55:
# line 348 "miCompi.y"
      {
      	fprintf(salidaAS,"expresion: termino\n");
      } break;
      case 56:
# line 354 "miCompi.y"
      {
      	fprintf(salidaAS,"termino: termino OP_MULTIPLICACION factor\n");
      } break;
      case 57:
# line 359 "miCompi.y"
      {
      	fprintf(salidaAS,"termino: termino OP_DIVISION factor\n");
      } break;
      case 58:
# line 364 "miCompi.y"
      {
      	fprintf(salidaAS,"termino: factor\n");
      } break;
      case 59:
# line 370 "miCompi.y"
      {
      	fprintf(salidaAS,"factor: ID\n");
      } break;
      case 60:
# line 375 "miCompi.y"
      {
      	fprintf(salidaAS,"factor: CTE_ENTERA\n");
      } break;
      case 61:
# line 380 "miCompi.y"
      {
      	fprintf(salidaAS,"factor: CTE_REAL\n");
      } break;
      case 62:
# line 385 "miCompi.y"
      {
      	fprintf(salidaAS,"factor: PAR_ABRE expresion PAR_CIERRA\n");
      } break;
      case 63:
# line 390 "miCompi.y"
      {
      	fprintf(salidaAS,"factor: filterc\n");
      } break;
      case 64:
# line 396 "miCompi.y"
      {
      	fprintf(salidaAS,"filterc: PR_FILTERC PAR_ABRE condicion_f COMA COR_ABRE lista_expresiones COR_CIERRA PAR_CIERRA\n");
      } break;
      case 65:
# line 402 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion_f: proposicion_f\n");
      } break;
      case 66:
# line 407 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion_f: proposicion_f PR_AND proposicion_f\n");
      } break;
      case 67:
# line 412 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion_f: proposicion_f PR_OR proposicion_f\n");
      } break;
      case 68:
# line 417 "miCompi.y"
      {
      	fprintf(salidaAS,"condicion_f: PR_NOT PAR_ABRE proposicion_f PAR_CIERRA\n");
      } break;
      case 69:
# line 423 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MAYOR expresion\n");
      } break;
      case 70:
# line 428 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MAYOR_IGUAL expresion\n");
      } break;
      case 71:
# line 433 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MENOR expresion\n");
      } break;
      case 72:
# line 438 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MENOR_IGUAL expresion\n");
      } break;
      case 73:
# line 443 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_IGUAL expresion\n");
      } break;
      case 74:
# line 448 "miCompi.y"
      {
      	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_DISTINTO expresion\n");
      } break;
      case 75:
# line 454 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_expresiones: lista_expresiones COMA expresion\n");
      } break;
      case 76:
# line 459 "miCompi.y"
      {
      	fprintf(salidaAS,"lista_expresiones: expresion\n");
      } break;    }
    goto enstack;
}
