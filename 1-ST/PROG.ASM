.model tiny
.code
org 100h

Start:		mov ah, 09h
		mov dx, offset Msg
		int 21h

		mov ax, 4c00h
		int 21h

Msg:		db 'hello world for the first time', 13, 10, '$'

end		Start