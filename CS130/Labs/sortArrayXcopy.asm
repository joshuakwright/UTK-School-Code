	
#Register List:	s0 - FD of Input File
#		s1 - Bytes read in Input File
#		s2 - FD of Output File
#		s3 - Bytes written to Output File

#Function List: C1X - Input / Ouput File Open / Read / Write
#		C2X, C26.XP - Prints Processed Ints
#		C3X - Sorts integers
#		EXIT - Exits Program		
	.data
BUF:	.word   0 : 200	#allocate array BUF[200] initialized to 0


OutOEr:	.asciiz "Error: output file open error" #opening output file error

OutREr:	.asciiz "Error: output file write error" #writitng output file error
IFILE:	.asciiz "C:/Users/mrjos/Desktop/Lab_6.2_Input_Files/ints_200.dat" 
OFILE:	.asciiz "C:/Users/mrjos/Desktop/Lab_6.2_Output_Files/ints.dat"
PInts:	.asciiz "Processed  ints"

	.text
	
############################################################ Main ###################################################################
# s0 : Integers Read In

MAIN:	la	$a0, IFILE
	la	$a1, BUF
	li	$a2, 200
	jal	FREAD
	move	$s0, $v0
	ble	$s0, $zero, EXIT
	
	la	$a0, BUF
	move	$a1, $s0
	jal	SORT

############################################################ Read File Function #####################################################
# t0 : *IFILE
# t1 : *BUF
# t2 : 4 * NMAX = Max Bytes to Read In	
# t3 : FD of IFILE
	
	.data
InOEr:	.asciiz "Error: input file open error" #opening input file error
InREr:	.asciiz "Error: input file read error" #reading input file error

	.text
	
FREAD:	addi	$sp, $sp, -4
	sw	$ra, 0($sp)
	move	$t0, $a0
	move	$t1, $a1
	move	$t2, $a2
	mul	$t2, $t2, 4

#opens read file		
C11: 	li 	$v0, 13		#open file
	la 	$a0, $t0	#put file address in $a0
	li	$a1, 0		#declares file for reading
	li	$a2, 0
	syscall			#open IFILE, puts fd in $v0
	move 	$t3, $v0	#puts fd in $s0
	beq	$t3, -1, C12	#branches to input file error statement
	j C13			#jumps to next instruction if no error

#outputs read file open error
C12:	la	$a0, InOEr
	jal	PRMSG
	j 	C15		#jump to program exit

#reads in file	
C13:	li 	$v0, 14		#read file syscall
	move 	$a0, $t3	#puts address of file in $a0
	la 	$a1, $t1	#loads the buffer address
	li	$a2, $t2	#tells syscall how many bytes to read
	syscall
	div	$v0, $v0, 4
	beq	$v0, 0, C14	#branches to input file read error statement
	j	C15

#file read error output	
C14:	la	$a0, InREr
	jal	PRMSG
	j 	C15		#jump to program exit
	
C15: 	lw	$ra, 0($sp)
	addi	$sp, $sp, 4
	jr	$ra		#jumps to next instruction if no error
	
######################################################### Set Precision Function ######################################
#sets initial values
C21:	li	$t7, '0'	# loads 0 into t7 for comparisons
	li	$t4, ' '	# loads space character into t4 for comparison
	la	$t5, PInts	# loads Processed ints string into t5

#finds the address where the integers should go in the string	
C22:	lb 	$t6, 0($t5)	#loads first byte into t6
	addi	$t5, $t5, 1	#increments the address to second byte
	beq	$t6, $t4, C23	#moves to next function if character at the address t5 is a space
	j	C22		#loops

#resets t6 so it can be reused and then finds the address of the last value in the string
C23:	move 	$t6, $t5	#loads string address into t6
C24: 	li	$t9, 0		#initializes t9 to 0
	lb 	$t8, 0($t6)	#loads byte 
	beq	$t8, $t9, C25	#when null charachter is reached at end of string, branch to next function
	addi	$t6, $t6, 1	#increment t6 to get next character for comparison
	j	C24		#loop

