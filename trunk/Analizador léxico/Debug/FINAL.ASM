.MODEL SMALL
.386
.STACK 2000h

.DATA
MAXTEXTSIZE equ 61
_main_contador dd ?
_main_mensaje db '$',60 dup(?)
_main_mensaje_long dd 1
_main_resultado dd ?
_main_seee dd ?
_main_a dd ?
_f1_RETURN dd ?
_f1_contador1 dd ?
_f1_resultado1 dd ?
_cte0 dd 1.0
_cte1 dd 3.0
_cte2 dd 19999.0
_cte3 dd 2.1
_cte4 db "hola",'$',26 dup(?)
_cte4_long dd 5
_cte5 dd 0.0
_cte6 dd 10.0
_cte7 dd 11.0
_cte8 dd 15.0
_cte9 dd 8.0
_cte10 dd 50.0
_cte11 dd 5.0
__ENTER          db MAXTEXTSIZE dup(?)
__NEW_LINE       db 0Dh,0Ah,'$'
_parteFrac       dd ?
_parteEntera     dd ?
_mul_10          dd 10.0
_x87_round       dw 077fh
_x87_env			db 28 dup(?)
_x87_stack0		dd ?
_x87_stack1		dd ?
_x87_stack2		dd ?
_x87_stack3		dd ?
_x87_stack4		dd ?
_x87_stack5		dd ?
_x87_stack6		dd ?
_x87_stack7		dd ?
_cadenaAux		db MAXTEXTSIZE dup(?)
_cadenaAux_long  dd 0

.CODE
	mov ax, @DATA
	mov ds, ax
	mov es, ax
	finit
	fldcw _x87_round

	fld _cte3
	fstp _main_a
	mov eax, _cte4_long
	mov _main_mensaje_long, eax
	cld
	mov esi,OFFSET _cte4
	mov edi,OFFSET _main_mensaje
	mov ecx, eax
	rep movsb
	fld _cte5
	fistp _main_contador
	fild _main_contador
	fstp _main_contador
	fld _main_contador
	fld _cte6
	mov bh, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_28
	mov bh, 000h
	mov bl, 0ffh
	and bh, bl
	jz etiqueta_166
	cld
	mov esi, OFFSET _main_mensaje
	mov edi, OFFSET __ENTER
	mov ecx, _main_mensaje_long
	rep movsb
	call IMPRIMIR
	fldz
	fld _cte7
	fld _cte5
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	ja etiqueta_39
	mov ch, 000h
	fld _cte7
	fld _cte6
	mov cl, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_45
	mov cl, 000h
	and ch, cl
	jz etiqueta_49
	fld1
	faddp
	fld _cte8
	fld _cte9
	fsubp
	fld _cte5
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	ja etiqueta_57
	mov ch, 000h
	fld _cte8
	fld _cte9
	fsubp
	fld _cte6
	mov cl, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_65
	mov cl, 000h
	and ch, cl
	jz etiqueta_69
	fld1
	faddp
	fld _cte1
	fld _cte1
	fmulp
	fld _cte5
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	ja etiqueta_77
	mov ch, 000h
	fld _cte1
	fld _cte1
	fmulp
	fld _cte6
	mov cl, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_85
	mov cl, 000h
	and ch, cl
	jz etiqueta_89
	fld1
	faddp
	fistp _main_resultado
	fild _main_resultado
	fstp _main_resultado
	mov eax, _main_resultado
	mov _parteFrac, eax
	call IMPRIMIRNUMERO
	fldz
	fld _cte7
	fld _cte5
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	ja etiqueta_98
	mov ch, 000h
	fld _cte7
	fld _cte6
	mov cl, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_104
	mov cl, 000h
	and ch, cl
	jz etiqueta_108
	fld1
	faddp
	fld _cte8
	fld _cte9
	fsubp
	fld _cte5
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	ja etiqueta_116
	mov ch, 000h
	fld _cte8
	fld _cte9
	fsubp
	fld _cte6
	mov cl, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_124
	mov cl, 000h
	and ch, cl
	jz etiqueta_128
	fld1
	faddp
	fld _cte1
	fld _cte1
	fmulp
	fld _cte5
	mov ch, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	ja etiqueta_136
	mov ch, 000h
	fld _cte1
	fld _cte1
	fmulp
	fld _cte6
	mov cl, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_144
	mov cl, 000h
	and ch, cl
	jz etiqueta_148
	fld1
	faddp
	fld _cte10
	faddp
	fld _cte1
	call f1
	fld _f1_RETURN
	fmulp
	faddp
	fld _cte11
	fld _main_a
	fmulp
	fsubp
	fstp _main_seee
	mov eax, _main_seee
	mov _parteFrac, eax
	call IMPRIMIRNUMERO
	fld _main_contador
	fld _cte0
	faddp
	fistp _main_contador
	fild _main_contador
	fstp _main_contador
	jmp etiqueta_22
	mov eax, _main_resultado
	mov _parteFrac, eax
	call IMPRIMIRNUMERO

ERROR:
	mov ax, 4C00h
	int 21h

f1:
	push ebx
	push ecx
	fstenv _x87_env
	fstp _x87_stack0
	fstp _x87_stack1
	fstp _x87_stack2
	fstp _x87_stack3
	fstp _x87_stack4
	fstp _x87_stack5
	fstp _x87_stack6
	fstp _x87_stack7

	fld _cte0
	fistp _f1_contador1
	fild _f1_contador1
	fstp _f1_contador1
	fld _cte1
	fld _cte0
	mov bh, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	ja etiqueta_9
	mov bh, 000h
etiqueta_9:
	mov bl, 0ffh
	and bh, bl
	jz etiqueta_14
	fld _cte2
	fistp _f1_contador1
	fild _f1_contador1
	fstp _f1_contador1
etiqueta_14:
	fld _f1_contador1
	fistp _f1_RETURN
	fild _f1_RETURN
	fstp _f1_RETURN

	fld _x87_stack7
	fld _x87_stack6
	fld _x87_stack5
	fld _x87_stack4
	fld _x87_stack3
	fld _x87_stack2
	fld _x87_stack1
	fld _x87_stack0
	fldenv _x87_env
	pop ebx
	pop ecx
	ret


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