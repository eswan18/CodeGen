.data
x:
	.quad 0
.data
y:
	.quad 4
.data
z:
	.quad 0
STR0:
	.string "hi"
.text
leaq STR0, %r9
movq %r9, z
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
movq $400, %r9
movq $20, %r10
movq %r9, %rax
imulq %r10
movq %rax, %r10
movq %r10, %rdi
call print_integer
MOVQ z, %r9
movq %r9, %rdi
call print_string
.data
STR1:
.string "hi"
.text
leaq STR1, %r9
movq %r9, %rdi
call print_string
movq $1, %r9
movq $1, %r10
cmpq $1, %r9
je L0
cmpq $1, %r10
je L0
movq $0, %r9
jmp L1
L0:
movq $1, %r9
L1:
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
