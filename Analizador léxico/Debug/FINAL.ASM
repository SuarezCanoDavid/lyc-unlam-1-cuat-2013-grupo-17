include  number.asm
include  macros2.asm
.MODEL SMALL
.386
.STACK 200h

.DATA
MAXTEXTSIZE equ 61
_main_contador dd ?
_main_mensaje db '$',60 dup(?)
_main_mensaje_long dd 1
_main_palabra db '$',60 dup(?)
_main_palabra_long dd 1
_main_i dd ?
_getContadorLocal_RETURN dd ?
_getContadorLocal_contador dd ?
_cte0 dd 5.0
_getContadorGlobal_RETURN dd ?
_getHola_RETURN db '$',60 dup(?)
_getHola_RETURN_long dd 1
_cte1 dd 4.0
_cte2 dd 8.0
_cte3 db "hola",'$',26 dup(?)
_cte3_long dd 5
_cte4 db "chau",'$',26 dup(?)
_cte4_long dd 5
_cte5 dd 6.0
_cte6 dd 0.0
_cte7 db "Contador global: ",'$',13 dup(?)
_cte7_long dd 18
_cte8 dd 10.0
_cte9 dd 1.0
_cte10 db "Contador local: ",'$',14 dup(?)
_cte10_long dd 17
_cte11 db " mundo",'$',24 dup(?)
_cte11_long dd 7
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
realAltura_MAIN  dd 0
strReadString		db MAXTEXTSIZE dup(?)

.CODE
	mov ax, @DATA
	mov ds, ax
	mov es, ax
	finit
	fldcw _x87_round

	fld _cte5
	fistp _main_contador
	fild _main_contador
	fstp _main_contador
	fld _cte6
	fistp _main_i
	fild _main_i
	fstp _main_i
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	 MOV DX,OFFSET _cte7
	 MOV Ah,9 
	 INT 21h
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	fld _cte6
	fistp _main_i
	fild _main_i
	fstp _main_i
etiqueta_36:
	fld _main_i
	fld _cte8
	mov bh, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_42
	mov bh, 000h
etiqueta_42:
	mov bl, 0ffh
	and bh, bl
	jz etiqueta_52
	call getContadorGlobal
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	displayFloat  _getContadorGlobal_RETURN , 0
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	fld _main_i
	fld _cte9
	faddp
	fistp _main_i
	fild _main_i
	fstp _main_i
	jmp etiqueta_36
etiqueta_52:
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	 MOV DX,OFFSET _cte10
	 MOV Ah,9 
	 INT 21h
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	fld _cte6
	fistp _main_i
	fild _main_i
	fstp _main_i
etiqueta_55:
	fld _main_i
	fld _cte8
	mov bh, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_61
	mov bh, 000h
etiqueta_61:
	mov bl, 0ffh
	and bh, bl
	jz etiqueta_71
	call getContadorLocal
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	displayFloat  _getContadorLocal_RETURN , 0
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	fld _main_i
	fld _cte9
	faddp
	fistp _main_i
	fild _main_i
	fstp _main_i
	jmp etiqueta_55
etiqueta_71:
	fld _cte6
	fistp _main_contador
	fild _main_contador
	fstp _main_contador
etiqueta_73:
	call getContadorGlobal
	fld _getContadorGlobal_RETURN
	fld _cte8
	mov bh, 0ffh
	fxch
	fcompp
	fstsw ax
	sahf
	jb etiqueta_80
	mov bh, 000h
etiqueta_80:
	mov bl, 0ffh
	and bh, bl
	jz etiqueta_93
	call getHola
	mov eax, _cte11_long
	add eax, _getHola_RETURN_long
	sub eax, 1
	cmp eax, MAXTEXTSIZE
	jg  ERROR
	mov _cadenaAux_long , eax
	cld
	mov esi, OFFSET _getHola_RETURN
	mov edi, OFFSET _cadenaAux
	mov ecx, _getHola_RETURN_long
	sub ecx, 1
	rep movsb
	mov esi, OFFSET _cte11
	mov ecx, _cte11_long
	rep movsb
	mov eax, _cadenaAux_long
	mov _main_mensaje_long, eax
	cld
	mov esi, OFFSET _cadenaAux
	mov edi, OFFSET _main_mensaje
	mov ecx, eax
	rep movsb
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	 MOV DX,OFFSET _main_mensaje
	 MOV Ah,9 
	 INT 21h
	mov dx, OFFSET __NEW_LINE
	mov ah, 9
	int 21h
	call getContadorGlobal
	fld _getContadorGlobal_RETURN
	fld _cte9
	faddp
	fistp _main_contador
	fild _main_contador
	fstp _main_contador
	jmp etiqueta_73
etiqueta_93:

ERROR:
	mov ax, 4C00h
	int 21h

getContadorLocal:
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
	fistp _getContadorLocal_contador
	fild _getContadorLocal_contador
	fstp _getContadorLocal_contador
	fld _getContadorLocal_contador
	fistp _getContadorLocal_RETURN
	fild _getContadorLocal_RETURN
	fstp _getContadorLocal_RETURN

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

getContadorGlobal:
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

	fld _main_contador
	fistp _getContadorGlobal_RETURN
	fild _getContadorGlobal_RETURN
	fstp _getContadorGlobal_RETURN

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

getHola:
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

	fld _main_contador
	fld _cte1
	mov bh, 0ffh
	fcompp
	fstsw ax
	sahf
	je etiqueta_15
	mov bh, 000h
etiqueta_15:
	fld _main_contador
	fld _cte2
	mov bl, 0ffh
	fcompp
	fstsw ax
	sahf
	je etiqueta_21
	mov bl, 000h
etiqueta_21:
	or bh, bl
	jz etiqueta_25
	mov eax, _cte3_long
	mov _main_palabra_long, eax
	cld
	mov esi,OFFSET _cte3
	mov edi,OFFSET _main_palabra
	mov ecx, eax
	rep movsb
	jmp etiqueta_26
etiqueta_25:
	mov eax, _cte4_long
	mov _main_palabra_long, eax
	cld
	mov esi,OFFSET _cte4
	mov edi,OFFSET _main_palabra
	mov ecx, eax
	rep movsb
etiqueta_26:
	mov eax, _main_palabra_long
	mov _getHola_RETURN_long, eax
	cld
	mov esi,OFFSET _main_palabra
	mov edi,OFFSET _getHola_RETURN
	mov ecx, eax
	rep movsb

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