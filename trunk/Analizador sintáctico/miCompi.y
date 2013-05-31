%{
#include "AnalizadorLexico.h"
#include <stdio.h>

FILE *salidaAS;
%}

%token PAR_ABRE
%token PAR_CIERRA
%token COR_ABRE
%token COR_CIERRA
%token COMA
%token PUNTO_COMA
%token DOS_PUNTOS
%token GUION_BAJO
%token ID
%token CTE_STRING
%token CTE_ENTERA
%token CTE_REAL
%token OP_SUMA
%token OP_RESTA
%token OP_MULTIPLICACION
%token OP_DIVISION
%token OP_MAYOR
%token OP_MAYOR_IGUAL
%token OP_MENOR
%token OP_MENOR_IGUAL
%token OP_IGUAL
%token OP_DISTINTO
%token OP_ASIGNACION
%token OP_CONCATENACION
%token PR_IF
%token PR_FI
%token PR_FOR
%token PR_ROF
%token PR_ELSE
%token PR_WPRINT
%token PR_FILTERC
%token PR_DO
%token PR_WHILE
%token PR_AND
%token PR_OR
%token PR_NOT
%token PR_VAR
%token PR_ENDVAR
%token PR_INT
%token PR_FLOAT
%token PR_STRING

%start programa

%%
programa: bloque_declaracion bloque_ejecucion
{
	fprintf(salidaAS,"programa: bloque_declaracion bloque_ejecucion\n");
};


programa: lista_wprints_cte
{
	fprintf(salidaAS,salidaAS,"programa: lista_wprints_cte\n");
};


bloque_declaracion: PR_VAR declaracion PR_ENDVAR
{
	fprintf(salidaAS,"bloque_declaracion: PR_VAR declaracion PR_ENDVAR\n");
};


declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA declaracion
{
	fprintf(salidaAS,"declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA declaracion\n");
};

declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA
{
	fprintf(salidaAS,"declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA\n");
};


lista_variables: lista_variables COMA ID
{
	fprintf(salidaAS,"lista_variables: lista_variables COMA ID\n");
};

lista_variables: ID
{
	fprintf(salidaAS,"lista_variables: ID\n");
};


lista_tipos: lista_tipos COMA tipo
{
	fprintf(salidaAS,"lista_tipos: lista_tipos COMA tipo\n");
};

lista_tipos: tipo
{
	fprintf(salidaAS,"lista_tipos: tipo\n");
};


tipo: PR_INT
{
	fprintf(salidaAS,"tipo: PR_INT\n");
};

tipo: PR_FLOAT
{
	fprintf(salidaAS,"tipo: PR_FLOAT\n");
};

tipo: PR_STRING
{
	fprintf(salidaAS,"tipo: PR_STRING\n");
};


bloque_ejecucion: lista_sentencias
{
	fprintf(salidaAS,"bloque_ejecucion: lista_sentencias\n");
};


lista_sentencias: lista_sentencias sentencia PUNTO_COMA
{
	fprintf(salidaAS,"lista_sentencias: lista_sentencias sentencia PUNTO_COMA\n");
};

lista_sentencias: sentencia PUNTO_COMA
{
	fprintf(salidaAS,"lista_sentencias: sentencia PUNTO_COMA\n");
};


sentencia: wprint
{
	fprintf(salidaAS,"sentencia: wprint\n");
};

sentencia: iteracion
{
	fprintf(salidaAS,"sentencia: iteracion\n");
};

sentencia: decision
{
	fprintf(salidaAS,"sentencia: decision\n");
};

sentencia: asignacion
{
	fprintf(salidaAS,"sentencia: asignacion\n");
};


lista_wprints_cte: lista_wprints_cte wprint_cte PUNTO_COMA
{
	fprintf(salidaAS,"lista_wprints_cte: lista_wprints_cte wprint_cte PUNTO_COMA\n");
};

lista_wprints_cte: wprint_cte PUNTO_COMA
{
	fprintf(salidaAS,"lista_wprints_cte: wprint_cte PUNTO_COMA\n");
};


wprint: wprint_cte
{
	fprintf(salidaAS,"wprint: wprint_cte\n");
};

wprint: wprint_id
{
	fprintf(salidaAS,"wprint: wprint_id\n");
};


