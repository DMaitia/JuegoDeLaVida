#include <mips/regdef.h>

	.align 2 		#alinear por tandas de 2^2=4 bytes
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
	#$fp -->	+-----------------------+
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
	#int columnaFinal ---  s1
	#int filaFinal    ---  s2 
	#columnaInicial   ---  s3 
	#filaInicial      ---  s4
	#vecinos 	  ---  s5
	####################################################
	
	################=PARAMETROS=########################
	#unsigned char *a      ---  a0
	#unsigned int columna  ---  a1
	#unsigned int fila     ---  a2
	#unsigned int cantFilas---  a3
	#unsigned int cantCols --- 0( $fp) ->  t7 
	####################################################
	
main:
	
	subu  sp, sp,56 #La cantidad de espacios que se debe mover depende de
			#el numero de registros que se deben guardar. Hay que hacer
			#espacio para ellos en el stack. Ver pag 99 del Patterson
			#Como hay 10 variables (5 pasados por parametro y 7 variables internas
			#Desplazo el stack en 48 bytes mas el stack pointer y
			#el frame pointer que creo que tambien tienen que tener 
			#suficiente espacio como para ser guardados 56. El minimo tamanio del stack frame
			#es de 24 bytes (A-27 Patterson) 
				
	sw	 $fp,12( sp); #Se tiene que hacer?
	sw	 gp,8( sp);  #
	move	 $fp, sp;     #
	
	######INICIALIZAR VARIABLES######
	subi  s4, a2,1		#filaInicial = fila - 1;
	subi  s3, a1,1		#columnaInicial = columna - 1;
	addi  s2, a2,1		#filaFinal = fila + 1;
	addi  s1, a1,1		#columnaFinal = columna + 1;
	add  s5, zero, zero	#vecinos = 0;
	lw  t7,0( $fp)		#cantCols ->  t7
	
	######ACOTAR POSICIONES ######### 
# Referencias : https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Mips/cond.html
#		Patterson: apendice A: A-64 "Branch Instructions"; 
#		Patterson: capitulo 2.7 "Instructions for making decisions"
	
 IF1:	bgez  s4, IF2			#if (filaInicial < 0) { 
    	add  s4, zero, zero		#  filaInicial = 0;
  					#} 
  					# Resumen:
  					# "Si filaInicial es mayor o igual a cero
  					# saltar a IF2 y sino ejecutar cuerpo"
	
 IF2:	bgez  s3, IF3			#if (columnaInicial < 0) {
	add  s3, zero, zero		#  columnaInicial = 0;
					#}

 IF3:	subi  t0, a3,1			#t0 = cantFilas - 1
	ble  s2, t0,IF4			#if (filaFinal > cantFilas - 1) {
	add  s2, zero, t0 		# filaFinal = cantFilas - 1;
					#}
					# Resumen: 
					# "Si filaFinal es menor o igual a cantFilas - 1
					# saltar a IF4 y sino ejecutar cuerpo"
		
 IF4:	subi  t0, t7,1			#t0 = cantCols - 1
	ble  s1, t0,LOOP		#if (columnaFinal > cantCols - 1){
	add  s1, zero, t0		#  columnaFinal = cantCols - 1;
					#}

	########CICLOS ANIDADOS############# 
#y =  s6
#x =  s7
	
LOOP:   addi  s6, s4,-1			# s6 = filaInicial - 1 (porque luego incremento cuando comienza el ciclo y arranca en filaInicial)

FOR1:	bge  s6, s2,FIN			#si y > filaFinal ir a FIN
	addi  s6, s6,1			#y++
	addi  s7, s3,-1			# s7 = columnaInicial - 1 (mismo motivo que antes)
	j FOR2			#No se si es necesario

IF5:	multu  s6, t7			#y*cantCols
	mflo   t1			#del producto almacenar los 32 bits menos significativos en  t1
	add  t1, t1, s7			#y*cantCols + x
	sll  t1, t1,2			#t1*4 (las posiciones se dan de a 4 bytes)
	ld   t2, t1(a0)		#a[y * cantCols + x] ->  t2
	add  s5, s5, t2			#vecinos +=  t2
	j FOR2
		
FOR2:	bge  s7, s1,FOR1		#si x > columnaFinal ir a FOR1
	addi  s7, s7,1			#x++
	
					#en esta parte cabe aclarar que por DeMorgan 
					#!(x == columna && y == fila) equivale a (x!=columna || y!=fila)
	bne  s7, a1,IF5
	beq  s6, a2,FOR2		#No se cumple ninguna de las 2 condiciones
	
	.end
