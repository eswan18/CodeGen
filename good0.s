.text
.globl main
main:
	pushq %rbp
	movq %rsp, %rbp


subq $8, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

########### BODY OF FUNCTION: ###########
movq $2, %r9
movq %r9, -8(%rbp)
movq $0, %r10
movq %r10, -8(%rbp)
L0:
MOVQ -8(%rbp), %r10
movq $3, %r11
cmpq %r11, %r10
jl L2
movq $0, %r11
jmp L3
L2:
movq $1, %r11
L3:
cmpq $1, %r11
jne L1
MOVQ -8(%rbp), %r10
movq %r10, %rdi
call print_integer
jmp L0
L1:
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