#branches to appropriate function to add 1, 2, or 3 integers to the string
C25:	move 	$t4, $zero	#initializes t4 to 0 so it can be reused
	addi	$t4, $t4, 3	#sets t4 to 3 for 3 integers
	div 	$t0, $s1, 100	#puts hundres place number in t0
	mfhi 	$t1		#gets remainder from division
	div 	$t2, $t1, 10	#puts tens place number in t2
	mfhi	$t3		#puts ones place in t3
	bgtz	$t0, C26.3	#branches to 3 integer functions
	subi	$t4, $t4, 1	#decrements integer count
	bgtz	$t2, C26.2	#branches to 2 integer count
	subi	$t4, $t4, 1	#decrements integer count
	j	C26.1		#branches to 1 integer count

#moves the address of ints in the processed ints string 3 to the right to make room for 3 integers
C26.3:	lb	$t9, ($t6)	#loads final character in string
	add	$t8, $t6, $t4	#increments address by 3 (in st4) 
	sb	$t9, ($t8)	#puts the final character three addresses to the right
	beq	$t6, $t5, C26.3P#breaks loop once ints has been moved three to the right
	subi	$t6, $t6, 1	#decrement the address
	j	C26.3		#loops

#puts 3 inteers in processed int string and outputs the string to the console	
C26.3P:	add	$t0, $t0, $t7	#converts integer to ascii character
	add 	$t3, $t3, $t7	#converts integer to ascii character
	add	$t2, $t2, $t7	#converts integer to ascii character
	sb	$t0, ($t5)	#places most significant integer in processed ints string
	addi	$t5, $t5, 1	#increments address to place next integer one address to the right
	sb	$t2, ($t5)	#places next integer in string
	addi	$t5, $t5, 1	#increments address to place next integer one address to the right
	sb	$t3, ($t5)	#places final integer in string
	li	$v0, 4		#prints string to the console syscall
	la	$a0, PInts	#loads address of string into a0
	syscall			
	j	C31		#branches to next function

#moves the address of ints in the processed ints string 2 to the right to make room for 2 integers
C26.2:	lb	$t9, ($t6)	#loads final character in string
	add	$t8, $t6, $t4	#increments address by 2 (in st4)
	sb	$t9, ($t8)	#puts the final character two addresses to the right
	beq	$t6, $t5, C26.2P#breaks loop once ints has been moved two to the right
	subi	$t6, $t6, 1	#decrement the address
	j	C26.2		#loops

#puts 3 inteers in processed int string and outputs the string to the console
C26.2P:	add 	$t3, $t3, $t7	#converts integer to ascii character
	add	$t2, $t2, $t7	#converts integer to ascii character
	sb	$t2, ($t5)	#places most significant integer in processed ints string
	addi	$t5, $t5, 1	#increments address to place next integer one address to the right
	sb	$t3, ($t5)	#places final integer in string
	li	$v0, 4		#prints string to the console syscall
	la	$a0, PInts	#loads address of string into a0
	syscall			
	j	C31		#branches to next function

#moves the address of ints in the processed ints string 1 to the right to make room for 1 integers	
C26.1:	lb	$t9, ($t6)	#loads final character in string
	add	$t8, $t6, $t4	#increments address by 1 (in st4)
	sb	$t9, ($t8)	#puts the final character one addresse to the right
	beq	$t6, $t5, C26.1P#breaks loop once ints has been moved one to the right
	subi	$t6, $t6, 1	#decrement the address
	j	C26.1		#loops

#puts 3 inteers in processed int string and outputs the string to the console	
C26.1P:	add	$t3, $t3, $t7	#converts integer to ascii character
	sb	$t3, ($t5)	#places integer in processed ints string
	li	$v0, 4		#prints string to the console syscall
	la	$a0, PInts	#loads address of string into a0
	syscall			
	j	C31		

