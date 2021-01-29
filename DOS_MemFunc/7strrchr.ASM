.186
.model tiny
.code

include lib.asm

;STR___
Start:
		mov di, offset source
		mov al, 's'
		call strchr
		.getch
		.exitDOS



;-------------------------------------------------------
; search char in the string from the end
;Entry: 
; es:di - string, end must be 00
; al - searching char
;Return: 
; es:di - first addres of symbol or di = 0ffffh if char not found in the string
;_______________________________________________________
strchr		proc
		push ax cx si
		pushf
		cld
		xor ah, ah
		
		xchg ah, al
		mov si, di
		
src:		scasb
		je endl
		jmp src
		
endl:		
		mov cx, di
		sub cx, si
		dec di
		std
		xchg al, ah
		repne scasb
		je yes		

no:		mov di, 0fffeh
yes:		inc di
		popf
		pop si cx ax
		ret
		endp
;-------------------------------------------------------
source:		db '0source for this programs', 0
dest:		db 'source not for this'	
end		Start