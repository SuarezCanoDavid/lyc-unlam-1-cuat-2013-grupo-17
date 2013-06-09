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
programa: bloque_declaracion bloque_ejecucion;

programa: lista_wprints_cte;


bloque_declaracion: PR_VAR
					{
						fprintf(salidaAS,"VAR\n");
					}
						
					declaracion

					PR_ENDVAR
					{
						fprintf(salidaAS,"ENDVAR\n");
					};


declaracion:	declaracion

				COR_ABRE
				{
					fprintf(salidaAS,"[");
				} 

				lista_variables 

				COR_CIERRA
				{
					fprintf(salidaAS,"]");
				}

				DOS_PUNTOS
				{
					fprintf(salidaAS,":");
				}

				COR_ABRE
				{
					fprintf(salidaAS,"[");
				}
				
				lista_tipos
				
				COR_CIERRA
				{
					fprintf(salidaAS,"]\n");
	
					equilibrarDeclaracion();
	
					asignarTipoDeDato();
	
					cantIDsEnTS += cantIDsEnDeclaracion;
					cantIDsEnDeclaracion = 0;
					cantTiposEnDeclaracion = 0;
				};

declaracion:	COR_ABRE
				{
					fprintf(salidaAS,"[");
				} 

				lista_variables 

				COR_CIERRA
				{
					fprintf(salidaAS,"]");
				}

				DOS_PUNTOS
				{
					fprintf(salidaAS,":");
				}

				COR_ABRE
				{
					fprintf(salidaAS,"[");
				}
				
				lista_tipos
				
				COR_CIERRA
				{
					fprintf(salidaAS,"]\n");
	
					equilibrarDeclaracion();
	
					asignarTipoDeDato();
	
					cantIDsEnTS += cantIDsEnDeclaracion;
					cantIDsEnDeclaracion = 0;
					cantTiposEnDeclaracion = 0;
				};


lista_variables:	ID
					{
						fprintf(salidaAS,"%s",TS[$1].nombre);
	
						posicionIDEnTS[cantIDsEnDeclaracion++] = $1;
					}

					COMA
					{
						fprintf(salidaAS,",");
					}
					
					lista_variables;

lista_variables:	ID
					{
						fprintf(salidaAS,"%s",TS[$1].nombre);
	
						posicionIDEnTS[cantIDsEnDeclaracion++] = $1;
					};


lista_tipos:	tipo
				{
					tipoDeID[cantTiposEnDeclaracion++] = $1;
				}

				COMA
				{
					fprintf(salidaAS,",");
				}

				lista_tipos;

lista_tipos:	tipo
				{
					tipoDeID[cantTiposEnDeclaracion++] = $1;
				};


tipo:	PR_INT
		{
			fprintf(salidaAS,"int");
	
			$$ = PR_INT;
		};

tipo:	PR_FLOAT
		{
			fprintf(salidaAS,"float");
	
			$$ = PR_FLOAT;
		};

tipo:	PR_STRING
		{
			fprintf(salidaAS,"string");
	
			$$ = PR_STRING;
		};


bloque_ejecucion: lista_sentencias;


lista_sentencias:	lista_sentencias

					sentencia 
					
					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};

lista_sentencias:	sentencia 
					
					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};


sentencia: wprint;

sentencia: iteracion;

sentencia: decision;

sentencia: asignacion;


lista_wprints_cte:	lista_wprints_cte 

					wprint_cte
					
					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};

lista_wprints_cte:	wprint_cte 

					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};


wprint: wprint_cte;

wprint: wprint_id;


wprint_cte:	wprint_parte_A
			
			CTE_STRING
			{
				fprintf(salidaAS,"\"%s\"",TS[$2].valor);
			}
			
			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};

wprint_cte: wprint_parte_A 
			
			CTE_REAL
			{
				fprintf(salidaAS,"%s",TS[$2].valor);
			} 
			
			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};


wprint_id:	wprint_parte_A
			
			ID
			{
				fprintf(salidaAS,"%s",TS[$2].nombre);
			}
			
			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};

wprint_parte_A:	PR_WPRINT
				{
					fprintf(salidaAS,"WPRINT");
				}

				PAR_ABRE
				{
					fprintf(salidaAS,"(");
				};


iteracion: iteracion_for;

iteracion: iteracion_dowhile;


iteracion_for:	PR_FOR
				{
					fprintf(salidaAS,"FOR");
				}

				PAR_ABRE
				{
					fprintf(salidaAS,"(");
				}

				asignacion
				
				PUNTO_COMA
				{
					fprintf(salidaAS,";");
				}

				condicion
				
				PUNTO_COMA
				{
					fprintf(salidaAS,";");
				}

				asignacion
				
				PAR_CIERRA
				{
					fprintf(salidaAS,")\n");
				}

				lista_sentencias
				
				PR_ROF
				{
					fprintf(salidaAS,"ROF");
				};


iteracion_dowhile:	PR_DO
					{
						fprintf(salidaAS,"DO\n");
					}

					lista_sentencias
					
					PR_WHILE
					{
						fprintf(salidaAS,"WHILE");
					}

					PAR_ABRE
					{
						fprintf(salidaAS,"(");
					}

					condicion
					
					PAR_CIERRA
					{
						fprintf(salidaAS,")");
					};


decision: decision_parte_A decision_parte_B;

decision_parte_A:	PR_IF
					{
						fprintf(salidaAS,"IF");
					}

					PAR_ABRE
					{
						fprintf(salidaAS,"(");
					}

					condicion

					PAR_CIERRA
					{
						fprintf(salidaAS,")\n");
					}

					lista_sentencias;

decision_parte_B:	PR_FI
					{
						fprintf(salidaAS,"FI");
					};

