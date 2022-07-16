Mod:	addi 	$t1, $zero, 153 
	div 	$t2, $t1, 100
	mfhi 	$t3
	li 	$v0, 1
	la 	$a0, ($t3)
	syscall

EXIT:	li 	$v0, 10		
	syscall

