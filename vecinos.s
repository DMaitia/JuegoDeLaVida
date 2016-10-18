#include <mips/regdef.h>

	.align 2
	.text
	.ent vecinos
	.globl vecinos


	#Stack Frame (http://www.cygwin.com/ml/binutils/2003-06/msg00436.html)
	#-----------
	#
	#	+-----------------------+
	#	|    Argument 6    	|
	#	+-----------------------+
	#	|    Argument 5		|   Se empieza por el argumento 5 puesto que los 4 primeros estan en los registros a0-a3
	#FP -->	+-----------------------+
	#	|    Saved Registers	|
	#	+-----------------------+
	#	|        ...		|
	#	+-----------------------+
	#	|    Local Variables	|
	#	+-----------------------+
	#	|        ...		|
	#	+-----------------------+
	#	|   Parameter Word 2	|
	#	+-----------------------+
	#SP -->	|   Parameter Word 1	|
	#	+-----------------------+
	#
	# LEER KAUFMANN Página 620 "Procedure Calls" Ahi esta explicado
	
	###############=VARIABLES INTERNAS=#################
	#int columnaFinal --- $s1
	#int filaFinal    --- $s2 
	#columnaInicial   --- $s3 
	#filaInicial      --- $s4
	#vecinos 	  --- $s5
	####################################################
	
	################=PARAMETROS=########################
	#unsigned char *a      --- $a0
	#unsigned int columna  --- $a1
	#unsigned int fila     --- $a2
	#unsigned int cantFilas--- $a3
	#unsigned int cantCols --- 0($fp)
	####################################################
	
main:
	
	subu $sp,$sp,40 #La cantidad de espacios que se debe mover depende de
			#el numero de registros que se deben guardar. Hay que hacer
			#espacio para ellos en el stack. Ver pag 99 del Patterson
			#Como hay 10 variables (5 pasados por parametro y 5 variables internas
			#Desplazo el stack en 40 bytes. El minimo tamanio del stack frame
			#es de 24 bytes (A-27 Patterson) 
				
	sw	$fp,12($sp);
	sw	$gp,8($sp);
	move	$fp,$sp;
	
	######INICIALIZAR VARIABLES######
	sub $s4,$a2,1		#filaInicial = fila - 1;
	sub $s3,$a1,1		#columnaInicial = columna - 1;
	add $s2,$a2,1		#filaFinal = fila + 1;
	add $s1,$a1,1		#columnaFinal = columna + 1;
	add $s5,$zero,$zero	#vecinos = 0;
	
	
	######ACOTAR POSICIONES ######### 
# Referencias : https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Mips/cond.html
#		Patterson: apendice A: A-64 "Branch Instructions"; 
#		Patterson: capitulo 2.7 "Instructions for making decisions"
	
$IF1:	bgez $s4,$IF2			#if (filaInicial < 0) { 
    	add $s4,$zero,$zero		#  filaInicial = 0;
  					#} 
  					# Resumen:
  					# "Si filaInicial es mayor o igual a cero
  					# saltar a IF2 y sino ejecutar cuerpo"
	
$IF2:	bgez $s3,$IF3			#if (columnaInicial < 0) {
	add $s3,$zero,$zero		#  columnaInicial = 0;
					#}

$IF3:	sub $t0,$a3,1			#t0 = cantFilas - 1
	ble $s2,$t0,IF4			#if (filaFinal > cantFilas - 1) {
	add $s2,$zero,$t0 		# filaFinal = cantFilas - 1;
					#}
					# Resumen: 
					# "Si filaFinal es menor o igual a cantFilas - 1
					# saltar a IF4 y sino ejecutar cuerpo"
					
$IF4:	lw $t1,0($fp)			#cantCols -> $t1
	sub $t0,$t1,1			#t0 = cantCols - 1
	ble $s1,$t0,LOOP		#if (columnaFinal > cantCols - 1){
	add $s1,$zero,$t0		#  columnaFinal = cantCols - 1;
					#}
					
LOOP:   

#########################
  for (int y = filaInicial; y <= filaFinal; y++) {
    for (int x = columnaInicial; x <= columnaFinal; x++) {
      if (!(x == columna && y == fila)) {
        vecinos += a[y * cantCols + x];
      }
    }
  }


