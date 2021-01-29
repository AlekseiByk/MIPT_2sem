section .text

global _start
;--------------------------------------------------------
;desplay string from the stack on cmd
;input: rdx - number of pushed chars

;destr: rax, rdi, rdx, rsi
;--------------------------------------------------------
%macro  PUTSTRINGFROMSTACK 0
nop

		shl rdx, 3
		mov rax, 0x01      
		mov rdi, 1         ; stdout
		mov rsi, rsp		;указатель на стэк
		syscall
		add rsp, rdx

nop
%endmacro
;---------------------------------------------------------

;---------------------------------------------------------
;выводит числа в 2кратных системах
;input: rax - number to display
;		r11 - mask
;		cl - number of bites to shr(power of 2)
;destr: 	rax, r10, rdx, rdi, rsi
;---------------------------------------------------------

to2x:	

		xor rdx, rdx
.lp:	
		mov r10, rax
		and rax, r11 		;mask
		add rdx, 1
		mov al, [numberCode + rax]
	    push rax
		mov rax, r10
		shr rax, cl
		cmp rax, 0
		jne .lp

		PUTSTRINGFROMSTACK

		ret

;---------------------------------------------------------

;---------------------------------------------------------
;display binary numbers
;input: rax - number to display
;destr: rax, r10, rdx, rdi, rsi, rcx, r11
;---------------------------------------------------------

tobin:

		mov r11, 0x01
		mov cl, 1
		call to2x
		ret

;---------------------------------------------------------

;---------------------------------------------------------
;display oct numbers
;input: rax - number to display
;destr: rax, r10, rdx, rdi, rsi, rcx, r11
;---------------------------------------------------------

tooct:

		mov r11, 0x07
		mov cl, 3
		call to2x
		ret

;---------------------------------------------------------

;---------------------------------------------------------
;display hexidecimal numbers
;input: rax - number to display
;destr: rax, r10, rdx, rdi, rsi, rcx, r11
;---------------------------------------------------------


tohex:

		mov r11, 0x0f
		mov cl, 4
		call to2x
		ret

;---------------------------------------------------------

;---------------------------------------------------------
;display decimal numbers
;input: rax - number to display
;destr: rax, r10, rdx, rdi, rsi, rcx
;---------------------------------------------------------

todec:
		mov rcx, 1
		cmp rax, 0
		jns	.lp
		neg rax
		mov r10, '-'

.lp:	xor rdx, rdx
		mov ebx, 10
		div ebx
		mov dl, [numberCode + rdx]
		push rdx
		inc rcx
		cmp rax, 0
		jne .lp

		push r10
		mov rdx, rcx
		PUTSTRINGFROMSTACK

		ret

;---------------------------------------------------------

;---------------------------------------------------------
;display char 
;input: rax - char to display
;destr: rax, rdx, rdi, rsi
;---------------------------------------------------------

printchar:
	mov rdx, 1
	push rax
	PUTSTRINGFROMSTACK
	ret
;---------------------------------------------------------

;---------------------------------------------------------
;display string
;input: rsi - pointer to string
;destr: rax, rdx, rdi, rsi
;---------------------------------------------------------

printstr:
		xor rdx, rdx				;strlen
.lp:	
		inc rdx
		cmp byte [rsi + rdx], 0  
		jne .lp
	
		mov rax, 0x01
		mov rdi, 1         ; stdout
		syscall
		ret

;---------------------------------------------------------

;---------------------------------------------------------
;printf function
;input: arguments in the steck from right to left and on the top of the stack pointer to format string
;destr: rax, rdx, rdi, rsi, r15 , r8
;---------------------------------------------------------
printf:
		pop r15	;return adress

		pop r8
.loop:	
		cmp byte [r8], 0
		je .endfunc

		cmp byte [r8], '%'
		je .proc

		mov byte al, [r8]
		call printchar

		jmp .endproc

.proc:

		inc r8
		cmp byte [r8], '%'		;%% 	rax - "%"
		jne .bin
		mov rax, '%'
		call printchar
		jmp .endproc

.bin:

		cmp byte [r8], 'b'		;%b 	rax - number
		jne .oct
		pop rax
		call tobin
		jmp .endproc

.oct:

		cmp byte [r8], 'o'		;%o 	rax - number
		jne .hex
		pop rax
		call tooct
		jmp .endproc

.hex:

		cmp byte [r8], 'x'		;%x 	rax - number
		jne .dec
		pop rax
		call tohex
		jmp .endproc

.dec:

		cmp byte [r8], 'd'		;%dis 	rax - number
		jne .str
		pop rax
		call todec
		jmp .endproc

.str:

		cmp byte [r8], 's'		;%s    	rsi - pointer
		jne .char
		pop rsi
		call printstr
		jmp .endproc

.char:
		cmp byte [r8], 'c'		;%c 	rax - char
		jne .endfunc
		pop rax
		call printchar


.endproc:	inc r8
			jmp .loop

.endfunc:	push r15
			ret

;---------------------------------------------------------
_start:   		
	push 127
	push '!'
	push 100
	push 3802
	push Firstarg
	push 1300d
	push 228d

	push 0edah
	push Firstarg
	push 100b
	push 'I'
	push Teststring
	
	call printf
    mov rax, 0x3C      ; exit64 (rdi)
    xor rdi, rdi
    syscall
;------------------------------------------------------------

section .data
numberCode: db "0123456789ABCDEF"

Teststring: db '%c absolutely, %b%%, %s %x! %d, %o', 10,'and I %s %x %d%%%c%b', 10, 0
Firstarg: db "Love", 0
;Second arg: db ""
;------------------------------------------------------------------------------------------
