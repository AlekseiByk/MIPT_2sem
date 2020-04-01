.186
.model tiny
.code

include lib.asm

;MEM___
Start:
		mov al, 'd'
		mov di, offset source
		mov si, offset dest
		mov cx, 9d
		
		call memcmp
		.getch
		.exitDOS



;-------------------------------------------------------
; compare first cx bytes from two arrays
;Entry: 
; es:di - first array
; es:si - second array
; cx - number of bytes to search
;Return: 
; ax = 0 arrays are equal
; ax = 1 if the first is above
; ax = -1 if second above
;Destr: di si cx 
;_______________________________________________________
memcmp		proc
		pushf 
		cld
		repe cmpsb
		je equal
		jb above
		mov ax, -1
		ret
equal:	mov ax, 0
		ret
above:	mov ax, 1
		popf
		ret
		endp
;-------------------------------------------------------
source:		db 'source for this programs'
dest:		db 'source not for this'	
end		Start