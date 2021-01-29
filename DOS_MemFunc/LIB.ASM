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