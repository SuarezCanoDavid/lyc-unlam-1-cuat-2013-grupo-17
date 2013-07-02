.MODEL SMALL
.386
.STACK 200h

.DATA
MAXTEXTSIZE equ 61
a dd ?
b dd ?
c dd ?
d dd ?
cadena1 db '$',60 dup(?)
cadena1_long dd 1
cadena2 db '$',60 dup(?)
cadena2_long dd 1
e dd ?
_cte0 dd 3.0
_cte1 dd 4.0
_cte2 dd 6.0
_cte3 dd 5.0
_cte4 db "VERDADERO - VERDADERO",'$',9 dup(?)
_cte4_long dd 22
_cte5 db "VERDADERO - FALSO",'$',13 dup(?)
_cte5_long dd 18
_cte6 db "FALSO - VERDADERO",'$',13 dup(?)
_cte6_long dd 18
_cte7 dd 15.999999999
_cte8 dd 20.0
__ENTER       db MAXTEXTSIZE dup(?)
__NEW_LINE    db 0Dh,0Ah,'$'
_parteFrac    dd ?
_parteEntera  dd ?
_mul_10       dd 10.0
_x87_round    dw 077fh

.CODE
	mov ax, @DATA
	mov ds, ax
	mov es, ax
	finit
	fldcw _x87_round

	fld _cte0
	fistp a
	fild a
	fstp a
	fld _cte1
	fistp b
	fild b
	fstp b
	fld _cte0
	fistp d
	fild d
	fstp d
	fld d
	fistp c
	fild c
	fstp c
	fld _cte2
	fistp d
	fild d
	fstp d
	fldz
	fld a
	fld _cte3
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jae etiqueta_16
	mov ch, 000h
etiqueta_16:
	mov cl, 0ffh
	and ch, cl
	jz etiqueta_21
	fld1
	faddp
etiqueta_21:
	fld b
	fld _cte3
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jae etiqueta_27
	mov ch, 000h
etiqueta_27:
	mov cl, 0ffh
	and ch, cl
	jz etiqueta_32
	fld1
	faddp
etiqueta_32:
	fld d
	fld _cte3
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jae etiqueta_38
	mov ch, 000h
etiqueta_38:
	mov cl, 0ffh
	and ch, cl
	jz etiqueta_43
	fld1
	faddp
etiqueta_43:
	fistp a
	fild a
	fstp a
	fld a
	fld b
	mov bh, 0ffh
	fcompp
	fstsw ax
	sahf
	jne etiqueta_50
	mov bh, 000h
etiqueta_50:
	fld a
	fld c
	mov bl, 0ffh
	fcompp
	fstsw ax
	sahf
	jne etiqueta_56
	mov bl, 000h
etiqueta_56:
	and bh, bl
	jz etiqueta_71
	fld c
	fld d
	mov bh, 0ffh
	fcompp
	fstsw ax
	sahf
	je etiqueta_64
	mov bh, 000h
etiqueta_64:
	mov bl, 0ffh
	and bh, bl
	jz etiqueta_69
	cld
	mov esi, OFFSET _cte4
	mov edi, OFFSET __ENTER
	mov ecx, _cte4_long
	rep movsb
	call IMPRIMIR
	jmp etiqueta_70
etiqueta_69:
	cld
	mov esi, OFFSET _cte5
	mov edi, OFFSET __ENTER
	mov ecx, _cte5_long
	rep movsb
	call IMPRIMIR
etiqueta_70:
	jmp etiqueta_81
etiqueta_71:
	fld a
	fld b
	mov bh, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jbe etiqueta_77
	mov bh, 000h
etiqueta_77:
	mov bl, 0ffh
	and bh, bl
	jz etiqueta_81
	cld
	mov esi, OFFSET _cte6
	mov edi, OFFSET __ENTER
	mov ecx, _cte6_long
	rep movsb
	call IMPRIMIR
etiqueta_81:
	fld _cte7
	fstp e
	fld _cte3
	fld _cte8
	fmulp
	fld _cte3
	fdivp
	fld e
	fsubp
	fistp a
	fild a
	fstp a
	mov eax, a
	mov _parteFrac, eax
	call IMPRIMIRNUMERO
	mov eax, e
	mov _parteFrac, eax
	call IMPRIMIRNUMERO
etiqueta_93:
ERROR:
	mov ax, 4C00h
	int 21h

IMPRIMIR:
	mov dx, OFFSET __ENTER
	mov ah, 9
	int 21h
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	ret

IMPRIMIRNUMERO:
	fld _parteFrac
	fistp _parteEntera
	fld _parteFrac
	fild _parteEntera
	fsubp
	fstp _parteFrac

	mov ecx, 0
	mov ebx, 10
	mov eax, _parteEntera
et1:
	mov edx, 0
	div ebx
	push dx
	inc ecx
	cmp eax, 0
	jne et1

	mov bx, 0
et2:
	pop dx
	add dl, 030h
	mov [__ENTER+bx], dl
	inc bx
	loop et2

	fld _parteFrac
	fldz
	fcompp
	fstsw ax
	sahf
	je fin

	mov [__ENTER+bx], '.'
	inc bx

	mov cl, 60
	sub cl, bl
	mov ch, 0ffh

et3:
	fld _parteFrac
	fmul _mul_10
	fst _parteFrac
	fistp _parteEntera
	mov edx, _parteEntera
	add dl, 030h
	mov [__ENTER+bx], dl
	inc bx
	dec cl
	fld _parteFrac
	fild _parteEntera
	fsubp
	fst _parteFrac
	fldz
	fcompp
	fstsw ax
	sahf
	jne et4
	mov ch, 000h
et4:
	and ch, cl
	jz fin
	jmp et3

fin:
	mov [__ENTER+bx], '$'
	call IMPRIMIR
	ret

END