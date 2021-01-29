.186
.model tiny
.code

include lib.asm

;STR___
Start:
		mov di, offset source
		mov al, 'c'
		call strchr
		.getch
		.exitDOS



;-------------------------------------------------------
; search char in the string
;Entry: 
; es:di - string, end must be 00
; al - searching char
;Return: 
; es:di - first addres of symbol or di = 0ffffh if char not found in the string
;_______________________________________________________
strchr		proc
		cld
		
src:	cmp byte ptr es:[di], 00h
		je no
		scasb		
		jne src
		
		dec di
		ret
		
no:		mov di, 0ffffh
		ret
		endp
;-------------------------------------------------------
source:		db 'source for this programs', 0
dest:		db 'source not for this'	
end		Start