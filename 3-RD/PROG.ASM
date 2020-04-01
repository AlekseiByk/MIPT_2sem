.model tiny
.code
org 100h

Start:		mov bx, 0b800h
		mov es, bx
		mov bx, (25/2*80 + 80/2)*2
		mov byte ptr es: [bx], 65
		inc bx
		mov byte ptr es: [bx], 4eh

		mov ax, 4c00h
		int 21h

end		Start