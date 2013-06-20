%{
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
programa:	bloque_declaracion bloque_ejecucion
			{
				GenerarAssembler();
			};

programa:	lista_wprints_cte;


bloque_declaracion: PR_VAR declaracion PR_ENDVAR;


declaracion:	declaracion COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA
				{
					fprintf(salidaAS,"]\n");
	
					equilibrarDeclaracion();
	
					asignarTipoDeDato();
	
					cantIDsEnTS += cantIDsEnDeclaracion;
					cantIDsEnDeclaracion = 0;
					cantTiposEnDeclaracion = 0;
				};

declaracion:	COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA
				{
					fprintf(salidaAS,"]\n");
	
					equilibrarDeclaracion();
	
					asignarTipoDeDato();
	
					cantIDsEnTS += cantIDsEnDeclaracion;
					cantIDsEnDeclaracion = 0;
					cantTiposEnDeclaracion = 0;
				};


lista_variables:	ID COMA lista_variables
					{
						posicionIDEnTS[cantIDsEnDeclaracion++] = $1;
					};

lista_variables:	ID
					{
						posicionIDEnTS[cantIDsEnDeclaracion++] = $1;
					};


lista_tipos:	tipo COMA lista_tipos
				{
					tipoDeID[cantTiposEnDeclaracion++] = $1;
				};

lista_tipos:	tipo
				{
					tipoDeID[cantTiposEnDeclaracion++] = $1;
				};


tipo:	PR_INT
		{
			$$ = PR_INT;
		};

tipo:	PR_FLOAT
		{
			$$ = PR_FLOAT;
		};

tipo:	PR_STRING
		{
			$$ = PR_STRING;
		};


bloque_ejecucion:	lista_sentencias;


lista_sentencias:	lista_sentencias sentencia PUNTO_COMA;

lista_sentencias:	sentencia PUNTO_COMA;


sentencia:	wprint;

sentencia:	iteracion;

sentencia:	decision;

sentencia:	asignacion;


lista_wprints_cte:	lista_wprints_cte wprint_cte PUNTO_COMA;

lista_wprints_cte:	wprint_cte PUNTO_COMA;


wprint:	wprint_cte;

wprint: wprint_id;


wprint_cte:	wprint_parte_A CTE_STRING PAR_CIERRA;

wprint_cte: wprint_parte_A CTE_REAL PAR_CIERRA;


wprint_id:	wprint_parte_A ID PAR_CIERRA;

wprint_parte_A:	PR_WPRINT PAR_ABRE;


iteracion:	iteracion_for;

iteracion:	iteracion_dowhile;


iteracion_for:	PR_FOR 

				PAR_ABRE

				asignacion
				{
					pushInt(cantTercetos,&pilaCondiciones);
				}
				
				PUNTO_COMA
				{
					registroBHUsado = FALSE;
				}

				condicion
				{
					tercetoInicial = cantTercetos;
				}
				
				PUNTO_COMA

				asignacion
				{
					ptrColaTercetos = (ColaDeTercetos *)malloc(sizeof(ColaDeTercetos));

					vaciarColaDeTercetos(ptrColaTercetos);

					for(aux = tercetoInicial; aux < cantTercetos; ++aux)
					{
						encolarTerceto(&listaDeTercetos[aux],ptrColaTercetos);
					}

					pushCola(ptrColaTercetos,&pilaColasTercetos);
					cantTercetos = tercetoInicial;
				}
				
				PAR_CIERRA

				lista_sentencias
				
				PR_ROF
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
				};


iteracion_dowhile:	PR_DO
					{
						pushInt(cantTercetos,&pilaCondiciones);
					}

					lista_sentencias
					
					PR_WHILE

					PAR_ABRE
					{
						registroBHUsado = FALSE;
					}

					condicion
					
					PAR_CIERRA
					{
						aux = popInt(&pilaCondiciones);
						listaDeTercetos[aux].tipoDeX = JNZ;
						listaDeTercetos[aux].y = popInt(&pilaCondiciones);
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					};


decision:	decision_parte_A decision_parte_B;

decision_parte_A:	PR_IF

					PAR_ABRE
					{
						registroBHUsado = FALSE;
					}

					condicion

					PAR_CIERRA

					lista_sentencias;

decision_parte_B:	PR_FI
					{
						aux = popInt(&pilaCondiciones);
						listaDeTercetos[aux].y = cantTercetos;
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					};

decision_parte_B:	PR_ELSE
					{
						borrarTerceto(&tercetoAux);
						aux = popInt(&pilaCondiciones);
						tercetoAux.tipoDeX = JMP;
						pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
						listaDeTercetos[aux].y = cantTercetos;
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					}

					lista_sentencias
			
					PR_FI
					{
						aux = popInt(&pilaCondiciones);
						listaDeTercetos[aux].y = cantTercetos;
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					};


asignacion: ID OP_ASIGNACION asignacion
			{
				printf("1 ASIGNACION -> ID := ASIGNACION\n");

				tercetoAux.x = OP_ASIGNACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $1;
				tercetoAux.tipoDeY = INDICE_TS;
				tercetoAux.z = $3;
				tercetoAux.tipoDeZ = INDICE_TS; 

				crearTerceto(&tercetoAux);

				printf("tope=%d\n",pilaExpresiones.tope);

				$$ = $1;
			};

