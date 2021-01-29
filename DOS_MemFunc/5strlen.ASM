.186
.model tiny
.code

include lib.asm

;STR___
Start:
		mov di, offset source
		
		call strlen
		.getch
		.exitDOS



;-------------------------------------------------------
; count lenght of the string
;Entry: 
; es:di - array, end must be 00
;Return: 
; ax strlen of the array
;Destr: di
;_______________________________________________________
strlen		proc
		pushf
		cld
		
		xor ax, ax
		dec di
		
src:		inc di
		inc ax
		cmp byte ptr es:[di], 00h
		jne src

		dec ax
		popf
		ret
		endp
;-------------------------------------------------------
source:		db 'source for this programs', 0
dest:		db 'source not for this'	
end		Start