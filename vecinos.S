#include <mips/regdef.h>

	.align 2 		#alinear por tandas de 2^2=4 bytes
	.text
	.ent vecinos
	.globl vecinos

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
	#unsigned int cantCols --- 48( $fp) ->  t7
	####################################################

vecinos:subu  sp, sp,32

	sw	 $fp,28(sp); #Guardo la direccion de memoria de fp en memoria
	sw	 gp,24(sp);  #
	move	 $fp,sp;    #uso fp,

	######INICIALIZAR VARIABLES######
	subu  s4, a2,1		#filaInicial = fila - 1;
	subu  s3, a1,1		#columnaInicial = columna - 1;
	addu  s2, a2,1		#filaFinal = fila + 1;
	addu  s1, a1,1		#columnaFinal = columna + 1;
	add  s5, zero, zero	#vecinos = 0;
	lw  t7,48($fp)		#cantCols ->  t7

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

 IF3:	subu  t0, a3,1			#t0 = cantFilas - 1
	ble  s2, t0,IF4			#if (filaFinal > cantFilas - 1) {
	add  s2, zero, t0 		# filaFinal = cantFilas - 1;
					#}
					# Resumen:
					# "Si filaFinal es menor o igual a cantFilas - 1
					# saltar a IF4 y sino ejecutar cuerpo"

 IF4:	subu  t0, t7,1			#t0 = cantCols - 1
	ble  s1, t0,LOOP		#if (columnaFinal > cantCols - 1){
	add  s1, zero, t0		#  columnaFinal = cantCols - 1;
					#}

				########CICLOS ANIDADOS#############
				#y =  s6
				#x =  s7

LOOP:   addi  s6, s4,-1		# s6 = filaInicial - 1 (porque luego incremento cuando comienza el ciclo y arranca en filaInicial)
                            	#bge	$t0,$t1,target	#  branch to target if  $t0 >= $t1
                            	#bgt	$t0,$t1,target	#  branch to target if  $t0 > $t1
				#FOR1:	bge  s6, s2,FIN		#si y > filaFinal ir a FIN
FOR1:	addu s6, s6,1		#y++
	bgt  s6, s2,RETURN	#si y > filaFinal ir a FIN

	addu  s7, s3,-1		# s7 = columnaInicial - 1 (mismo motivo que antes)
	j FOR2			#No se si es necesario

IF5:	multu  s6, t7			#y*cantCols
	mflo   t1			#del producto almacenar los 32 bits menos significativos en  t1
	add  t1, t1, s7			#y*cantCols + x

	addu t3,t1,a0			## sumo el desplazamiento mas la direccion que hay en a0
	lbu   t2,(t3)			#a[y * cantCols + x] ->  t2 (t3) direccion de memoria que hay en t3
	add  s5, s5, t2			#vecinos +=  t2
	j FOR2

FOR2:	addi s7, s7,1			#x++
	bgt  s7, s1,FOR1		#si x > columnaFinal ir a FOR1


					#en esta parte cabe aclarar que por DeMorgan
					#!(x == columna && y == fila) equivale a (x!=columna || y!=fila)
	bne  s7, a1,IF5
	beq  s6, a2,FOR2		#No se cumple ninguna de las 2 condiciones
	j IF5

RETURN:	addu v0,s5,zero  #coloco en ra lo que hay en s5
	lw $fp,28(sp)
	addi sp,sp,32	 #restauro el valor del stack pointer
	j ra             #retorno el valor
	.end vecinos