decision_parte_B:	PR_ELSE
					{
						fprintf(salidaAS,"ELSE\n");
					}

					lista_sentencias 
			
					PR_FI
					{
						fprintf(salidaAS,"FI");
					};


asignacion: asignacion_parte_A asignacion;
asignacion: asignacion_parte_A asignacion_parte_B;

asignacion_parte_A:	ID
					{
						fprintf(salidaAS,"%s",TS[$1].nombre);
					}

					OP_ASIGNACION
					{
						fprintf(salidaAS," = ");
					};

asignacion_parte_B:	expresion
					{
						fprintf(salidaAS,"(=%d)",$1);
					};

asignacion_parte_B:	CTE_STRING
					{
						fprintf(salidaAS,"\"%s\"",TS[$1].valor);
					};

asignacion_parte_B:	concatenacion;


concatenacion:	concatenacion_parte_extrema

				OP_CONCATENACION
				{
					fprintf(salidaAS," ++ "); 
				}

				concatenacion_parte_extrema;

concatenacion_parte_extrema:	ID
								{
									fprintf(salidaAS,"%s",TS[$1].nombre);
								};

concatenacion_parte_extrema:	CTE_STRING
								{
									fprintf(salidaAS,"\"%s\"",TS[$1].valor);
								};


condicion:	proposicion;

condicion:	proposicion 
			
			PR_AND
			{
				fprintf(salidaAS," AND ");
			}

			proposicion;

condicion:	proposicion 
			
			PR_OR
			{
				fprintf(salidaAS," OR ");
			}

			proposicion;

condicion:	PR_NOT
			{
				fprintf(salidaAS," NOT ");
			}

			PAR_ABRE
			{
				fprintf(salidaAS,"(");
			}

			proposicion
			
			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};


proposicion:	expresion

				OP_MAYOR
				{
					fprintf(salidaAS," > ");
				}

				expresion;

proposicion:	expresion

				OP_MAYOR_IGUAL
				{
					fprintf(salidaAS," >= ");
				}

				expresion;

proposicion:	expresion

				OP_MENOR
				{
					fprintf(salidaAS," < ");
				}

				expresion;

proposicion:	expresion 

				OP_MENOR_IGUAL
				{
					fprintf(salidaAS," <= ");
				}

				expresion;

proposicion:	expresion

				OP_IGUAL
				{
					fprintf(salidaAS," == ");
				}

				expresion;

proposicion:	expresion
	
				OP_DISTINTO
				{
					fprintf(salidaAS," != ");
				}

				expresion;


expresion:	expresion

			OP_SUMA
			{
				fprintf(salidaAS," + ");
			}

			termino
			{
				$$ = $1 + $4;
			};

expresion:	expresion
	
			OP_RESTA
			{
				fprintf(salidaAS," - ");
			}

			termino
			{
				$$ = $1 - $4;
			};

expresion:	termino;


termino:	termino

			OP_MULTIPLICACION
			{
				fprintf(salidaAS," * ");
			}

			factor
			{
				$$ = $1 * $4;
			};

termino:	termino

			OP_DIVISION
			{
				fprintf(salidaAS," / ");
			}

			factor
			{
				$$ = $1 / $4;
			};

termino:	factor
			{
				$$ = $1;
			};	


factor:	ID
		{
			fprintf(salidaAS,"%s",TS[$1].nombre);
		};

factor: CTE_ENTERA
		{
			fprintf(salidaAS,"%s",TS[$1].valor);
			$$ = atoi(TS[$1].valor);
		};

factor: CTE_REAL
		{	
			fprintf(salidaAS,"%s",TS[$1].valor);
			$$ = atoi(TS[$1].valor);
		};

factor: PAR_ABRE
		{
			fprintf(salidaAS,"(");
		}

		expresion

		PAR_CIERRA
		{
			fprintf(salidaAS,")");
			$$ = $3;
		};

factor: filterc;


filterc:	PR_FILTERC
			{
				fprintf(salidaAS,"FILTERC");
			}

			PAR_ABRE
			{
				fprintf(salidaAS,"(");
			}

			condicion_f
			
			COMA
			{
				fprintf(salidaAS,",");
			}

			COR_ABRE
			{
				fprintf(salidaAS,"[");
			}

			lista_expresiones
			
			COR_CIERRA
			{
				fprintf(salidaAS,"]");
			}

			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};


condicion_f:	proposicion_f;

condicion_f:	proposicion_f 

				PR_AND
				{
					fprintf(salidaAS," AND ");
				}

				proposicion_f;

condicion_f:	proposicion_f

				PR_OR
				{
					fprintf(salidaAS," OR ");
				}

				proposicion_f;

condicion_f:	PR_NOT 
				{
					fprintf(salidaAS,"NOT");
				}

				PAR_ABRE 
				{
					fprintf(salidaAS,"(");
				}

				proposicion_f 
				
				PAR_CIERRA
				{
					fprintf(salidaAS,")");
				};


proposicion_f:	GUION_BAJO 
				{
					fprintf(salidaAS,"_");
				}

				OP_MAYOR 
				{
					fprintf(salidaAS," > ");
				}

				expresion;

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_MAYOR_IGUAL
				{
					fprintf(salidaAS," >= ");
				}

				expresion;

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_MENOR
				{
					fprintf(salidaAS," < ");
				}

				expresion;

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_MENOR_IGUAL
				{
					fprintf(salidaAS," <= ");
				}

				expresion;

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_IGUAL
				{
					fprintf(salidaAS," == ");
				}

				expresion;

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_DISTINTO
				{
					fprintf(salidaAS," != ");
				}

				expresion;


lista_expresiones:	lista_expresiones

					COMA
					{
						fprintf(salidaAS,",");
					}

					expresion;

lista_expresiones: expresion;

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

