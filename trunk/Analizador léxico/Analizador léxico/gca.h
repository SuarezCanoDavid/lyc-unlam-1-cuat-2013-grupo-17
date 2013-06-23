#include <stdio.h>

#ifndef GCA_H
#define GCA_H

void GenerarAssembler();
void GenerarEncabezado();
void DeclararVariables();
void GeneracionCodigo();
void GenerarAssemblerByTerceto();
void TercetoTokes(int);
void LlenarPilaEtiquetas();
void asmSuma();
void asmResta();
void asmMultiplicar();
void asmDividir();
void asmAsignacion();
void asmDistinto();
void asmIgual();
void asmMayor();
void asmMayorIgual();
void asmMenor();
void asmMenorIgual();
#endif