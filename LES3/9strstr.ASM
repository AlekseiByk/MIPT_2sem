.186
.model tiny
.code

include lib.asm

;STR___
Start:
		mov si, offset source
		mov di, offset dest
		call strstr
		.getch
		.exitDOS



;-------------------------------------------------------
; search for string 2 in the string 1
;Entry: 
; es:si - where search
; es:di - what to search
;Return: 
; es:si - first addres of symbol or di = 0ffffh if char not found in the string
; Destr: 
;_______________________________________________________
strstr		proc
		push ax
		pushf
		cld
		mov ax, di

comp:		cmp byte ptr es:[di], 00h
		je exit
		cmp byte ptr es:[si], 00h
		je notf
		cmpsb
		je comp
		
		mov di,ax
		jmp comp
		
notf:	mov si, 0ffffh
		pop ax
		ret
exit:		sub si, di
		add si, ax
		popf
		pop ax
		ret
		endp
;-------------------------------------------------------
source:		db 'source for this programs', 0
dest:		db 'fork', 0	
end		Start