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
MOVQ $45, %r9
#EXPR_CODEGEN
MOVQ $3, %r10
movq %r9, %rax
imulq %r10
movq %rax, %r10
#EXPR_CODEGEN
MOVQ y, %r9
movq %r10, %rax
imulq %r9
movq %rax, %r9
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
