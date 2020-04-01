locals @@

.getch		macro
		nop
		xor ah, ah
		int 16h
		nop
		endm

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
.clearscr		macro
		mov al, 03h
		int 10h			;ClearScreen
		endm
.pause		macro
		nop
		push ax dx cx
		mov ax, 8600h
		mov dx, 0
		mov cx, 01h
		int 15h
		pop cx dx ax
		nop
		endm
org 100h