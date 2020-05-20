.text
.globl		main
main:
li		$t0, 1
sw		$t0, num1
li		$t0, 2
sw		$t0, num2
li		$t0, 3
sw		$t0, num3
lw		$t0, num1
li		$v0, 1
move		$a0, $t0
syscall
lw		$t0, num2
li		$v0, 11
li		$a0, 44
syscall
li		$v0, 1
move		$a0, $t0
syscall
lw		$t0, num3
li		$v0, 11
li		$a0, 44
syscall
li		$v0, 1
move		$a0, $t0
syscall
li		$v0, 4
la		$a0, _nl
syscall
lw		$t0, num1
li		$v0, 1
move		$a0, $t0
syscall
lw		$t0, num1
lw		$t1, num2
mul		$t2, $t0, $t1
lw		$t0, num3
div		$t1, $t2, $t0
li		$v0, 11
li		$a0, 44
syscall
li		$v0, 1
move		$a0, $t1
syscall
lw		$t0, num3
lw		$t1, num2
addi		$t3, $zero, 1
addi		$t2, $zero, 0
L1:
beq		$t2, $t1, L2
mul		$t3, $t0, $t3
addi		$t2, $t2, 1
j		L1
L2:
li		$v0, 11
li		$a0, 44
syscall
li		$v0, 1
move		$a0, $t3
syscall
li		$v0, 4
la		$a0, _nl
syscall
li		$v0, 4
la		$a0, _nl
syscall
li		$t0, 5
li		$v0, 1
move		$a0, $t0
syscall
li		$v0, 4
la		$a0, _nl
syscall
lw		$t0, num3
li		$t1, 5
mul		$t2, $t0, $t1
addi		$t0, $zero, 0
L3:
beq		$t0, $t2, L4
li		$v0, 11
li		$a0, 32
syscall
addi		$t0, $t0, 1
j		L3
L4:
li		$t0, 5
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
num1:	.word		0
num2:	.word		0
num3:	.word		0
_nl:	.asciiz		"\n"
