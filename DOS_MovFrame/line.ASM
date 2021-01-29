
.model tiny
.code

include lib.asm
SCRCENTER 	equ 0b878h
BRCORNER 	equ 188d
TLCORNER 	equ 201d
HORIZONTAL 	equ 205d
ONEROW 		equ 10d
BLCORNER	equ 200d
TRCORNER	equ 187d
VERTICAL	equ 186d
SPACE		equ 32d

Start:	
		.clearscr
		mov si, 76d		;координата x первой рамки
		mov bp, SCRCENTER
		mov dl, 4d
		mov dh, 1d
Fram:		
		mov ah, 76h		; фон и цвет тени
		call DrawFr
		sub si, 2d		
		sub bp, 10d
		mov ah, 5eh		; фон и цвет рамки
		call DrawFr
		.pause			;пауза
		
		add dh, 2d		
		add dl, 4d
		sub si, 2d

		cmp dl, 40d		;максимальная ширина рамки
		jne Fram
		
		
		.getch
		.exitDOS
		
;-------------------------------------------------------
;Draw frame
; Entry:
; si - first column
; bp - first row
; dh - hight
; dl - width
; ah - font and color
; Destr: 
;-------------------------------------------------------
DrawFr  proc
		push ax bx cx dx di bp 
		mov di, si
		mov cl, dl
		mov bx, (TLCORNER shl 8) or TRCORNER
		mov al, HORIZONTAL
		call DrawLine
		
	;------------------------------------
	;cycle
	; dh - hight
	;------------------------------------
		mov bx, (VERTICAL shl 8) or VERTICAL
		mov al, SPACE
Cycle:		
		add bp, ONEROW
		mov di, si
		mov cl, dl
		call DrawLine
		
		dec dh
		cmp dh, 0d
		jne Cycle
	;------------------------------------
		
		
		add bp, ONEROW
		mov di, si
		mov cl, dl
		mov bx, (BLCORNER shl 8) or BRCORNER
		mov al, HORIZONTAL
		call DrawLine
		
		pop bp di dx cx bx ax
		ret
		endp
;-------------------------------------------------------
;Draw top line of the frame with corners
;Entry:
; dx - start of the frame (y)
; di - start of the frame (x)
; cx - wight
; bh - left corner char
; bl - right corner char
; al - basic char
;Destr: dx, di, cx
;-------------------------------------------------------
DrawLine		proc
		push es
		
		xchg bh, al
		mov es, bp
		stosw
		xchg al, bh
		rep stosw
		xchg al, bl
		stosw
		xchg bl, al
		
		pop es
		ret
		endp
		
;-------------------------------------------------------

end		Start