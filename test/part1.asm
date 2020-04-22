	.text	
	.globl		main
main:
	li		$t0, 10
	sw		$t0, num1
	li		$t0, 4
	sw		$t0, num2
	lw		$t0, num1
	lw		$t1, num2
	add		$t2, $t0, $t1
	sw		$t2, num1
	lw		$t0, num1
	lw		$t1, num2
	sub		$t2, $t0, $t1
	sw		$t2, num1
	lw		$t0, num1
	lw		$t1, num2
	mul		$t2, $t0, $t1
	sw		$t2, num1
	lw		$t0, num1
	lw		$t1, num2
	div		$t2, $t0, $t1
	sw		$t2, num1
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num1
	li		$t1, 1
	li		$t2, -1
	mul		$t3, $t1, $t2
	add		$t1, $t0, $t3
	sw		$t1, num1
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num1
	li		$t1, 2
	addi		$t3, $zero, 1
	addi		$t2, $zero, 0
L1:
	beq		$t2, $t1, L2
	mul		$t3, $t0, $t3
	addi		$t2, $t2, 1
	j		L1
L2:
	sw		$t3, num1
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	lw		$t0, num1
	li		$t1, 100
	slt		$t3, $t0, $t1
	li		$t2, 1
	bne		$t3, $t2, L3
	li		$t0, 1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L3:
	lw		$t0, num1
	li		$t1, 100
	slt		$t3, $t1, $t0
	li		$t2, 1
	bne		$t3, $t2, L4
	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L4:
	lw		$t0, num1
	li		$t1, 100
	bne		$t0, $t1, L5
	li		$t0, 3
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L5:
	lw		$t0, num1
	li		$t1, 50
	slt		$t3, $t1, $t0
	li		$t2, 0
	bne		$t3, $t2, L6
	li		$t0, 4
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L6:
	lw		$t0, num1
	li		$t1, 100
	slt		$t3, $t0, $t1
	li		$t2, 0
	bne		$t3, $t2, L7
	li		$t0, 5
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L7:
	lw		$t0, num1
	li		$t1, 0
	beq		$t0, $t1, L8
	li		$t0, 6
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L8:
	lw		$t0, num1
	li		$t1, 81
	bne		$t0, $t1, L9
	j		L10
L9:
	li		$t0, 7
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L10:
	lw		$t0, num1
	li		$t1, 81
	bne		$t0, $t1, L11
	j		L13
L11:	j		L12
L13:
	li		$t0, 2
	li		$t1, 2
	bne		$t0, $t1, L12
	li		$t0, 8
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L12:
	lw		$t0, num1
	li		$t1, 80
	bne		$t0, $t1, L14
	j		L16
L14:
	lw		$t0, num1
	li		$t1, 81
	bne		$t0, $t1, L15
L16:
	li		$t0, 9
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
L15:
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
num1:	.word		0
num2:	.word		0
