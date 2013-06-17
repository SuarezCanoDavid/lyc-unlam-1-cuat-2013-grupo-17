%{
#include "AnalizadorLexico.h"
#include "Pila.h"
#include "GCI.h"
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

int aux = 0;

Pila pilaExpresiones;
Pila pilaCondiciones;

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
programa:	bloque_declaracion
			{
				++aux;
			}

			bloque_ejecucion
			{
				++aux;
			};

programa:	lista_wprints_cte
			{
				++aux;
			};


bloque_declaracion: PR_VAR
					{
						fprintf(salidaAS,"VAR\n");
					}
						
					declaracion
					{
						++aux;
					}

					PR_ENDVAR
					{
						fprintf(salidaAS,"ENDVAR\n");
					};


declaracion:	declaracion
				{
					++aux;
				}

				COR_ABRE
				{
					fprintf(salidaAS,"[");
				} 

				lista_variables
				{
					++aux;
				} 

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
				{
					++aux;
				}
				
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
				{
					++aux;
				} 

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
				{
					++aux;
				}
				
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
					
					lista_variables
					{
						++aux;
					};

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

				lista_tipos
				{
					++aux;
				};

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


bloque_ejecucion:	lista_sentencias
					{
						++aux;
					};


lista_sentencias:	lista_sentencias
					{
						++aux;
					}

					sentencia
					{
						++aux;
					} 
					
					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};

lista_sentencias:	sentencia
					{
						++aux;
					} 
					
					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};


sentencia:	wprint
			{
				++aux;
			};

sentencia:	iteracion
			{
				++aux;
			};

sentencia:	decision
			{
				++aux;
			};

sentencia:	asignacion
			{
				++aux;
			};


lista_wprints_cte:	lista_wprints_cte
					{
						++aux;
					}

					wprint_cte
					{
						++aux;
					}
					
					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};

lista_wprints_cte:	wprint_cte
					{
						++aux;
					} 

					PUNTO_COMA
					{
						fprintf(salidaAS,";\n");
					};


wprint:	wprint_cte
		{
			++aux;
		};

wprint: wprint_id
		{
			++aux;
		};


wprint_cte:	wprint_parte_A
			{
				++aux;
			}
			
			CTE_STRING
			{
				fprintf(salidaAS,"\"%s\"",TS[$3].valor);
			}
			
			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};

wprint_cte: wprint_parte_A
			{
				++aux;
			} 
			
			CTE_REAL
			{
				fprintf(salidaAS,"%s",TS[$3].valor);
			} 
			
			PAR_CIERRA
			{
				fprintf(salidaAS,")");
			};


