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
########### END OF FUNCTION BODY ###########
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbx
	movq %rbp, %rsp
	popq %rbp
	ret
