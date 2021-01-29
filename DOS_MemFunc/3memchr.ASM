.186
.model tiny
.code

include lib.asm

;MEM___
Start:
		mov al, 'd'
		mov di, offset source
		mov cx, 12d
		
		call memchr
		.getch
		.exitDOS



;-------------------------------------------------------
; search for first byte al in the suorce array
;Entry:
; es:di - source
; cx - number of bytes to search
; ah - byte for search
;Return: 
; es:di - addres of the first matched byte if it is and di = 0FFFFh if there is no al bytes
;Destr:
;_______________________________________________________
memchr		proc
		pushf
		cld
		
		repne scasb
		je @@match
		mov di, 0000h
@@match:	dec di
		popf
		ret
		endp
;-------------------------------------------------------
source:		db 'source for this programs'
dest:		
end		Start