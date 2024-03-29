/*
 * MAIN.C: main routine for ANSIC parser
 * Verion 1.0
 * By Xing Liu
 * Copyright(c) Abraxas Software Inc. (R), 1988, All rights reserved
 *
 */

#include <stdio.h>

FILE *fopen();
FILE *fin;
int c;
int lineno;

int lexdebug = 0;

main(int argc, char *argv[]) {
    int ttype;

    if (argc < 2) {
        yyerror("Usage: acc <program>");
        exit(1);
    };
  
    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        yyerror("Can't open source program file");
        exit(1);
    };

    c = getc(fin);

    if (yyparse()) {
        yyerror("Error(s) found by the parser");
        exit(1);
    } else {
        fprintf(stdout, "No syntax error was found by the parser\n");
    };

    fclose(fin);
}

yyerror(s) {

    fprintf(stderr, "%s\n", s);
}
