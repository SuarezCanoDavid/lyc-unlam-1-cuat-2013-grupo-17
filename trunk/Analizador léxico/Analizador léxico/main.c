#include "AnalizadorLexico.h"
#include <stdio.h>

int main()
{
	FILE *archivo;
	int tipoToken;

	fopen_s(&archivo,"fuente.txt","r");

	if(archivo == NULL)
	{
		printf("No se pudo abrir fuente.txt");
	}
	else
	{
		inicializarAL(archivo);

		while(!feof(archivo))
		{
			tipoToken = yylex();
		}

		fclose(archivo);
	}

	return 0;
}
