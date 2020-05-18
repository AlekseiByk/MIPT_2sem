.186
.model tiny
.code

include lib.asm

org 100h

FONT			equ 4eh
;------------------------------------------------------	
;	write value of the register reg
;------------------------------------------------------	
.Dreg	macro reg
		nop
		push bx				;сохраняем bx
		mov bx, reg			;помещаем в bx что хотим вывести
		call DrawReg		;вывод
		pop bx
		nop		
		endm
;------------------------------------------------------	
;------------------------------------------------------	
; write two chars and move cursor to the next row


;------------------------------------------------------	
.writer	macro let1, let2
		nop
		
		push ax
		mov ax, (FONT shl 8) or 32d			;пробел
		stosw
		mov ax, (FONT shl 8) or let2		;сначала второй символ
		stosw
		mov ax, (FONT shl 8) or let1		;потом первый
		stosw
		pop ax
		add di, 80*2 + 14					;
		
		nop
		endm
;-------------------------------------------------------
Start:
;=======================================================
;interupt	

		xor ax, ax
		mov es, ax
		
		
		
		mov bx, 9 * 4			; адрес прерывания 8*4 -таймер
		
		mov ax,   es:[bx]
		mov Old09, ax
		mov ax, es:[bx+2]
		mov Old09+2, ax
		
		cli
		mov ax, cs
		mov word ptr es:[bx], offset New09
		mov word ptr es:[bx + 2], ax
		sti
		mov ah, 31h
		mov dx, offset TheEnd
		shr dx, 4
		inc dx
		int 21h
;==================================================================================================================================
;==================================================================================================================================
;==================================================================================================================================
New09	proc
		push ax
		in al, 60h


		cmp al, 80h
		ja endreg
		cmp byte ptr ds:[register + 4], 01h
		je repiet
		
		pop ax
		
		push di es si bp dx cx ax
		
		mov byte ptr ds:[mem], 01h
		mov byte ptr ds:[register + 4], 01h
		xor bp, bp
		
		mov dx, 0b81ch	
cl1:	mov es, dx		;верхний левый угол рамки
		mov di, 14	
		mov cx, 9
tomem:	mov ax, word ptr es:[di]
		mov word ptr ss:[bp + 100], ax
		add bp, 2
		add di, 2
		loop tomem
		add dx, 10d
		cmp dx, 0b81ch + 10d*15d
		jb cl1
		
		
		mov di, 0b800h			;видеопамять
		mov es, di
		mov di, 80*3*2 - 18		;верхний левый угол рамки
		mov dl, 9d
		mov dh, 13d
		mov ah, FONT
		call DrawFr				;нарисовать рамку
		
		pop ax cx dx bp si es di
		call DrawAllReg
		jmp	jump
		
repiet:
		pop ax
		call DrawAllReg
		jmp	jump
		
endreg:
		pop ax
		cmp byte ptr ds:[mem], 01h
		jne jump
		
		cmp byte ptr ds:[mem], 00h
		mov byte ptr ds:[register + 4], 00h
		push es di bp dx cx ax
		xor bp, bp
		mov dx, 0b81ch			;видеопамять	
cl2:	mov es, dx
		mov di, 14		;верхний левый угол рамки
		mov cx, 9
frommem:	mov ax, word ptr ss:[bp + 100]
		mov word ptr es:[di], ax
		add bp, 2
		add di, 2
		loop frommem
		add dx, 10d
		cmp dx, 0b81ch + 10d*15d
		jb cl2

		
		pop ax cx dx bp di es
		
		
jump:
		db 0eah
Old09	dw 0
		dw 0
	;	iret
		endp
;==================================================================================================================================
;==================================================================================================================================
;==================================================================================================================================
;		.getch
;		.exitDOS

;----------------------------------------------------------------------------------
DrawAllReg proc
		push di es
		
		mov word ptr ds:[offset register], di 
		mov word ptr ds:[offset register + 2], es ;сохранили в памяти
		mov di, 0b800h			;видеопамять
		mov es, di
		mov di, 80*4*2 - 4		;левый верхний угол регистров
		std
		.Dreg ax
		.writer 'a', 'x'
		.Dreg bx
		.writer 'b', 'x'
		.Dreg cx
		.writer 'c', 'x'
		.Dreg dx
		.writer 'd', 'x'
		.Dreg si
		.writer 's', 'i'
		push bx
		mov word ptr bx, ds:[offset register]
		call DrawReg
		pop bx
		.writer 'd', 'i'
		.Dreg bp
		.writer 'b', 'p'
		.Dreg sp
		.writer 's', 'p'
		.Dreg cs
		.writer 'c', 's'
		.Dreg ds
		.writer 'd', 's'
		.Dreg ss
		.writer 's', 's'
		push bx
		mov word ptr bx, ds:[offset register + 2]
		call DrawReg
		pop bx
		.writer 'e', 's'
		
		push bx
		pushf		;флаги в bx
		pop bx
		.Dreg bx
		.writer 'f', 'l'
		pop bx
		cld
		
		pop es di
		endp
;--------------------------------------------------------------------------------
;-------------------------------------------------------
; printf register 
; bx - register to print
;-------------------------------------------------------
Drawreg		proc
		push cx bx ax
		xor cx, cx

@@cycle:	mov ax, bx	; записываем число которое выводим
		shr ax, cl ; сдвигаем на сl бит
		and ax, 000fh ; оставляем 1/2 байта
		call Donumber
		mov ah, FONT	
		stosw
		
		add cl, 4d	; 4 - 1/2 байта
		cmp cl, 16d
		jne @@cycle

		pop ax bx cx
		ret
		endp
;-------------------------------------------------------
;-------------------------------------------------------
; конвертирует число для вывода на экран
;Entry:
; al - число(0xh), x = 1234567890abcdef
;Ret:
; al - aiscii code of the char
;-------------------------------------------------------
Donumber		proc
		cmp byte ptr al, 10d
		jb number
		add al, 'A' - '0' - 10d
number:		add al, '0'
		ret
		endp
;-------------------------------------------------------

;-------------------------------------------------------
;Draw frame
; Entry:
; es - videomemory paragraph
; di - top left corner from 0b800h
; bp - first row
; dh - hight
; dl - width
; ah - font and color
; Destr: 
;-------------------------------------------------------
DrawFr  proc
		push bx cx
		xor cx, cx
		mov cl, dl
		mov bx, (TLCORNER shl 8) or TRCORNER
		mov al, HORIZONTAL
		sub cl, 2
		call DrawLine
		
	;------------------------------------
	;cycle
	; dh - hight
	;------------------------------------
		mov bx, (VERTICAL shl 8) or VERTICAL
		mov al, SPACE
Cycle:		
		add di, ONEROW
		push dx
		xor dh, dh
		sub di, dx
		sub di, dx
		mov cl, dl
		sub cl, 2
		call DrawLine
		pop dx
		dec dh
		cmp dh, 0d
		jne Cycle
	;------------------------------------
		
		add di, ONEROW
		sub di, dx
		sub di, dx
		mov cl, dl
		sub cl, 2
		mov bx, (BLCORNER shl 8) or BRCORNER
		mov al, HORIZONTAL
		call DrawLine
		
		pop cx bx
		ret
		endp
;-------------------------------------------------------
;Draw line of the frame with corners
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
		
		xchg bh, al
		stosw
		xchg al, bh
		rep stosw
		xchg al, bl
		stosw
		xchg bl, al
		
		ret
		endp
		
;-------------------------------------------------------

register:	db '    ',0,0,'                '
mem:	db 1000 dup (00h)
theEnd:
end		Start