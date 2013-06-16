#ifndef PILA_H
#define PILA_H

#define TAM_PILA	20

struct Pila
{
	int contenedor[TAM_PILA];
	int tope;
};
typedef struct Pila Pila;

void push(int elemento,Pila *pila);
int pop(Pila *pila);
void vaciar(Pila *pila);

#endif