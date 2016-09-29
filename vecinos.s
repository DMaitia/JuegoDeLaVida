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
	sw $zero,finH($sp)
	sw $zero,finV($sp)
	sw $zero,inicioV($sp)

	sw $a0,32($sp) //guardo a
	sw $a1,36($sp) //guardo i
	sw $a2,40($sp) //guardo j
	sw $a3,44($sp) //guardo M

	//inicioV = i - 1; no si se puede hacer directo con el -1
	add $t0,$a1,-1
	sw $t0,inicioV($sp)
	//if (inicioV < 0) inicioV = 0;
	blez $t0,inicioVCero
	//inicioH = j - 1;
	add $t0,$a2,-1
	sw $t0,inicioH($sp)
	blez $t0,inicioHCero
	//finV = i + 1;
	add $t0,$a1,1
	sw $t0,finV($sp)
	//if (finV > N - 1)
	//cargo N-1 en t1
	add $t1,$a4,-1
	
inicioVCero:	sw $zero,inicioV($sp)
inicioHCero:	sw $zero,inicioH($sp)
