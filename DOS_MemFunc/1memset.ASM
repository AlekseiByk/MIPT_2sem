.186
.model tiny
.code

include lib.asm

Start:
		mov cx, 10
		mov ax, 0025h
		mov di, offset info
		
		call memset
		.getch
		.exitDOS



;-------------------------------------------------------MEMSET
;
; set n words in destination with char
;Entry:	ax - char
;	cx - count
;	es : di - destination
; Destr: cx di
;_______________________________________________________
memset		proc
		pushf
		cld
		
		rep stosw
		popf
		ret
		
		endp
;_______________________________________________________
info:
end		Start