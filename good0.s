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
movq $2, %r9
movq $9, %r10
movq %r9, %rdi
movq %r10, %rsi
call integer_power
movq %rax, %r10
movq %r10, %rdi
call print_integer
movq $0, %r9
movq %r9, %rax
########### END OF FUNCTION BODY ###########

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx

	movq %rbp, %rsp
	popq %rbp

	ret
