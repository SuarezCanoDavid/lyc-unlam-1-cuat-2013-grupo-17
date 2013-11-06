#include "Pila.h"
#include <stdlib.h>
#include <stdio.h>

void pushInt(int elemento,PilaDeInt *pila)
{
	pila->contenedor[pila->tope] = elemento;

	++pila->tope;

	if(pila->tope > TAM_PILA)
		printf("Tamano max pila!!!\n");
}

int popInt(PilaDeInt *pila)
{
	--pila->tope;

	return pila->contenedor[pila->tope];
}

void vaciarPilaDeInt(PilaDeInt *pila)
{
	pila->tope = 0;
}

int VerTope(PilaDeInt *pila)
{
	return pila->contenedor[pila->tope-1];

}

int PilaVacia(PilaDeInt *pila)
{
	if(pila->tope==0)
		return 1;
	return 0;
}

void OrdenarPila(PilaDeInt *pila)
{
	qsort (pila->contenedor,(size_t) pila->tope,sizeof(int),comparar);
}

int comparar (const void * a, const void * b)
{
	if( *(int*)a >  *(int*)b )
	{ 
		return -1;
	}
	else 
	{	  
		if( *(int*)a == *(int*)b )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

int contiene(PilaDeInt *pila,int elemento)
{
	int encontrado = FALSE;
	int i = 0;

	while(i < pila->tope && encontrado == FALSE)
	{
		if(pila->contenedor[i] == elemento)
		{
			encontrado = TRUE;
		}

		++i;
	}

	return encontrado;
}


//Pila de colas

void pushCola(ColaDeTercetos *c,PilaDeColas *pila)
{
	pila->contenedor[pila->tope] = c;

	++pila->tope;
}

ColaDeTercetos *popCola(PilaDeColas *pila)
{
	--pila->tope;

	return pila->contenedor[pila->tope];
}

void vaciarPilaDeCola(PilaDeColas *pila)
{
	pila->tope = 0;
}