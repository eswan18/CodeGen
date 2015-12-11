.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp


	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

########### BODY OF FUNCTION: ###########
movq $0, %r9
movq %r9, %rcx
movq $1, %r9
movq %r9, %rdx
movq $50, %r9
movq %r9, %rsi
movq $100, %r9
movq %r9, %rdi
MOVQ $0, %rax
PUSH %r10
PUSH %r11
CALL print_it
POP %r11
POP %r10
MOVQ %rax, %r9
movq $0, %r10
movq %r10, %rax
########### END OF FUNCTION BODY ###########

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx

	movq %rbp, %rsp
	popq %rbp

	ret
.text
.globl print_it
print_it:
	pushq %rbp
	movq %rsp, %rbp

	pushq %rdi
	pushq %rsi
	pushq %rdx
	pushq %rcx

	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

########### BODY OF FUNCTION: ###########
MOVQ -8(%rbp), %r10
movq %r10, %rdi
call print_character
MOVQ -16(%rbp), %r10
movq %r10, %rdi
call print_integer
MOVQ -24(%rbp), %r10
movq %r10, %rdi
call print_boolean
MOVQ -32(%rbp), %r10
movq %r10, %rdi
call print_boolean
########### END OF FUNCTION BODY ###########

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx

	movq %rbp, %rsp
	popq %rbp

	ret
