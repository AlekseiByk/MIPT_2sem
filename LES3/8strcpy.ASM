.186
.model tiny
.code

include lib.asm

;STR___
Start:
		mov si, offset source
		mov di, offset dest
		call strcpy
		mov dx, offset dest
		.toscreen
		.getch
		.exitDOS
		


;-------------------------------------------------------
; compare first cx bytes from two arrays
;Entry: 
; es:si - source string, end must be 00
; es:di - destination addres
;destroi: si di
;_______________________________________________________
strcpy		proc
		pushf
		cld
cycle:		cmp byte ptr es:[si], 00h
		je exit
		movsb
		jmp cycle
		
exit:	popf
		ret
		endp
;-------------------------------------------------------
source:		db 'source for this programs$', 0
dest:		db 'source not for this$', 0	
end		Start