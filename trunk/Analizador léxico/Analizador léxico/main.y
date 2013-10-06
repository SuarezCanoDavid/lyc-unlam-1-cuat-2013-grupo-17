%{
#include "AnalizadorLexico.h"
#include "Pila.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
%token PR_MAIN
%token PR_FUNCTION

%left OP_SUMA
%left OP_RESTA
%left OP_MULTIPLICACION
%left OP_DIVISION
%right OP_ASIGNACION

%start programa

%%
programa:	bloque_declaracion bloque_ejecucion
			{
				imprimirTercetos();
				GenerarAssembler();
			};

programa:	lista_wprints_cte
			{
				imprimirTercetos();
				GenerarAssembler();
			};


bloque_declaracion: PR_VAR declaracion PR_ENDVAR;


declaracion:	declaracion COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA
				{
					equilibrarDeclaracion();
	
					asignarTipoDeDato();
	
					cantIDsEnTS += cantIDsEnDeclaracion;
					cantIDsEnDeclaracion = 0;
					cantTiposEnDeclaracion = 0;
				};

declaracion:	COR_ABRE lista_variables COR_CIERRA DOS_PUNTOS COR_ABRE lista_tipos COR_CIERRA
				{
					equilibrarDeclaracion();
	
					asignarTipoDeDato();
	
					cantIDsEnTS += cantIDsEnDeclaracion;
					cantIDsEnDeclaracion = 0;
					cantTiposEnDeclaracion = 0;
				};


lista_variables:	lista_variables COMA ID
					{
						posicionIDEnTS[cantIDsEnDeclaracion++] = $3;
					};

lista_variables:	ID
					{
						posicionIDEnTS[cantIDsEnDeclaracion++] = $1;
					};


lista_tipos:	lista_tipos COMA tipo
				{
					tipoDeID[cantTiposEnDeclaracion++] = $3;
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


wprint_cte:	PR_WPRINT PAR_ABRE CTE_STRING PAR_CIERRA
			{
				borrarTerceto(&tercetoAux);
				tercetoAux.x = PR_WPRINT;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $3;
				tercetoAux.tipoDeY = INDICE_TS;
				crearTerceto(&tercetoAux);
			};

wprint_cte: PR_WPRINT PAR_ABRE CTE_REAL PAR_CIERRA
			{
				borrarTerceto(&tercetoAux);
				tercetoAux.x = PR_WPRINT;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $3;
				tercetoAux.tipoDeY = INDICE_TS;
				crearTerceto(&tercetoAux);
			};


wprint_id:	PR_WPRINT PAR_ABRE ID PAR_CIERRA
			{
				verificarDeclaracion($3);

				borrarTerceto(&tercetoAux);
				tercetoAux.x = PR_WPRINT;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $3;
				tercetoAux.tipoDeY = INDICE_TS;
				crearTerceto(&tercetoAux);
			};


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
					pushInt(cantTercetos,&pilaDesplazamientos);
				}
				
				PUNTO_COMA

				asignacion
				{
					ptrColaIncremento = (ColaDeTercetos *)malloc(sizeof(ColaDeTercetos));

					vaciarColaDeTercetos(ptrColaIncremento);

					for(aux = VerTope(&pilaDesplazamientos); aux < cantTercetos; ++aux)
					{
						encolarTerceto(&listaDeTercetos[aux],ptrColaIncremento);
					}

					pushCola(ptrColaIncremento,&pilaColasIncrementos);
					cantTercetos = VerTope(&pilaDesplazamientos);
				}
				
				PAR_CIERRA

				lista_sentencias
				
				PR_ROF
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
				verificarDeclaracion($1);

				if(TS[$1].tipo == PR_INT && TS[$3].tipo == PR_STRING)
				{
					printf("\n%d %d\n",TS[$1].tipo,TS[$3].tipo);
					lanzarError("No se puede asignar un tipo STRING a un tipo INT");
				}

				if(TS[$1].tipo == PR_FLOAT && TS[$3].tipo == PR_STRING)
				{
					lanzarError("No se puede asignar un tipo STRING a un tipo FLOAT");
				}

				if(TS[$1].tipo == PR_STRING && TS[$3].tipo == PR_INT)
				{
					lanzarError("No se puede asignar un tipo INT a un tipo STRING");
				}

				if(TS[$1].tipo == PR_STRING && TS[$3].tipo == PR_FLOAT)
				{
					lanzarError("No se puede asignar un tipo FLOAT a un tipo STRING");
				}

				if(TS[$1].tipo == PR_INT && TS[$3].tipo == PR_FLOAT)
				{
					printf("\nADVERTENCIA\nLINEA: %d\nDESCRIPCION: Asignacion de tipo FLOAT a tipo INT con posible perdida de precision\n",lineaActual);
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_ASIGNACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $1;
				tercetoAux.tipoDeY = INDICE_TS;
				tercetoAux.z = $3;
				tercetoAux.tipoDeZ = INDICE_TS; 
				crearTerceto(&tercetoAux);

				$$ = $1;
			};

asignacion: ID OP_ASIGNACION expresion
			{
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

				if(TS[$1].tipo == PR_INT && $3 == PR_FLOAT)
				{
					printf("\nADVERTENCIA\nLINEA: %d\nDESCRIPCION: Asignacion de tipo FLOAT a tipo INT con posible perdida de precision\n",lineaActual);
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_ASIGNACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $1;
				tercetoAux.tipoDeY = INDICE_TS;
				tercetoAux.z = popInt(pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO; 
				crearTerceto(&tercetoAux);

				$$ = $1;
			};

asignacion: ID OP_ASIGNACION CTE_STRING
			{
				verificarDeclaracion($1);

				if(TS[$1].tipo == PR_INT)
				{
					lanzarError("No puede asignar un tipo STRING a un tipo INT");
				}

				if(TS[$1].tipo == PR_FLOAT)
				{
					lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_ASIGNACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $1;
				tercetoAux.tipoDeY = INDICE_TS;
				tercetoAux.z = $3;
				tercetoAux.tipoDeZ = INDICE_TS;
				crearTerceto(&tercetoAux);

				$$ = $1;
			};

asignacion: ID OP_ASIGNACION concatenacion
			{
				verificarDeclaracion($1);

				if(TS[$1].tipo == PR_INT)
				{
					lanzarError("No puede asignar un tipo STRING a un tipo INT");
				}

				if(TS[$1].tipo == PR_FLOAT)
				{
					lanzarError("No puede asignar un tipo STRING a un tipo FLOAT");
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_ASIGNACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $1;
				tercetoAux.tipoDeY = INDICE_TS;
				tercetoAux.z = cantTercetos - 1;
				tercetoAux.tipoDeZ = NRO_TERCETO;
				crearTerceto(&tercetoAux);

				$$ = $1;
			};

concatenacion:	concatenacion_parte_extrema OP_CONCATENACION concatenacion_parte_extrema
				{
					borrarTerceto(&tercetoAux);
					tercetoAux.x = OP_CONCATENACION;
					tercetoAux.tipoDeX = TOKEN;
					tercetoAux.y = $1;
					tercetoAux.tipoDeY = INDICE_TS;
					tercetoAux.z = $3;
					tercetoAux.tipoDeZ = INDICE_TS;
					crearTerceto(&tercetoAux);
				};

concatenacion_parte_extrema:	ID
								{
									verificarDeclaracion($1);

									if(TS[$1].tipo != PR_STRING)
									{
										lanzarError("Solo puede usar el operador concatenacion con tipos STRING");
									}
								};

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


proposicion:	expresion OP_MAYOR expresion
				{
					if($1 == PR_STRING || $3 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion:	expresion OP_MAYOR_IGUAL expresion
				{
					if($1 == PR_STRING || $3 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion:	expresion OP_MENOR expresion
				{
					if($1 == PR_STRING || $3 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion:	expresion OP_MENOR_IGUAL expresion
				{
					if($1 == PR_STRING || $3 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion:	expresion OP_IGUAL expresion
				{
					if($1 == PR_STRING || $3 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion:	expresion OP_DISTINTO expresion
				{
					if($1 == PR_STRING || $3 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};


expresion:	expresion OP_SUMA termino
			{
				if($1 == PR_STRING || $3 == PR_STRING)
				{
					lanzarError("No se puede usar un tipo STRING en una expresion");
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_SUMA;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

				if($1 == PR_FLOAT || $3 == PR_FLOAT)
				{
					$$ = PR_FLOAT;
				}
				else
				{
					$$ = PR_INT;
				}
			};

expresion:	expresion OP_RESTA termino
			{
				if($1 == PR_STRING || $3 == PR_STRING)
				{
					lanzarError("No se puede usar un tipo STRING en una expresion");
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_RESTA;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

				if($1 == PR_FLOAT || $3 == PR_FLOAT)
				{
					$$ = PR_FLOAT;
				}
				else
				{
					$$ = PR_INT;
				}
			};

expresion:	termino
			{
				$$ = $1;
			};


termino:	termino OP_MULTIPLICACION factor
			{
				if($1 == PR_STRING || $3 == PR_STRING)
				{
					lanzarError("No se puede usar un tipo STRING en una expresion");
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_MULTIPLICACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

				if($1 == PR_FLOAT || $3 == PR_FLOAT)
				{
					$$ = PR_FLOAT;
				}
				else
				{
					$$ = PR_INT;
				}
			};

termino:	termino OP_DIVISION factor
			{
				if($1 == PR_STRING || $3 == PR_STRING)
				{
					lanzarError("No se puede usar un tipo STRING en una expresion");
				}

				borrarTerceto(&tercetoAux);
				tercetoAux.x = OP_DIVISION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = popInt(pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = popInt(pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

				if($1 == PR_FLOAT || $3 == PR_FLOAT)
				{
					$$ = PR_FLOAT;
				}
				else
				{
					$$ = PR_INT;
				}
			};

termino:	factor
			{
				$$ = $1;
			};	


factor:	ID
		{
			verificarDeclaracion($1);

			borrarTerceto(&tercetoAux);
			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;

			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

			$$ = TS[$1].tipo;
		};

factor: CTE_ENTERA
		{
			borrarTerceto(&tercetoAux);
			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;

			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

			$$ = PR_INT;
		};

factor: CTE_REAL
		{
			borrarTerceto(&tercetoAux);
			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;

			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

			$$ = PR_FLOAT;
		};

factor: PAR_ABRE expresion PAR_CIERRA
		{
			borrarTerceto(&tercetoAux);
			tercetoAux.x = popInt(pilaExpresiones);
			tercetoAux.tipoDeX = NRO_TERCETO;

			pushInt(crearTerceto(&tercetoAux),pilaExpresiones);

			$$ = $2;
		};

factor: filterc
		{
			$$ = PR_INT;
		};


filterc:	PR_FILTERC
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
			}

			PAR_ABRE

			condicion_f
			
			COMA

			COR_ABRE
			{
				tercetosInicialesFilterc[0] = cantTercetos;
			}

			lista_expresiones
			
			COR_CIERRA

			PAR_CIERRA
			{
				pilaExpresiones = &pilaExpresionesNormal;
			};


condicion_f:	proposicion_f
				{
					tipoCondicionFilterc = 0;
				};

condicion_f:	proposicion_f  PR_AND proposicion_f
				{
					tipoCondicionFilterc = PR_AND;
				};

condicion_f:	proposicion_f PR_OR proposicion_f
				{
					tipoCondicionFilterc = PR_OR;
				};

condicion_f:	PR_NOT PAR_ABRE proposicion_f PAR_CIERRA
				{
					tipoCondicionFilterc = PR_NOT;
				};


proposicion_f:	GUION_BAJO 
				
				OP_MAYOR
				{
					if(registroCHUsado == FALSE)
					{
						tercetosInicialesFilterc[1] = cantTercetos;
					}
					else
					{
						tercetosInicialesFilterc[2] = cantTercetos;
					}
				} 
				
				expresion
				{
					if($4 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion_f:	GUION_BAJO

				OP_MAYOR_IGUAL
				{
					if(registroCHUsado == FALSE)
					{
						tercetosInicialesFilterc[1] = cantTercetos;
					}
					else
					{
						tercetosInicialesFilterc[2] = cantTercetos;
					}
				} 
				
				expresion
				{
					if($4 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion_f:	GUION_BAJO

				OP_MENOR
				{
					if(registroCHUsado == FALSE)
					{
						tercetosInicialesFilterc[1] = cantTercetos;
					}
					else
					{
						tercetosInicialesFilterc[2] = cantTercetos;
					}
				} 
				
				expresion
				{
					if($4 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion_f:	GUION_BAJO

				OP_MENOR_IGUAL
				{
					if(registroCHUsado == FALSE)
					{
						tercetosInicialesFilterc[1] = cantTercetos;
					}
					else
					{
						tercetosInicialesFilterc[2] = cantTercetos;
					}
				} 
				
				expresion
				{
					if($4 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion_f:	GUION_BAJO

				OP_IGUAL
				{
					if(registroCHUsado == FALSE)
					{
						tercetosInicialesFilterc[1] = cantTercetos;
					}
					else
					{
						tercetosInicialesFilterc[2] = cantTercetos;
					}
				} 
				
				expresion
				{
					if($4 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};

proposicion_f:	GUION_BAJO

				OP_DISTINTO
				{
					if(registroCHUsado == FALSE)
					{
						tercetosInicialesFilterc[1] = cantTercetos;
					}
					else
					{
						tercetosInicialesFilterc[2] = cantTercetos;
					}
				} 
				
				expresion
				{
					if($4 == PR_STRING)
					{
						lanzarError("No se puede usar un tipo STRING en una comparacion");
					}

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
				};


lista_expresiones:	expresion
					{
						if($1 == PR_STRING)
						{
							lanzarError("No se puede usar un tipo STRING en una comparacion");
						}

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
					}

					COMA
					{
						tercetosInicialesFilterc[0] = cantTercetos;
					}

					lista_expresiones;

lista_expresiones:	expresion
					{
						if($1 == PR_STRING)
						{
							lanzarError("No se puede usar un tipo STRING en una comparacion");
						}

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
					};

%%


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
	
    if(archivo == NULL)
    {
        printf("No se pudo abrir el archivo fuente");
    }
    else
    {
        inicializarAL(archivo);

		yyparse();
		
        fclose(archivo);
    }
	
    return 0;
}

int yyerror(char *mensaje)
{
	printf("\nERROR\nLINEA: %d\nDESCRIPCION: %s\n",lineaActual,mensaje);
	
	fclose(archivo);
	fclose(archivoDeTokens);
	
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
	printf("\nERROR\nLINEA: %d\nDESCRIPCION: %s\n",lineaActual,mensaje);
		
	fclose(archivo);
	fclose(archivoDeTokens);

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

