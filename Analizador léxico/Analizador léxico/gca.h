#include <stdio.h>


#ifndef GCA_H
#define GCA_H

void GenerarAssembler();
void GenerarEncabezado();
void DeclararVariables();
void GeneracionCodigo();
void GenerarCodigoString();
int GenerarAssemblerByTerceto();
int TercetoTokes(int,int);
void LlenarPilaEtiquetas();
void asmSuma();
void asmResta();
void asmMultiplicar();
void asmDividir();
void asmAsignacion(int);
void asmDistinto();
void asmIgual();
void asmMayor();
void asmMayorIgual();
void asmMenor();
void asmMenorIgual();
void asmSalto(int);
void asmAnd(int);
void asmOr(int);
void asmNot(int);
void asmWprint(int);
void asmConcatenacion(int);
void adecuarTS();
void asmCall(int);
void asmEtiquetaFuncion(int);
void asmEtiquetaReturn(int);
void asmScanf(int);
void GenerarCodigoInput();
void GenerarConversiones();
#endif