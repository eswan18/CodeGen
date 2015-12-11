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
movq $4, %r9
movq %r9, -8(%rbp)
MOVQ -8(%rbp), %r10
movq $4, %r11
cmpq %r11, %r10
jl L2
movq $0, %r11
jmp L3
L2:
movq $1, %r11
L3:
movq $1, %r10
cmpq $0, %r11
je L4
cmpq $0, %r10
je L4
movq $1, %r11
jmp L5
L4:
movq $0, %r11
L5:
cmpq $1, %r11
jne L0
.data
STR0:
.string "yes
"
.text
leaq STR0, %r10
movq %r10, %rdi
call print_string
jmp L1
L0:
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
