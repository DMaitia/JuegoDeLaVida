#include <mips/regdef.h>
	#finH
	#finV
	#inicioH
	#inicioV
	#vecinos;
	.text
	.ent factorial
	.globl factorial
main:
	//hay que ver el stack para saber cuanto se corre
	subu $sp,$sp,16
	sw	$fp,12($sp)
	sw	$gp,8($sp)
	move	$fp,$sp
	//inicializo las variables
	sw $0,finH($sp)
	sw $0,finV($sp)
	sw $0,inicioH($sp)
	sw $0,inicioV($sp)

	//inicioV = i - 1;
	sw $a0,32($sp) //guardo a
	sw $a1,36($sp) //guardo i
	sw $a2,40($sp) //guardo j
	sw $a3,44($sp) //guardo M
	lw $t0,$a1,1
