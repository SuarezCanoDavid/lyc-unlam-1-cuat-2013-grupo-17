.DATA
MAXTEXTSIZE equ 61  ;30 + 30 + 1($)

cadena1         db  "hola ",'$',55 dup(?)   ;primera cadena de origen
_cadena1_long   dd  6                       ;longitud de la primera cadena de origen
cadena2         db  "mundo",'$',55 dup(?)   ;segunda cadena de origen
_cadena2_long   dd  6                       ;longitud de la segunda cadena de origen
mensaje         db  '$',60 dup(?)			;cadena de destino
_mensaje_long   dd  1                       ;longitud de la cadena de destino
_cadenaAux		db  MAXTEXTSIZE dup(?)      ;cadena auxiliar
_cadenaAux_long dd  0                       ;longitud de la cadena auxiliar


;verifico que se pueda realizar la concatenacion
mov ax, _cadena1_long           ;Cargo la longitud de cadena1 en ax
add ax, _cadena2_long           ;Le sumo a ax la longitud de cadena2
sub ax, 1                       ;Le resto 1 porque antes contabilicé dos veces el símbolo $
cmp ax, MAXTEXTSIZE             ;Comparo
jg  ERROR                       ;Si (_cadena1_long+_cadena2_long-1) > MAXTEXTSIZE, salto a ERROR o termino

;comienzo la concatenacion
mov _cadenaAux_long, ax			;Guardo el tamaño final que tendra cadenaAux
mov ax, _cadena1_long           ;Guardo el tamaño de cadena1 en ax
sub ax, 1                       ;Resto 1 de ax (no voy a guardar el simbolo $ de cadena1)
cld                             ;Indico que el incremento sea positivo
mov esi, cadena1                ;apunto esi al inicio de cadena1
mov edi, _cadenaAux             ;apunto edi al inicio de cadenaAux
mov ecx, ax                     ;Indico que va a tener que repetir ax veces
rep movsb                       ;copio los caracteres de cadena1 a cadenaAux (byte a byte)
mov esi, cadena2                ;apunto esi al inicio de cadena2
mov ecx, _cadena2_long          ;Indico que va a tener que repetir _cadena2_long veces
rep movsb                       ;concateno los caracteres de cadena2 a cadenaAux (byte a byte)

;asignacion
mov ax, _cadenaAux_long			;Guardo el tamaño de cadenaAux
mov _mensaje_long, ax			;Guardo el tamaño final que tendrá mensaje
cld								;Indico que el incremento sea positivo
mov esi, _cadenaAux				;apunto esi al inicio de cadenaAux
mov edi, mensaje				;apunto edi al inicio de mensaje
mov ecx, ax						;Indico que va a tener que repetir ax veces
rep movsb						;copio los caracteres de cadenaAux a mensaje (byte a byte)