asignacion: ID OP_ASIGNACION expresion
			{
				printf("1 ASIGNACION -> ID := EXPRESION\n");

				tercetoAux.x = OP_ASIGNACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $1;
				tercetoAux.tipoDeY = INDICE_TS;
				tercetoAux.z = popInt(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO; 

				crearTerceto(&tercetoAux);

				$$ = $1;
			};

asignacion: ID OP_ASIGNACION CTE_STRING;

asignacion: ID OP_ASIGNACION concatenacion;

concatenacion:	concatenacion_parte_extrema OP_CONCATENACION concatenacion_parte_extrema;

concatenacion_parte_extrema:	ID;

concatenacion_parte_extrema:	CTE_STRING;


condicion:	proposicion
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
			};

condicion:	proposicion PR_AND proposicion
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
			};

condicion:	proposicion PR_OR proposicion
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
			};

condicion:	PR_NOT PAR_ABRE proposicion PAR_CIERRA
			{
				borrarTerceto(&tercetoAux);
				tercetoAux.x = PR_NOT;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.tipoDeY = BH;
				crearTerceto(&tercetoAux);

				borrarTerceto(&tercetoAux);
				tercetoAux.tipoDeX = JZ;
				pushInt(crearTerceto(&tercetoAux),&pilaCondiciones);
			};


proposicion:	expresion OP_MAYOR expresion
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
				};

proposicion:	expresion OP_MAYOR_IGUAL expresion
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
				};

proposicion:	expresion OP_MENOR expresion
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
				};

proposicion:	expresion OP_MENOR_IGUAL expresion
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
				};

proposicion:	expresion OP_IGUAL expresion
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
				};

proposicion:	expresion OP_DISTINTO expresion
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
				};


expresion:	expresion OP_SUMA termino
			{
				printf("2 EXPRESION -> EXPRESION + TERMINO\n");

				tercetoAux.x = OP_SUMA;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

expresion:	expresion OP_RESTA termino
			{
				tercetoAux.x = OP_RESTA;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

expresion:	termino
			{
				printf("3 EXPRESION -> TERMINO\n");
			};


termino:	termino OP_MULTIPLICACION factor
			{
				printf("4 TERMINO -> TERMINO * FACTOR\n");

				tercetoAux.x = OP_MULTIPLICACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

termino:	termino OP_DIVISION factor
			{
				tercetoAux.x = OP_DIVISION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

termino:	factor
			{
				printf("5 TERMINO -> FACTOR\n");
			};	


factor:	ID
		{
			printf("6 FACTOR -> ID (%s)\n",TS[$1].nombre);

			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
		};

factor: CTE_ENTERA
		{
			printf("7 FACTOR -> CTE (%d)\n",$$);

			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
		};

factor: CTE_REAL
		{
			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
		};

factor: PAR_ABRE expresion PAR_CIERRA
		{
			printf("8 FACTOR -> ( EXPRESION )\n");

			tercetoAux.x = popInt(&pilaExpresiones);
			tercetoAux.tipoDeX = NRO_TERCETO;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			pushInt(crearTerceto(&tercetoAux),&pilaExpresiones);
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
			{
				++aux;
			}
			
			COMA
			{
				fprintf(salidaAS,",");
			}

			COR_ABRE
			{
				fprintf(salidaAS,"[");
			}

			lista_expresiones
			{
				++aux;
			}
			
			COR_CIERRA
			{
				fprintf(salidaAS,"]");
			}

			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};


condicion_f:	proposicion_f
				{
					++aux;
				};

condicion_f:	proposicion_f
				{
					++aux;
				} 

				PR_AND
				{
					fprintf(salidaAS," AND ");
				}

				proposicion_f
				{
					++aux;
				};

condicion_f:	proposicion_f
				{
					++aux;
				}

				PR_OR
				{
					fprintf(salidaAS," OR ");
				}

				proposicion_f
				{
					++aux;
				};

condicion_f:	PR_NOT 
				{
					fprintf(salidaAS,"NOT");
				}

				PAR_ABRE 
				{
					fprintf(salidaAS,"(");
				}

				proposicion_f
				{
					++aux;
				} 
				
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

				expresion
				{
					++aux;
				};

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_MAYOR_IGUAL
				{
					fprintf(salidaAS," >= ");
				}

				expresion
				{
					++aux;
				};

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_MENOR
				{
					fprintf(salidaAS," < ");
				}

				expresion
				{
					++aux;
				};

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_MENOR_IGUAL
				{
					fprintf(salidaAS," <= ");
				}

				expresion
				{
					++aux;
				};

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_IGUAL
				{
					fprintf(salidaAS," == ");
				}

				expresion
				{
					++aux;
				};

proposicion_f:	GUION_BAJO
				{
					fprintf(salidaAS,"_");
				}

				OP_DISTINTO
				{
					fprintf(salidaAS," != ");
				}

				expresion
				{
					++aux;
				};


lista_expresiones:	lista_expresiones
					{
						++aux;
					}

					COMA
					{
						fprintf(salidaAS,",");
					}

					expresion
					{
						++aux;
					};

lista_expresiones:	expresion
					{
						++aux;
					};

%%


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