wprint_id:	wprint_parte_A
			{
				++aux;
			}
			
			ID
			{
				fprintf(salidaAS,"%s",TS[$3].nombre);
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


iteracion:	iteracion_for
			{
				++aux;
			};

iteracion:	iteracion_dowhile
			{
				++aux;
			};


iteracion_for:	PR_FOR
				{
					fprintf(salidaAS,"FOR");
				}

				PAR_ABRE
				{
					fprintf(salidaAS,"(");
				}

				asignacion
				{
					++aux;
				}
				
				PUNTO_COMA
				{
					fprintf(salidaAS,";");

					registroBHUsado = FALSE;
				}

				condicion
				{
					++aux;
				}
				
				PUNTO_COMA
				{
					fprintf(salidaAS,";");
				}

				asignacion
				{
					++aux;
				}
				
				PAR_CIERRA
				{
					fprintf(salidaAS,")\n");
				}

				lista_sentencias
				{
					++aux;
				}
				
				PR_ROF
				{
					fprintf(salidaAS,"ROF");
				};


iteracion_dowhile:	PR_DO
					{
						fprintf(salidaAS,"DO\n");

						push(cantTercetos,&pilaCondiciones);
					}

					lista_sentencias
					{
						++aux;
					}
					
					PR_WHILE
					{
						fprintf(salidaAS,"WHILE");
					}

					PAR_ABRE
					{
						fprintf(salidaAS,"(");

						registroBHUsado = FALSE;
					}

					condicion
					{
						++aux;
					}
					
					PAR_CIERRA
					{
						fprintf(salidaAS,")");

						aux = pop(&pilaCondiciones);
						listaDeTercetos[aux].tipoDeX = JNZ;
						listaDeTercetos[aux].y = pop(&pilaCondiciones);
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					};


decision:	decision_parte_A
			{
				++aux;
			}
			 
			decision_parte_B
			{
				++aux;
			};

decision_parte_A:	PR_IF
					{
						fprintf(salidaAS,"IF");
					}

					PAR_ABRE
					{
						fprintf(salidaAS,"(");

						registroBHUsado = FALSE;
					}

					condicion
					{
						++aux;
					}

					PAR_CIERRA
					{
						fprintf(salidaAS,")\n");
					}

					lista_sentencias
					{
						++aux;
					};

decision_parte_B:	PR_FI
					{
						fprintf(salidaAS,"FI");

						aux = pop(&pilaCondiciones);
						listaDeTercetos[aux].y = cantTercetos;
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					};

decision_parte_B:	PR_ELSE
					{
						fprintf(salidaAS,"ELSE\n");

						borrarTerceto(&tercetoAux);
						aux = pop(&pilaCondiciones);
						tercetoAux.tipoDeX = JMP;
						push(crearTerceto(&tercetoAux),&pilaCondiciones);
						listaDeTercetos[aux].y = cantTercetos;
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					}

					lista_sentencias
					{
						++aux;
					} 
			
					PR_FI
					{
						fprintf(salidaAS,"FI");

						aux = pop(&pilaCondiciones);
						listaDeTercetos[aux].y = cantTercetos;
						listaDeTercetos[aux].tipoDeY = NRO_TERCETO;
					};


asignacion: ID OP_ASIGNACION asignacion
			{
				fprintf(salidaAS,"(=%d)",$1);
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
				fprintf(salidaAS,"(=%d)",$1);
				printf("1 ASIGNACION -> ID := EXPRESION\n");

				tercetoAux.x = OP_ASIGNACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.y = $1;
				tercetoAux.tipoDeY = INDICE_TS;
				tercetoAux.z = pop(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO; 

				crearTerceto(&tercetoAux);

				$$ = $1;
			};

asignacion: ID OP_ASIGNACION CTE_STRING;

asignacion: ID OP_ASIGNACION concatenacion;

concatenacion:	concatenacion_parte_extrema
				{
					++aux;
				}

				OP_CONCATENACION
				{
					fprintf(salidaAS," ++ "); 
				}

				concatenacion_parte_extrema
				{
					++aux;
				};

concatenacion_parte_extrema:	ID
								{
									fprintf(salidaAS,"%s",TS[$1].nombre);
								};

concatenacion_parte_extrema:	CTE_STRING
								{
									fprintf(salidaAS,"\"%s\"",TS[$1].valor);
								};


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
				push(crearTerceto(&tercetoAux),&pilaCondiciones);
			};

condicion:	proposicion
			{
				++aux;
			} 
			
			PR_AND
			{
				fprintf(salidaAS," AND ");
			}

			proposicion
			{
				borrarTerceto(&tercetoAux);
				tercetoAux.x = PR_AND;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.tipoDeY = BH;
				tercetoAux.tipoDeZ = BL;
				crearTerceto(&tercetoAux);

				borrarTerceto(&tercetoAux);
				tercetoAux.tipoDeX = JZ;
				push(crearTerceto(&tercetoAux),&pilaCondiciones);
			};

condicion:	proposicion
			{
				++aux;
			} 
			
			PR_OR
			{
				fprintf(salidaAS," OR ");
			}

			proposicion
			{
				borrarTerceto(&tercetoAux);
				tercetoAux.x = PR_OR;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.tipoDeY = BH;
				tercetoAux.tipoDeZ = BL;
				crearTerceto(&tercetoAux);

				borrarTerceto(&tercetoAux);
				tercetoAux.tipoDeX = JZ;
				push(crearTerceto(&tercetoAux),&pilaCondiciones);
			};

condicion:	PR_NOT
			{
				fprintf(salidaAS," NOT ");
			}

			PAR_ABRE
			{
				fprintf(salidaAS,"(");
			}

			proposicion
			{
				++aux;
			}
			
			PAR_CIERRA
			{
				fprintf(salidaAS,")");

				borrarTerceto(&tercetoAux);
				tercetoAux.x = PR_NOT;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.tipoDeY = BH;
				crearTerceto(&tercetoAux);

				borrarTerceto(&tercetoAux);
				tercetoAux.tipoDeX = JZ;
				push(crearTerceto(&tercetoAux),&pilaCondiciones);
			};


proposicion:	expresion
				{
					++aux;
				}

				OP_MAYOR
				{
					fprintf(salidaAS," > ");
				}

				expresion
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

proposicion:	expresion
				{
					++aux;
				}

				OP_MAYOR_IGUAL
				{
					fprintf(salidaAS," >= ");
				}

				expresion
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

proposicion:	expresion
				{
					++aux;
				}

				OP_MENOR
				{
					fprintf(salidaAS," < ");
				}

				expresion
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

proposicion:	expresion
				{
					++aux;
				} 

				OP_MENOR_IGUAL
				{
					fprintf(salidaAS," <= ");
				}

				expresion
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

proposicion:	expresion
				{
					++aux;
				}

				OP_IGUAL
				{
					fprintf(salidaAS," == ");
				}

				expresion
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

proposicion:	expresion
				{
					++aux;
				}
	
				OP_DISTINTO
				{
					fprintf(salidaAS," != ");
				}

				expresion
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


expresion:	expresion
			{
				++aux;
			}

			OP_SUMA
			{
				fprintf(salidaAS," + ");
			}

			termino
			{
				$$ = $1 + $5;
				printf("2 EXPRESION -> EXPRESION + TERMINO\n");

				tercetoAux.x = OP_SUMA;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = pop(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = pop(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				push(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

expresion:	expresion
			{
				++aux;
			}
	
			OP_RESTA
			{
				fprintf(salidaAS," - ");
			}

			termino
			{
				$$ = $1 - $5;

				tercetoAux.x = OP_RESTA;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = pop(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = pop(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				push(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

expresion:	termino
			{
				++aux;
				printf("3 EXPRESION -> TERMINO\n");
			};


termino:	termino
			{
				++aux;
			}

			OP_MULTIPLICACION
			{
				fprintf(salidaAS," * ");
			}

			factor
			{
				$$ = $1 * $5;
				printf("4 TERMINO -> TERMINO * FACTOR\n");

				tercetoAux.x = OP_MULTIPLICACION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = pop(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = pop(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				push(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

termino:	termino
			{
				++aux;
			}

			OP_DIVISION
			{
				fprintf(salidaAS," / ");
			}

			factor
			{
				$$ = $1 / $5;

				tercetoAux.x = OP_DIVISION;
				tercetoAux.tipoDeX = TOKEN;
				tercetoAux.z = pop(&pilaExpresiones);
				tercetoAux.tipoDeZ = NRO_TERCETO;
				tercetoAux.y = pop(&pilaExpresiones);
				tercetoAux.tipoDeY = NRO_TERCETO; 

				push(crearTerceto(&tercetoAux),&pilaExpresiones);
			};

termino:	factor
			{
				$$ = $1;
				printf("5 TERMINO -> FACTOR\n");
			};	


factor:	ID
		{
			fprintf(salidaAS,"%s",TS[$1].nombre);
			printf("6 FACTOR -> ID (%s)\n",TS[$1].nombre);

			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			push(crearTerceto(&tercetoAux),&pilaExpresiones);
		};

factor: CTE_ENTERA
		{
			fprintf(salidaAS,"%s",TS[$1].valor);
			$$ = atoi(TS[$1].valor);
			printf("7 FACTOR -> CTE (%d)\n",$$);

			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			push(crearTerceto(&tercetoAux),&pilaExpresiones);
		};

factor: CTE_REAL
		{	
			fprintf(salidaAS,"%s",TS[$1].valor);
			$$ = atoi(TS[$1].valor);

			tercetoAux.x = $1;
			tercetoAux.tipoDeX = INDICE_TS;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			push(crearTerceto(&tercetoAux),&pilaExpresiones);
		};

factor: PAR_ABRE
		{
			fprintf(salidaAS,"(");
		}

		expresion
		{
			++aux;
		}

		PAR_CIERRA
		{
			fprintf(salidaAS,")");
			$$ = $3;
			printf("8 FACTOR -> ( EXPRESION )\n");

			tercetoAux.x = pop(&pilaExpresiones);
			tercetoAux.tipoDeX = NRO_TERCETO;
			tercetoAux.tipoDeY = IGNORAR;
			tercetoAux.tipoDeZ = IGNORAR;

			push(crearTerceto(&tercetoAux),&pilaExpresiones);
		};

factor: filterc
		{
			++aux;
		};


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
	vaciar(&pilaExpresiones);
	vaciar(&pilaCondiciones);

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

