.globl	_Z10hashSearchP12hash_table_tPcj
	.type	_Z10hashSearchP12hash_table_tPcj, @function
_Z10hashSearchP12hash_table_tPcj:
.LFB19:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	
	movq	%rsi, -32(%rbp)

		movq	%rdi, %rax
		movq	(%rax), %rcx

		movl	8(%rax), %esi
	
	movl	%edx, %eax
	xor %rdx, %rdx
	divl	%esi


	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movq	%rax, %r10

	movq	-32(%rbp), %rdi
.L30:

	movq	40(%r10), %rax
	testq	%rax, %rax
	je	.L27

	leaq	4(%r10), %rsi

	call	strcmp@PLT

	testl	%eax, %eax
	jne	.L28
	xor	%eax, %eax
	jmp	.L29
.L28:

	movq	40(%r10), %r10
	jmp	.L30
.L27:
	movq	%r10, %rax
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	_Z10hashSearchP12hash_table_tPcj, .-_Z10hashSearchP12hash_table_tPcj


	.globl	_Z13HashSolve_RolPc
	.type	_Z13HashSolve_RolPc, @function
_Z13HashSolve_RolPc:
.LFB24:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	xor %ecx, %ecx
	jmp .L50	
.L49:
	roll	$13, %ecx
	
	xorl	%eax, %ecx
	inc %rdi
.L50:
	movzbl	(%rdi), %eax
	testb	%al, %al
	jne	.L49
	movl	%ecx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	_Z13HashSolve_RolPc, .-_Z13HashSolve_RolPc
