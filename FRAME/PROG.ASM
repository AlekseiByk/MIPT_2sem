.model tiny
.code
org 100h

Start:		mov cx, 0b832h			;the left top corner
		mov ax, 0FCDh			; ax = ' '+ black font
Column:		mov es, cx
		mov bx, 40			
		mov word ptr es: [bx], 0FBAh	; column char
		add bx, 2

		cmp cx, 0b8beh
		jne Row
		mov ax, 0FCDh

Row:		mov word ptr es: [bx], ax
		add bx, 2
		cmp bx, 120
		jne Row
		
		mov ax, 0F20h
		
		mov bx, 120
		mov word ptr es: [bx], 0FBAh
		add cx, 10

		cmp cx, 0b8c8h
		jne Column

Corner:		mov cx, 0b832h			; draw corners
		mov es, cx
		mov byte ptr es: [40], 201
		mov byte ptr es: [120], 187
		mov cx, 0b8beh
		mov es, cx
		mov byte ptr es: [40], 200
		mov byte ptr es: [120], 188
 
		mov cx,	0b83ch
Shadow:		mov es, cx
		mov word ptr es: [122], 07BAh
		add cx, 10
		cmp cx, 0b8D2h
		jne Shadow

		mov bx, 42
Shadow2:	mov word ptr es: [bx], 07CDh
		add bx, 2
		cmp bx, 122
		jne Shadow2
		
		mov cx, 0b83ch			; shadow corners
		mov es, cx
		mov byte ptr es: [122], 187
		mov cx, 0b8c8h
		mov es, cx
		mov byte ptr es: [42], 200
		mov byte ptr es: [122], 188

		mov ax, 4c00h
		int 21h

end		Start