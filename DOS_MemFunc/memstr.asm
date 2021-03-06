;-------------------------------------------------------MEMSET
;
; set n words in destination with char
;Entry:	ax - char
;	cx - count
;	es : di - destination
; Destr: cx di
;_______________________________________________________
memset		macro
		cld
		rep stosw
		ret
		
		endm
;_______________________________________________________


;-------------------------------------------------------MEMCPY
; copy CX words from source to destination
;Entry:	ds:si - source
;	es:di - destination
;	cx - count
;Destr: cx di si
;_______________________________________________________
memcpy		macro
		cld
		rep movsb
		ret
		endm
;-------------------------------------------------------

;-------------------------------------------------------MEMCHR
; search for first byte al in the suorce array
;Entry:
; es:di - source
; cx - number of bytes to search
; ah - byte for search
;Return: 
; es:di - addres of the first matched byte if it is and di = 0FFFFh if there is no al bytes
;Destr: cx
;_______________________________________________________
memchr		proc
		cld
		
		repne scasb
		je @@match
		mov di, 0000h
@@match:	dec di
		ret
		endp
;-------------------------------------------------------


;-------------------------------------------------------MEMCMP
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
		cld
		repe cmpsb
		mov al, byte ptr es:[di - 1]
		sub al, byte ptr es:[si - 1]
		ret
;		je equal
;		jb above
;		mov ax, -1
;		ret
;equal:	mov ax, 0
;		ret
;above:	mov ax, 1
;		ret
		endp
;-------------------------------------------------------


;-------------------------------------------------------STRLEN
; count lenght of the string
;Entry: 
; es:di - array, end must be 00
;Return: 
; ax strlen of the array
;Destr: di
;_______________________________________________________
strlen		proc
		cld
		
		xor ax, ax
		dec di
		
src:		inc di
		inc ax
		cmp byte ptr es:[di], 00h
		jne src

		dec ax
		ret
		endp
;-------------------------------------------------------


;-------------------------------------------------------STRCHR
; search char in the string
;Entry: 
; es:di - string, end must be 00
; al - searching char
;Return: 
; es:di - first addres of symbol or di = 0ffffh if char not found in the string
;_______________________________________________________
strchr		proc
		cld
		
src:	cmp byte ptr es:[di], 00h
		je no
		scasb		
		jne src
		
		dec di
		ret
		
no:		mov di, 0ffffh
		ret
		endp
;-------------------------------------------------------


;-------------------------------------------------------STRRCHR
; search char in the string from the end
;Entry: 
; es:di - string, end must be 00
; al - searching char
;Return: 
; es:di - first addres of symbol or di ...= 0ffffh if char not found in the string
; destr: ah si cx
;_______________________________________________________
strrchr		proc
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

no:		mov di, 0ffffh
		ret
yes:		inc di
		ret
		endp
;-------------------------------------------------------


;-------------------------------------------------------STRCPY
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



;-------------------------------------------------------STRSTR
; search for string 2 in the string 1
;Entry: 
; es:si - where search
; es:di - what to search
;Return: 
; es:si - first addres of symbol or di = 0ffffh if char not found in the string
; Destr: di si ax
;_______________________________________________________
strstr		proc
		cld
		mov ax, di

comp:		cmp byte ptr es:[di], 00h
		je exit
		cmp byte ptr es:[si], 00h
		je notf
		cmpsb
		je comp
		
		mov di,ax
		jmp comp
		
notf:	mov si, 0ffffh
		ret
		
exit:		sub si, di
		add si, ax
		ret
		endp
;-------------------------------------------------------