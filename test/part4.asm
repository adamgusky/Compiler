	.text	
	.globl		main
main:
	li		$t0, 0
	sw		$t0, i
L2:
	lw		$t1, i
	li		$t2, 9
	slt		$t4, $t1, $t2
	li		$t3, 1
	bne		$t4, $t3, L1
	lw		$t1, i
	lw		$t2, i
	la		$t4, array
	add		$t1, $t1, $t1
	add		$t1, $t1, $t1
	add		$t5, $t1, $t4
	sw		$t2, 0($t5)
	lw		$t1, i
	li		$t2, 1
	add		$t3, $t1, $t2
	sw		$t3, i
	j		L2
L1:
	li		$t0, 2
	sw		$t0, i
	lw		$t0, i
	li		$t2, 2
	mul		$t3, $t0, $t2
	la		$t0, array
	sll		$t3, $t3, 2
	add		$t0, $t0, $t3
	lw		$t2, 0($t0)
	sw		$t2, i
	lw		$t0, i
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$v0, 10
	syscall	
	.data	
	.align		4
i:	.word		0
array:	.space		40
_nl:	.asciiz		"\n"
