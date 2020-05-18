## Function optimization based on hash-table from 1 sem
Fyrstly I ran hash-tably with: `time ./hash_table`
and it showed 2500sec, thats bad... and I rewrote some functions and add function that do pointers to words.

```
char** Create_words(char *text, int* wNumber){
    assert(text != nullptr);

    char** index = (char**) calloc(500000, sizeof(index[0]));
    assert(index != nullptr);

    index[0] = text;

    int k = 0;
    while ((*text)!= '\0')
    {
        if (!(isalpha(*text) || isdigit(*text)))
        {
            *text = '\0';

            k++;
            index[k] = text + 2;

            text++;
        }
        text++;
    }

  	(*wNumber) = k + 1;

    return index;
}
```

Than made a cicle and...
```
for (int i = 0; i < 400; i++)
        FillHashTable(Index, wNumber, &HashTable, HashSolve_Rol);
```
... ran again with `time`.

It showed 36.7 sec.

Ran callgrind(`valgrind --tool=callgrind ./hash`)
and:

![alt-текст](https://github.com/AlekseiByk/assembler/blob/master/hash_optimization/before.png "Kcachegrind befor asm optimization")

I saw that i need to rewrite `hash_solve` and `hash_search`.

Compiled with key `-S`(`gcc -S Hash_table.c Hash.s`) to get assembler code of the program.

Very scary? but not difficult.

Firstly `hash_solve_ROL`:

```
_Z13HashSolve_RolPc:
.LFB24:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
.L51:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L49
	movl	-8(%rbp), %eax
	jmp	.L52
.L49:
	movl	-8(%rbp), %eax
	roll	$13, %eax
	movl	%eax, %ecx
	movl	-4(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -4(%rbp)
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	xorl	%ecx, %eax
	movl	%eax, -8(%rbp)
	jmp	.L51
.L52:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
```
	
After some lines of code It turned into this:
   
```
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
```
   
And `Hash_Search`:
   
```
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
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rcx
	movq	-24(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, %esi
	movl	-36(%rbp), %eax
	movl	$0, %edx
	divl	%esi
	movl	%edx, %eax
	movl	%eax, %edx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$4, %rax
	addq	%rcx, %rax
	movq	%rax, -8(%rbp)
.L30:
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	testq	%rax, %rax
	je	.L27
	movq	-8(%rbp), %rax
	leaq	4(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L28
	movl	$0, %eax
	jmp	.L29
.L28:
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, -8(%rbp)
	jmp	.L30
.L27:
	movq	-8(%rbp), %rax
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
```
    
turned into:
   
```
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
```
    
Ok, ran with `time` and `callgrind` and the results are as follows:

time: 24sec

and Kcachegrind:

![alt-текст](https://github.com/AlekseiByk/assembler/blob/master/hash_optimization/after.png "Kcachegrind after asm optimization")

as we can see `hash_solve` became 3 times faster and `hash_search` became 1.7 times faster

and 

## 40% optimization for all program
and minus 30 lines of assemble code
    