wprint_cte: PR_WPRINT PAR_ABRE CTE_STRING PAR_CIERRA
{
	fprintf(salidaAS,"wprint_cte: PR_WPRINT PAR_ABRE CTE_STRING PAR_CIERRA\n");
};

wprint_cte: PR_WPRINT PAR_ABRE CTE_REAL PAR_CIERRA
{
	fprintf(salidaAS,"wprint_cte: PR_WPRINT PAR_ABRE CTE_REAL PAR_CIERRA\n");
};


wprint_id: PR_WPRINT PAR_ABRE ID PAR_CIERRA
{
	fprintf(salidaAS,"wprint_id: PR_WPRINT PAR_ABRE ID PAR_CIERRA\n");
};


iteracion: iteracion_for
{
	fprintf(salidaAS,"iteracion: iteracion_for\n");
};

iteracion: iteracion_dowhile
{
	fprintf(salidaAS,"iteracion: iteracion_dowhile\n");
};


iteracion_for: PR_FOR PAR_ABRE asignacion PUNTO_COMA condicion PUNTO_COMA asignacion PAR_CIERRA lista_sentencias PR_ROF
{
	fprintf(salidaAS,"iteracion_for: PR_FOR PAR_ABRE asignacion PUNTO_COMA condicion PUNTO_COMA asignacion PAR_CIERRA lista_sentencias PR_ROF\n");
};


iteracion_dowhile: PR_DO lista_sentencias PR_WHILE PAR_ABRE condicion PAR_CIERRA
{
	fprintf(salidaAS,"iteracion_dowhile: PR_DO lista_sentencias PR_WHILE PAR_ABRE condicion PAR_CIERRA\n");
};


decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_FI
{
	fprintf(salidaAS,"decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_FI\n");
};

decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_ELSE lista_sentencias PR_FI
{
	fprintf(salidaAS,"decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_ELSE lista_sentencias PR_FI\n");
};


asignacion: ID OP_ASIGNACION asignacion
{
	fprintf(salidaAS,"asignacion: ID OP_ASIGNACION asignacion\n");
};

asignacion: asignacion_num_o_id
{
	fprintf(salidaAS,"asignacion: asignacion_num_o_id\n");
};

asignacion: asignacion_string
{
	fprintf(salidaAS,"asignacion: asignacion_string\n");
};


asignacion_num_o_id: ID OP_ASIGNACION expresion
{
	fprintf(salidaAS,"asignacion_num_o_id: ID OP_ASIGNACION expresion\n");
};


asignacion_string: ID OP_ASIGNACION CTE_STRING
{
	fprintf(salidaAS,"\n");
};

asignacion_string: ID OP_ASIGNACION concatenacion
{
	fprintf(salidaAS,"asignacion_string: ID OP_ASIGNACION CTE_STRING\n");
};


concatenacion: ID OP_CONCATENACION ID
{
	fprintf(salidaAS,"concatenacion: ID OP_CONCATENACION ID\n");
};

concatenacion: ID OP_CONCATENACION CTE_STRING
{
	fprintf(salidaAS,"concatenacion: ID OP_CONCATENACION CTE_STRING\n");
};

concatenacion: CTE_STRING OP_CONCATENACION ID
{
	fprintf(salidaAS,"concatenacion: CTE_STRING OP_CONCATENACION ID\n");
};

concatenacion: CTE_STRING OP_CONCATENACION CTE_STRING
{
	fprintf(salidaAS,"concatenacion: CTE_STRING OP_CONCATENACION CTE_STRING\n");
};


condicion: proposicion
{
	fprintf(salidaAS,"condicion: proposicion\n");
};

condicion: proposicion PR_AND proposicion
{
	fprintf(salidaAS,"condicion: proposicion PR_AND proposicion\n");
};

condicion: proposicion PR_OR proposicion
{
	fprintf(salidaAS,"condicion: proposicion PR_OR proposicion\n");
};

condicion: PR_NOT PAR_ABRE proposicion PAR_CIERRA
{
	fprintf(salidaAS,"condicion: PR_NOT PAR_ABRE proposicion PAR_CIERRA\n");
};


proposicion: expresion OP_MAYOR expresion
{
	fprintf(salidaAS,"proposicion: expresion OP_MAYOR expresion\n");
};

