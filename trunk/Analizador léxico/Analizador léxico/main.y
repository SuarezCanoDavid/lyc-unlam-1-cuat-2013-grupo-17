%{
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

%left OP_SUMA
%left OP_RESTA
%left OP_MULTIPLICACION
%left OP_DIVISION
%right OP_ASIGNACION

%start programa

%%
programa: bloque_declaracion bloque_ejecucion
{
	fprintf(salidaAS,"01 programa: bloque_declaracion bloque_ejecucion\n");
};


programa: lista_wprints_cte
{
	fprintf(salidaAS,"02 programa: lista_wprints_cte\n");
};


bloque_declaracion: PR_VAR declaracion PR_ENDVAR
{
	fprintf(salidaAS,"03 bloque_declaracion: PR_VAR declaracion PR_ENDVAR\n");
};


declaracion: declaracion COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA 
{
	fprintf(salidaAS,"04 declaracion: declaracion COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA\n");
	
	equilibrarDeclaracion();
	
	asignarTipoDeDato();
	
	cantIDsEnTS += cantIDsEnDeclaracion;
	cantIDsEnDeclaracion = 0;
	cantTiposEnDeclaracion = 0;
};

declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA
{
	fprintf(salidaAS,"05 declaracion: COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA\n");
	
	equilibrarDeclaracion();
	
	asignarTipoDeDato();
	
	cantIDsEnTS += cantIDsEnDeclaracion;
	cantIDsEnDeclaracion = 0;
	cantTiposEnDeclaracion = 0;
};


lista_variables: lista_variables COMA ID
{
	fprintf(salidaAS,"06 lista_variables: lista_variables COMA ID\n");
	
	posicionIDEnTS[cantIDsEnDeclaracion++] = $3;
};

lista_variables: ID
{
	fprintf(salidaAS,"07 lista_variables: ID\n");
	
	posicionIDEnTS[cantIDsEnDeclaracion++] = $1;
};


lista_tipos: lista_tipos COMA tipo
{
	fprintf(salidaAS,"08 lista_tipos: lista_tipos COMA tipo\n");
	
	tipoDeID[cantTiposEnDeclaracion++] = $3;
};

lista_tipos: tipo
{
	fprintf(salidaAS,"09 lista_tipos: tipo\n");
	
	tipoDeID[cantTiposEnDeclaracion++] = $1;
};


tipo: PR_INT
{
	fprintf(salidaAS,"10 tipo: PR_INT\n");
	
	$$ = PR_INT;
};

tipo: PR_FLOAT
{
	fprintf(salidaAS,"11 tipo: PR_FLOAT\n");
	
	$$ = PR_FLOAT;
};

tipo: PR_STRING
{
	fprintf(salidaAS,"12 tipo: PR_STRING\n");
	
	$$ = PR_STRING;
};


bloque_ejecucion: lista_sentencias
{
	fprintf(salidaAS,"13 bloque_ejecucion: lista_sentencias\n");
};


lista_sentencias: lista_sentencias sentencia PUNTO_COMA
{
	fprintf(salidaAS,"14 lista_sentencias: lista_sentencias sentencia PUNTO_COMA\n");
};

lista_sentencias: sentencia PUNTO_COMA
{
	fprintf(salidaAS,"15 lista_sentencias: sentencia PUNTO_COMA\n");
};


sentencia: wprint
{
	fprintf(salidaAS,"16 sentencia: wprint\n");
};

sentencia: iteracion
{
	fprintf(salidaAS,"17 sentencia: iteracion\n");
};

sentencia: decision
{
	fprintf(salidaAS,"18 sentencia: decision\n");
};

sentencia: asignacion
{
	fprintf(salidaAS,"19 sentencia: asignacion\n");
};


lista_wprints_cte: lista_wprints_cte wprint_cte PUNTO_COMA
{
	fprintf(salidaAS,"20 lista_wprints_cte: lista_wprints_cte wprint_cte PUNTO_COMA\n");
};

lista_wprints_cte: wprint_cte PUNTO_COMA
{
	fprintf(salidaAS,"21 lista_wprints_cte: wprint_cte PUNTO_COMA\n");
};


wprint: wprint_cte
{
	fprintf(salidaAS,"22 wprint: wprint_cte\n");
};

wprint: wprint_id
{
	fprintf(salidaAS,"23 wprint: wprint_id\n");
};


wprint_cte: PR_WPRINT PAR_ABRE CTE_STRING PAR_CIERRA
{
	fprintf(salidaAS,"24 wprint_cte: PR_WPRINT PAR_ABRE CTE_STRING PAR_CIERRA\n");
};

wprint_cte: PR_WPRINT PAR_ABRE CTE_REAL PAR_CIERRA
{
	fprintf(salidaAS,"25 wprint_cte: PR_WPRINT PAR_ABRE CTE_REAL PAR_CIERRA\n");
};


wprint_id: PR_WPRINT PAR_ABRE ID PAR_CIERRA
{
	fprintf(salidaAS,"26 wprint_id: PR_WPRINT PAR_ABRE ID PAR_CIERRA\n");

	verificarDeclaracion($3);
};


iteracion: iteracion_for
{
	fprintf(salidaAS,"27 iteracion: iteracion_for\n");
};

iteracion: iteracion_dowhile
{
	fprintf(salidaAS,"28 iteracion: iteracion_dowhile\n");
};


iteracion_for: PR_FOR PAR_ABRE asignacion PUNTO_COMA condicion PUNTO_COMA asignacion PAR_CIERRA lista_sentencias PR_ROF
{
	fprintf(salidaAS,"29 iteracion_for: PR_FOR PAR_ABRE asignacion PUNTO_COMA condicion PUNTO_COMA asignacion PAR_CIERRA lista_sentencias PR_ROF\n");
};


iteracion_dowhile: PR_DO lista_sentencias PR_WHILE PAR_ABRE condicion PAR_CIERRA
{
	fprintf(salidaAS,"30 iteracion_dowhile: PR_DO lista_sentencias PR_WHILE PAR_ABRE condicion PAR_CIERRA\n");
};


decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_FI
{
	fprintf(salidaAS,"31 decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_FI\n");
};

decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_ELSE lista_sentencias PR_FI
{
	fprintf(salidaAS,"32 decision: PR_IF PAR_ABRE condicion PAR_CIERRA lista_sentencias PR_ELSE lista_sentencias PR_FI\n");
};


asignacion: ID OP_ASIGNACION asignacion
{
	fprintf(salidaAS,"33 asignacion: ID OP_ASIGNACION asignacion\n");

	verificarDeclaracion($1);

	if(TS[$1].tipo == PR_INT && $3 == PR_STRING)
	{
		lanzarError("No se puede asignar un tipo STRING a un tipo INT");
	}

	if(TS[$1].tipo == PR_FLOAT && $3 == PR_STRING)
	{
		lanzarError("No se puede asignar un tipo STRING a un tipo FLOAT");
	}

	if(TS[$1].tipo == PR_STRING && $3 == PR_INT)
	{
		lanzarError("No se puede asignar un tipo INT a un tipo STRING");
	}

	if(TS[$1].tipo == PR_STRING && $3 == PR_FLOAT)
	{
		lanzarError("No se puede asignar un tipo FLOAT a un tipo STRING");
	}
};

asignacion: asignacion_num_o_id
{
	fprintf(salidaAS,"34 asignacion: asignacion_num_o_id\n");
};

asignacion: asignacion_string
{
	fprintf(salidaAS,"35 asignacion: asignacion_string\n");
};


asignacion_num_o_id: ID OP_ASIGNACION expresion
{
	fprintf(salidaAS,"36 asignacion_num_o_id: ID OP_ASIGNACION expresion\n");

	verificarDeclaracion($1);

	if(TS[$1].tipo == PR_STRING)
	{
		lanzarError("No puede asignar una expresion a un tipo STRING");
	}

	$$ = TS[$1].tipo;
};


asignacion_string: ID OP_ASIGNACION CTE_STRING
{
	fprintf(salidaAS,"37 asignacion_string: ID OP_ASIGNACION CTE_STRING\n");

	verificarDeclaracion($1);

	if(TS[$1].tipo == PR_INT)
	{
		lanzarError("No puede asignar un tipo STRING a un tipo INT");
	}

	if(TS[$1].tipo == PR_FLOAT)
	{
		lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
	}

	$$ = TS[$1].tipo;
};

asignacion_string: ID OP_ASIGNACION concatenacion
{
	fprintf(salidaAS,"38 asignacion_string: ID OP_ASIGNACION concatenacion\n");

	verificarDeclaracion($1);

	if(TS[$1].tipo == PR_INT)
	{
		lanzarError("No puede asignar un tipo STRING a un tipo INT");
	}

	if(TS[$1].tipo == PR_FLOAT)
	{
		lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
	}

	$$ = TS[$1].tipo;
};


concatenacion: ID OP_CONCATENACION ID
{
	fprintf(salidaAS,"39 concatenacion: ID OP_CONCATENACION ID\n");

	verificarDeclaracion($1);
	verificarDeclaracion($3);

	if(TS[$1].tipo != PR_STRING || TS[$3].tipo != PR_STRING)
	{
		lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
	}
};

concatenacion: ID OP_CONCATENACION CTE_STRING
{
	fprintf(salidaAS,"40 concatenacion: ID OP_CONCATENACION CTE_STRING\n");

	verificarDeclaracion($1);

	if(TS[$1].tipo != PR_STRING)
	{
		lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
	}
};

concatenacion: CTE_STRING OP_CONCATENACION ID
{
	fprintf(salidaAS,"41 concatenacion: CTE_STRING OP_CONCATENACION ID\n");

	verificarDeclaracion($3);

	if(TS[$3].tipo != PR_STRING)
	{
		lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
	}
};

concatenacion: CTE_STRING OP_CONCATENACION CTE_STRING
{
	fprintf(salidaAS,"42 concatenacion: CTE_STRING OP_CONCATENACION CTE_STRING\n");
};


condicion: proposicion
{
	fprintf(salidaAS,"43 condicion: proposicion\n");
};

condicion: proposicion PR_AND proposicion
{
	fprintf(salidaAS,"44 condicion: proposicion PR_AND proposicion\n");
};

condicion: proposicion PR_OR proposicion
{
	fprintf(salidaAS,"45 condicion: proposicion PR_OR proposicion\n");
};

condicion: PR_NOT PAR_ABRE proposicion PAR_CIERRA
{
	fprintf(salidaAS,"46 condicion: PR_NOT PAR_ABRE proposicion PAR_CIERRA\n");
};


proposicion: expresion OP_MAYOR expresion
{
	fprintf(salidaAS,"47 proposicion: expresion OP_MAYOR expresion\n");
};

proposicion: expresion OP_MAYOR_IGUAL expresion
{
	fprintf(salidaAS,"48 proposicion: expresion OP_MAYOR_IGUAL expresion\n");
};

proposicion: expresion OP_MENOR expresion
{
	fprintf(salidaAS,"49 proposicion: expresion OP_MENOR expresion\n");
};

proposicion: expresion OP_MENOR_IGUAL expresion
{
	fprintf(salidaAS,"50 proposicion: expresion OP_MENOR_IGUAL expresion\n");
};

proposicion: expresion OP_IGUAL expresion
{
	fprintf(salidaAS,"51 proposicion: expresion OP_IGUAL expresion\n");
};

proposicion: expresion OP_DISTINTO expresion
{
	fprintf(salidaAS,"52 proposicion: expresion OP_DISTINTO expresion\n");
};


expresion: expresion OP_SUMA termino
{
	fprintf(salidaAS,"53 expresion: expresion OP_SUMA termino\n");
};

expresion: expresion OP_RESTA termino
{
	fprintf(salidaAS,"54 expresion: expresion OP_RESTA termino\n");
};

expresion: termino
{
	fprintf(salidaAS,"55 expresion: termino\n");
};


termino: termino OP_MULTIPLICACION factor
{
	fprintf(salidaAS,"56 termino: termino OP_MULTIPLICACION factor\n");
};

termino: termino OP_DIVISION factor
{
	fprintf(salidaAS,"57 termino: termino OP_DIVISION factor\n");
};

termino: factor
{
	fprintf(salidaAS,"58 termino: factor\n");
};


factor: ID
{
	fprintf(salidaAS,"59 factor: ID\n");

	verificarDeclaracion($1);

	if(TS[$1].tipo == PR_STRING)
	{
		lanzarError("No puede usar tipos STRING en una expresion");
	}
};

factor: CTE_ENTERA
{
	fprintf(salidaAS,"60 factor: CTE_ENTERA\n");
};

factor: CTE_REAL
{
	fprintf(salidaAS,"61 factor: CTE_REAL\n");
};

factor: PAR_ABRE expresion PAR_CIERRA
{
	fprintf(salidaAS,"62 factor: PAR_ABRE expresion PAR_CIERRA\n");
};

factor: filterc
{
	fprintf(salidaAS,"63 factor: filterc\n");
};


filterc: PR_FILTERC PAR_ABRE condicion_f COMA COR_ABRE lista_expresiones COR_CIERRA PAR_CIERRA
{
	fprintf(salidaAS,"64 filterc: PR_FILTERC PAR_ABRE condicion_f COMA COR_ABRE lista_expresiones COR_CIERRA PAR_CIERRA\n");
};


condicion_f: proposicion_f
{
	fprintf(salidaAS,"65 condicion_f: proposicion_f\n");
};

condicion_f: proposicion_f PR_AND proposicion_f
{
	fprintf(salidaAS,"66 condicion_f: proposicion_f PR_AND proposicion_f\n");
};

condicion_f: proposicion_f PR_OR proposicion_f
{
	fprintf(salidaAS,"67 condicion_f: proposicion_f PR_OR proposicion_f\n");
};

condicion_f: PR_NOT PAR_ABRE proposicion_f PAR_CIERRA
{
	fprintf(salidaAS,"68 condicion_f: PR_NOT PAR_ABRE proposicion_f PAR_CIERRA\n");
};


proposicion_f: GUION_BAJO OP_MAYOR expresion
{
	fprintf(salidaAS,"69 proposicion_f: GUION_BAJO OP_MAYOR expresion\n");
};

proposicion_f: GUION_BAJO OP_MAYOR_IGUAL expresion
{
	fprintf(salidaAS,"70 proposicion_f: GUION_BAJO OP_MAYOR_IGUAL expresion\n");
};

proposicion_f: GUION_BAJO OP_MENOR expresion
{
	fprintf(salidaAS,"71 proposicion_f: GUION_BAJO OP_MENOR expresion\n");
};

proposicion_f: GUION_BAJO OP_MENOR_IGUAL expresion
{
	fprintf(salidaAS,"72 proposicion_f: GUION_BAJO OP_MENOR_IGUAL expresion\n");
};

proposicion_f: GUION_BAJO OP_IGUAL expresion
{
	fprintf(salidaAS,"73 proposicion_f: GUION_BAJO OP_IGUAL expresion\n");
};

proposicion_f: GUION_BAJO OP_DISTINTO expresion
{
	fprintf(salidaAS,"74 proposicion_f: GUION_BAJO OP_DISTINTO expresion\n");
};


lista_expresiones: lista_expresiones COMA expresion
{
	fprintf(salidaAS,"75 lista_expresiones: lista_expresiones COMA expresion\n");
};

lista_expresiones: expresion
{
	fprintf(salidaAS,"76 lista_expresiones: expresion\n");
};
%%


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

