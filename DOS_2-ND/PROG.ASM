.model tiny
.code
org 100h

Start:		mov ah, 0Eh
		mov si, offset Msg

		mov al, [si]
Next:		int 10h
		
		inc si
		mov al, [si]
		cmp al, '$'
		jne Next

		mov ax, 4c00h
		int 21h

Msg:		db 'hello World', 13, 10, '$'

end		Start