proposicion: expresion OP_MAYOR_IGUAL expresion
{
	fprintf(salidaAS,"proposicion: expresion OP_MAYOR_IGUAL expresion\n");
};

proposicion: expresion OP_MENOR expresion
{
	fprintf(salidaAS,"proposicion: expresion OP_MENOR expresion\n");
};

proposicion: expresion OP_MENOR_IGUAL expresion
{
	fprintf(salidaAS,"proposicion: expresion OP_MENOR_IGUAL expresion\n");
};

proposicion: expresion OP_IGUAL expresion
{
	fprintf(salidaAS,"proposicion: expresion OP_IGUAL expresion\n");
};

proposicion: expresion OP_DISTINTO expresion
{
	fprintf(salidaAS,"proposicion: expresion OP_DISTINTO expresion\n");
};


expresion: expresion OP_SUMA termino
{
	fprintf(salidaAS,"expresion: expresion OP_SUMA termino\n");
};

expresion: expresion OP_RESTA termino
{
	fprintf(salidaAS,"expresion: expresion OP_RESTA termino\n");
};

expresion: termino
{
	fprintf(salidaAS,"expresion: termino\n");
};


termino: termino OP_MULTIPLICACION factor
{
	fprintf(salidaAS,"termino: termino OP_MULTIPLICACION factor\n");
};

termino: termino OP_DIVISION factor
{
	fprintf(salidaAS,"termino: termino OP_DIVISION factor\n");
};

termino: factor
{
	fprintf(salidaAS,"termino: factor\n");
};


factor: ID
{
	fprintf(salidaAS,"factor: ID\n");
};

factor: CTE_ENTERA
{
	fprintf(salidaAS,"factor: CTE_ENTERA\n");
};

factor: CTE_REAL
{
	fprintf(salidaAS,"factor: CTE_REAL\n");
};

factor: PAR_ABRE expresion PAR_CIERRA
{
	fprintf(salidaAS,"factor: PAR_ABRE expresion PAR_CIERRA\n");
};

factor: filterc
{
	fprintf(salidaAS,"factor: filterc\n");
};


filterc: PR_FILTERC PAR_ABRE condicion_f COMA COR_ABRE lista_expresiones COR_CIERRA PAR_CIERRA
{
	fprintf(salidaAS,"filterc: PR_FILTERC PAR_ABRE condicion_f COMA COR_ABRE lista_expresiones COR_CIERRA PAR_CIERRA\n");
};


condicion_f: proposicion_f
{
	fprintf(salidaAS,"condicion_f: proposicion_f\n");
};

condicion_f: proposicion_f PR_AND proposicion_f
{
	fprintf(salidaAS,"condicion_f: proposicion_f PR_AND proposicion_f\n");
};

condicion_f: proposicion_f PR_OR proposicion_f
{
	fprintf(salidaAS,"condicion_f: proposicion_f PR_OR proposicion_f\n");
};

condicion_f: PR_NOT PAR_ABRE proposicion_f PAR_CIERRA
{
	fprintf(salidaAS,"condicion_f: PR_NOT PAR_ABRE proposicion_f PAR_CIERRA\n");
};


proposicion_f: GUION_BAJO OP_MAYOR expresion
{
	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MAYOR expresion\n");
};

proposicion_f: GUION_BAJO OP_MAYOR_IGUAL expresion
{
	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MAYOR_IGUAL expresion\n");
};

proposicion_f: GUION_BAJO OP_MENOR expresion
{
	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MENOR expresion\n");
};

proposicion_f: GUION_BAJO OP_MENOR_IGUAL expresion
{
	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_MENOR_IGUAL expresion\n");
};

proposicion_f: GUION_BAJO OP_IGUAL expresion
{
	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_IGUAL expresion\n");
};

proposicion_f: GUION_BAJO OP_DISTINTO expresion
{
	fprintf(salidaAS,"proposicion_f: GUION_BAJO OP_DISTINTO expresion\n");
};


lista_expresiones: lista_expresiones COMA expresion
{
	fprintf(salidaAS,"lista_expresiones: lista_expresiones COMA expresion\n");
};

lista_expresiones: expresion
{
	fprintf(salidaAS,"lista_expresiones: expresion\n");
};
%%


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
}