.text
.globl		main
main:
li		$t0, 10
sw		$t0, num1
li		$t0, 4
sw		$t0, num2
lw		$t0, num1
lw		$t1, num2
bne		$t0, $t1, L1
li		$t0, 1
li		$v0, 1
move		$a0, $t0
syscall
li		$v0, 4
la		$a0, _nl
syscall
j		L2
L1:
li		$t0, 5
li		$v0, 1
move		$a0, $t0
syscall
li		$v0, 4
la		$a0, _nl
syscall
L2:
li		$t0, 0
sw		$t0, counter
L4:
lw		$t0, counter
li		$t1, 5
slt		$t3, $t0, $t1
li		$t2, 1
bne		$t3, $t2, L3
lw		$t0, counter
li		$v0, 1
move		$a0, $t0
syscall
li		$v0, 4
la		$a0, _nl
syscall
lw		$t0, counter
li		$t1, 1
add		$t2, $t0, $t1
sw		$t2, counter
j		L4
L3:
li		$t0, 0
sw		$t0, counter
L6:
lw		$t1, counter
li		$t2, 5
slt		$t4, $t1, $t2
li		$t3, 1
bne		$t4, $t3, L5
lw		$t1, counter
li		$v0, 1
move		$a0, $t1
syscall
li		$v0, 4
la		$a0, _nl
syscall
lw		$t1, counter
li		$t2, 1
add		$t3, $t1, $t2
sw		$t3, counter
j		L6
L5:
li		$v0, 10
syscall
.data
.align		4
counter:	.word		0
num1:	.word		0
num2:	.word		0
_nl:	.asciiz		"\n"
