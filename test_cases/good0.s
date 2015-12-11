.data
x:
	.quad 0
.data
y:
	.quad 4
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
#EXPR_CODEGEN
#EXPR_CODEGEN
#EXPR_CODEGEN
movq $7, %r9
#EXPR_CODEGEN
movq $7, %r10
cmpq %r10, %r9
jg L0
movq $0, %r10
jmp L1
L0:
movq $1, %r10
L1:
cmpq $0, %r10
je L2
movq $0, %r10
jmp L3
L2:
movq $1, %r10
L3:
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
