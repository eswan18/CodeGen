.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp


subq $16, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

########### BODY OF FUNCTION: ###########
movq $10, %r9
movq %r9, -8(%rbp)
MOVQ -8(%rbp), %r10
movq %r10, %rdi
call print_integer
movq $10, %r10
movq %r10, %rdi
call print_character
movq $1, %r10
movq %r10, -16(%rbp)
MOVQ -16(%rbp), %r11
movq %r11, %rdi
call print_boolean
MOVQ -8(%rbp), %r11
movq %r11, %rax
########### END OF FUNCTION BODY ###########

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx

	movq %rbp, %rsp
	popq %rbp

	ret