################################################################## Sort Function ######################################################
# s0 : Number of Ints to Sort
# t0 : *BUF
# t1 : 
# t2 : 
# t3 :
# t4 : 
# t5 : 

SORT:	addi	$sp, $sp, 8
	sw	$ra, 4($sp)
	sw	$s0, 0($sp)
	move	$t0, $a0
	move	$s0, $a1

#resets temp variable values and initializes values needed for sort loops
C31:	mul	$s0, $s0, 4	#reverts s1 to bytes read
	subi	$t4, $s0, 4	#finds N - 1
	move	$t5, $t0	#loads BUF address
	add	$t5, $t5, $t4	#loads address of last integer
	
#selection sort while outer loop
C32:	subi	$t1, $t5, 4 	#assigns inner loop variable value
	bgt	$t5, $t0, C33	#branches to inner while loop
	lw   	$s0, 0($sp)    	# stack: push $ra
        lw   	$ra, 4($sp)    	# stack: push $ra
        addi 	$sp, $sp, 8   	# stack: shrink size
	jr	$ra		#exits swap function

#selection sort while inner loop
C33:	bge	$t1, $t0, C34 	#branches to if statement
	subi	$t5, $t5, 4	#decrements outer loop variable
	j	C32		#outer loop branch

#if statement to determine if swap is needed
C34:	lw	$t2, ($t5)	#loads first integer for comparison
	lw	$t3, ($t1)	#loads second integer for comparison
	blt	$t2, $t3, C35	#if statements that branches to integer swap
	subi	$t1, $t1, 4	#decrements inner loop variable
	j	C33		#inner loop branch

#swaps values
C35:	sw	$t2, ($t1)	#swaps address of first integer
	sw	$t3, ($t5)	#swaps address of second integer
	subi	$t1, $t1, 4	#decrements inner loop variable
	j	C33		#inner loop branch

################################################################## Write File Function ################################################

#opens file for writing			
C13:	li 	$v0, 13		#opens file for writing
	la 	$a0, OFILE	#puts file address in $a0
	li 	$a1, 1		#declears file for writing
	li	$a2, 0		
	syscall
	move 	$s2, $v0	#puts FD of ouput file in s2
	beq	$s2, -1, C18	#branches to error statement if error
	j	C14		#jumps to next instruction if no error

#outputs write file open error
C18:	li 	$v0, 4		#print string syscall
	la 	$a0, OutOEr	#loads output error message
	syscall
	j 	EXIT		#jumps to exit

#writes to file	
C14:	li	$v0, 15		#write to file syscall
	move	$a0, $s2	#puts address of file in $a0
	la	$a1, BUF	#puts BUF address in $a1
	#mul	$s1, $s1, 4	#changes s1 back to bytes from integers
	move	$a2, $s1	#tells syscall how many bytes to write to the file
	syscall
	move 	$s3, $v0	#puts bytes read in $s3
	beq	$s3, 0, C19	#branches to error message if error
	j 	C15		#jump to next intruction if ok

#outputs write error
C19:	li 	$v0, 4		#output string syscall
	la 	$a0, OutREr	#loads error message
	syscall
	j 	EXIT		#jumps to exit

#closes all the files
C15:	li 	$v0, 16		#close file syscall
	move 	$a0, $s0	#puts address of input file in $a0
	syscall
	
	li 	$v0, 16		#close file syscall
	move 	$a0, $s2	#puts address of output file in $a0
	syscall
	
#exits the program
EXIT:	li 	$v0, 10		#exits the program
	syscall

############################################################## PRINT FUNCTION ############################################
# t0 : *text

PRMSG:	addi	$sp, $sp, -4
	sw	$ra, 0($sp)
	move 	$t0, $a0
	li	$v0, 4
	la	$a0, $t0
	syscall
	
	addi $a0, $0, '\n'  # a0: newline
        li   $v0, 11        # print
        syscall             #
        
	lw	$ra, 0($sp)
	addi	$sp, $sp, 4
	jr	$ra