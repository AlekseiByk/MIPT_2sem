.186
.model tiny
.code

include lib.asm

;MEM___
Start:
		mov cx, 12
		mov si, offset source
		mov di, offset dest
		
		call memcpy
		mov dx, offset dest
		.toscreen
		.getch
		.exitDOS



;-------------------------------------------------------MEMCPY
; copy CX words from source to destination
;Entry:	ds:si - source
;	es:di - destination
;	cx - count
;Destr: cx di
;_______________________________________________________
memcpy		proc
		pushf
		cld
		
		rep movsb
		popf
		ret
		endp
;-------------------------------------------------------
source:		db 'destination$'
dest:		
end		Start