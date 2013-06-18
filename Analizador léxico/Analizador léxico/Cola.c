#include "Cola.h"
#include "GCI.h"

void vaciarColaDeTercetos(ColaDeTercetos *c)
{
	c->tercetoInicial = 0;
	c->cantTercetos = 0;
}

void encolarTerceto(Terceto *t,ColaDeTercetos *c)
{
	c->contenedor[c->cantTercetos].x = t->x;
	c->contenedor[c->cantTercetos].tipoDeX = t->tipoDeX;
	c->contenedor[c->cantTercetos].y = t->y;
	c->contenedor[c->cantTercetos].tipoDeY = t->tipoDeY;
	c->contenedor[c->cantTercetos].z = t->z;
	c->contenedor[c->cantTercetos].tipoDeZ = t->tipoDeZ;

	++c->cantTercetos;
}

Terceto *desencolarTerceto(ColaDeTercetos *c)
{
	--c->cantTercetos;

	return &c->contenedor[c->tercetoInicial++];
}

int colaDeTercetosEstaVacia(ColaDeTercetos *c)
{
	if(c->cantTercetos == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void ajustarTercetos(ColaDeTercetos *c,int ajuste)
{
	int i;

	for(i = c->tercetoInicial; i < c->cantTercetos; ++i)
	{
		if(c->contenedor[i].tipoDeX == NRO_TERCETO)
		{
			c->contenedor[i].x = ajuste++;
		}

		if(c->contenedor[i].tipoDeY == NRO_TERCETO)
		{
			c->contenedor[i].y = ajuste++;
		}

		if(c->contenedor[i].tipoDeZ == NRO_TERCETO)
		{
			c->contenedor[i].z = ajuste++;
		}
	}
}