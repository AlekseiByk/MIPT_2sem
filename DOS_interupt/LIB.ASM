locals @@

;--------------
;wait keyboard
;
;--------------

.getch		macro
		nop
		push ax
		
		xor ah, ah
		int 16h
		
		pop ax
		nop
		endm
		
;--------------
;exit DOS program with code ret or 0
;
;--------------
.exitDOS	 	macro ret
		nop
		
	ifnb <ret>
		mov ax, 4c00h or (ret and 0ffh)
	else
		mov ah, 4ch
	endif

		int 21h
		
		nop
		endm
		
;--------------	
; print chars until $ ,dx - source array
;
;--------------	

.toscreen macro
		nop
		push ax
		
		xor ax, ax
		mov ah, 09h
		int 21h
		
		pop ax
		nop
		endm
org 100h

SCRCENTER		equ 0b878h
TLCORNER		equ 201d
TRCORNER		equ 187d
BLCORNER		equ 200d
BRCORNER		equ 188d
HORIZONTAL		equ 205d
VERTICAL		equ 186d
SPACE		equ 32d
ONEROW		equ 80*2