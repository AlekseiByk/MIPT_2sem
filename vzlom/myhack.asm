.186
.model tiny
.code

org 100h
;----------------------------------------------------------
Start:	
		mov ah, 09h
		mov dx, offset input
		int 21h
		
		mov dx, offset pas
		mov ah, 0ah
inp:		int 21h
		
strt:		xor cx, cx
		xor bp, bp
		mov cl, [bp + offset pas + 1]
		inc cl
		xor bp, bp
loop1:		
		mov byte ptr ah, [bp]
		shr ah, 2
		xor [bp + offset pas + 2], ah
		inc bp
		loop loop1
		
		xor bp, bp		
		
		mov ax, 0000h
		mov byte ptr cl, 3
		mov bp, cx
loop3:	
		mov ah, [bp + offset pas + 2]
		cmp byte ptr [bp + offset paasok], ah
		jne yes
		dec bp
		loop loop3
		mov ax, 0edah

	
yes:
		cmp ax, 0edah
		jne mess
		
		mov ah, 09h
		
		mov dx, offset good
		int 21h
		
		mov dx, offset allo
		int 21h
		jmp close
		
mess:
		mov ah, 09h
		
		mov dx, offset inv
		int 21h
		
		mov dx, offset acc
		int 21h


close:
		mov ax, 4c00h
		int 21h		

;----------------------------------------------------------
pas:	db 38, 30 dup (0h)
paasok:	db 71h, 49h, 6ch, 2Ah,0,0,0,0,0
input:	db 'Hello, please input password:', 13, 10, '$'
pas1:	db 'password', 13, 10, '$'
good:	db 'correct password', 13, 10, '$'
acc:	db 'access denied', 13, 10, '$'
allo:	db 'access allowed', 13, 10, '$'
inv:	db 'incorrect password', 13, 10, '$'
end		Start