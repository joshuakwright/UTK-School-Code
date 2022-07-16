#Joshua Wright CS130 Lab 6.2 4/23/20
		
#Function List:	MAIN - Calls FREAD, SORT, FWRITE, PRSTXT and exits program
#		FREAD(C11-C15) - Reads in file and populates BUF with its contents
#		SORT(C31-C35) - Sorts integers in BUF in ascending order
#		FWRITE(C16-C110) - Opens output file and writes BUF contents to it, then closes the files
#		PRSTXT(C21-C27) - Prints the number of Processed Ints to the Console
#		PRMSG - Prints input string to console

	.data
BUF:	.word   0 : 200	#allocate array BUF[200] initialized to 0

IFILE:	.asciiz "ints.dat"
OFILE:	.asciiz "ints_62.dat"
PInts:	.asciiz "Processed  ints"

	.text
	
############################################################ Main ###################################################################
# s0 : Integers Read In
# s1 : Integers Written Out

#calls all the function to read, sort, output ints, and write to a file
MAIN:	la	$a0, IFILE	#loads a0 with input file address
	la	$a1, BUF	#loads a1 with BUF address
	li	$a2, 200	#loads a2 with max num of ints
	jal	FREAD		#calls FREAD
	move	$s0, $v0	#puts ints read into s0
	ble	$s0, $zero, EXIT#exits the program if error in reading
	
	la	$a0, BUF	#loads a0 with BUF address
	move	$a1, $s0	#loads a1 with ints read in
	jal	SORT		#calls SORT
	
	la	$a0, OFILE	#loads a0 with output file address
	la	$a1, BUF	#loads a1 with BUF address
	move	$a2, $s0	#loads a2 with ints read in
	jal	FWRITE		#calls FWRITE
	move	$s1, $v0	#puts ints written in s1
	ble	$s1, $zero, EXIT#exits the program if error in writing
	
	la	$a0, PInts	#loads a0 with processed ints string address
	move	$a1, $s0	#loads a1 with num of ints read in
	jal	PRSTXT		#calls PRSTXT function
	
	#exits the program
	EXIT:	li 	$v0, 10		#exits the program
	syscall

############################################################ Read File Function #####################################################
# s0 : FD of IFILE 
# s1 : *BUF
# s2 : 4 * NMAX = Max Bytes to Read In	
# t0 : *IFILE 
	
	.data
InOEr:	.asciiz "Error: input file open error" #opening input file error
InREr:	.asciiz "Error: input file read error" #reading input file error

	.text
	
#starts FREAD function by saving registers to stack and brings in arguments
FREAD:	addi	$sp, $sp, -4	#stack: grows size
	sw	$ra, 0($sp)	#stack: stores $ra
	move	$t0, $a0	#brings in a0
	move	$s1, $a1	#brings in a1
	move	$s2, $a2	#brings in a2
	mul	$s2, $s2, 4	#changes max ints to max bytes

#opens read file		
C11: 	li 	$v0, 13		#open file
	la 	$a0, ($t0)	#put file address in $a0
	li	$a1, 0		#declares file for reading
	li	$a2, 0	
	syscall			#open IFILE, puts fd in $v0
	move 	$s0, $v0	#puts fd in $s0
	beq	$s0, -1, C12	#branches to input file error statement
	j C13			#jumps to next instruction if no error

#outputs read file open error
C12:	la	$a0, InOEr	#loads a0 with text address
	jal	PRMSG		#calls print function
	j 	C15		#jump to program exit

#reads in file	
C13:	li 	$v0, 14		#read file syscall
	move 	$a0, $s0	#puts address of file in $a0
	la 	$a1, ($s1)	#loads the buffer address
	move	$a2, $s2	#tells syscall how many bytes to read
	syscall
	div	$v0, $v0, 4	#changes output from bytes to ints read
	beq	$v0, 0, C14	#branches to input file read error statement
	j	C15		#branches to exit function

#file read error output	
C14:	la	$a0, InREr	#loads error message into a0
	jal	PRMSG		#calls print function
	j 	C15		#jump to fucntion exit
	
C15: 	lw	$ra, 0($sp)	#stack: push $ra
	addi	$sp, $sp, 4	#shrinks stack
	jr	$ra		#Exits READ function
	
######################################################### Set Precision Function ######################################
# s0 : Ints Read In
# t0 : Hundres Place Int Num
# t1 : Tens and Ones Place Int Nums
# t2 : Tens Place Int Num
# t3 : Ones Place Int Num
# t4 : Has ' ' as value in C22 and is reset and used as integer place count
# t5 : Addres of place in string where integers should be inserted
# t6 : Hold byte at $st5 address in C22 and then from C23 on it hold the address right after the last character in the string
# t7 : *PInts
# t8 : used to hold byte values for comparison and movement
# t9 : used to hold byte values for comparison and movement

#starts PRTXT function by saving registers to stack and brings in arguments
PRSTXT: addi	$sp, $sp, -8	#stack: grow size
	sw	$ra, 4($sp)	#stack: saves $ra
	sw	$s0, 0($sp)	#stack: saves $s0
	move	$t7, $a0	#brings in a0
	move	$s0, $a1	#brings in a1
	
#sets initial values
C21:	li	$t4, ' '	# loads space character into t4 for comparison
	move	$t5, $t7	# loads Processed ints string into t5

#finds the address where the integers should go in the string	
C22:	lb 	$t6, 0($t5)	#loads first byte into t6
	addi	$t5, $t5, 1	#increments the address to second byte
	beq	$t6, $t4, C23	#moves to next function if character at the address t5 is a space
	j	C22		#loops

#resets t6 so it can be reused and then finds the last value in the string
C23:	move 	$t6, $t5	#loads string address into t6
C24: 	li	$t9, 0		#initializes t9 to 0
	lb 	$t8, 0($t6)	#loads byte 
	beq	$t8, $t9, C25	#when null charachter is reached at end of string, branch to next function
	addi	$t6, $t6, 1	#increment t6 to get next character for comparison
	j	C24		#loop

#branches to appropriate function to add 1, 2, or 3 integers to the string
C25:	move 	$t4, $zero	#initializes t4 to 0 so it can be reused
	addi	$t4, $t4, 3	#sets t4 to 3 for 3 integers
	div 	$t0, $s0, 100	#puts hundres place number in t0
	mfhi 	$t1		#gets remainder from division
	div 	$t2, $t1, 10	#puts tens place number in t2
	mfhi	$t3		#puts ones place in t3
	bgtz	$t0, C26.3	#branches to 3 integer functions
	subi	$t4, $t4, 1	#decrements integer count
	bgtz	$t2, C26.2	#branches to 2 integer count
	subi	$t4, $t4, 1	#decrements integer count
	j	C26.1		#branches to 1 integer count

#moves the address of "ints" in the processed ints string 3 to the right to make room for 3 integers
C26.3:	lb	$t9, ($t6)	#loads final character in string
	add	$t8, $t6, $t4	#increments address by 3 (in st4) 
	sb	$t9, ($t8)	#puts the final character three addresses to the right
	beq	$t6, $t5, C26.3P#breaks loop once "ints" has been moved three to the right
	subi	$t6, $t6, 1	#decrement the address
	j	C26.3		#loops

#puts 3 integers in processed int string and outputs the string to the console	
C26.3P:	addi	$t0, $t0, '0'	#converts integer to ascii character
	addi 	$t3, $t3, '0'	#converts integer to ascii character
	addi	$t2, $t2, '0'	#converts integer to ascii character
	sb	$t0, ($t5)	#places most significant integer in processed ints string
	addi	$t5, $t5, 1	#increments address to place next integer one address to the right
	sb	$t2, ($t5)	#places next integer in string
	addi	$t5, $t5, 1	#increments address to place next integer one address to the right
	sb	$t3, ($t5)	#places final integer in string
	move	$a0, $t7	#loads processed ints string into a0
	jal	PRMSG		#calls print function	
	j	C27		#branches to function exit

#moves the address of "ints" in the processed ints string 2 to the right to make room for 2 integers
C26.2:	lb	$t9, ($t6)	#loads final character in string
	add	$t8, $t6, $t4	#increments address by 2 (in st4)
	sb	$t9, ($t8)	#puts the final character two addresses to the right
	beq	$t6, $t5, C26.2P#breaks loop once ints has been moved two to the right
	subi	$t6, $t6, 1	#decrement the address
	j	C26.2		#loops

#puts 3 integers in processed int string and outputs the string to the console
C26.2P:	add 	$t3, $t3, '0'	#converts integer to ascii character
	add	$t2, $t2, '0'	#converts integer to ascii character
	sb	$t2, ($t5)	#places most significant integer in processed ints string
	addi	$t5, $t5, 1	#increments address to place next integer one address to the right
	sb	$t3, ($t5)	#places final integer in string
	move	$a0, $t7	#loads processed ints string into a0
	jal	PRMSG		#calls print function	
	j	C27		#branches to function exit

#moves the address of "ints" in the processed ints string 1 to the right to make room for 1 integers	
C26.1:	lb	$t9, ($t6)	#loads final character in string
	add	$t8, $t6, $t4	#increments address by 1 (in st4)
	sb	$t9, ($t8)	#puts the final character one addresse to the right
	beq	$t6, $t5, C26.1P#breaks loop once ints has been moved one to the right
	subi	$t6, $t6, 1	#decrement the address
	j	C26.1		#loops

#puts 3 integers in processed int string and outputs the string to the console	
C26.1P:	add	$t3, $t3, '0'	#converts integer to ascii character
	sb	$t3, ($t5)	#places integer in processed ints string
	move	$a0, $t7	#loads processed ints string into a0
	jal	PRMSG		#calls print function	
	j	C27		#branches to function exit

#restores stack and exits PRSTXT function
C27:	lw   	$s0, 0($sp)    	# stack: push $ra
        lw   	$ra, 4($sp)    	# stack: push $ra
        addi 	$sp, $sp, 8   	# stack: shrink size
	jr	$ra		#exits set PRSTXT function

################################################################## Sort Function ######################################################
# s0 : Number of Ints to Sort
# t0 : *BUF
# t1 : Inner Loop Variable
# t2 : BUF($t5)
# t3 : BUF($t1)
# t4 : Bytes Read In - 1
# t5 : Outer Loop Variable 

#starts SORT function by saving registers to stack and brings in arguments 
SORT:	addi	$sp, $sp, -8	#stack: grow size
	sw	$ra, 4($sp)	#stack: saves $ra
	sw	$s0, 0($sp)	#stack: saves $s0
	move	$t0, $a0	#brings in a0
	move	$s0, $a1	#brings in a1

#initialies values needed for sort loops
C31:	mul	$s0, $s0, 4	#reverts s0 to bytes read
	subi	$t4, $s0, 4	#finds Bytes Read In - 1 : Outer Loop Variable
	move	$t5, $t0	#loads BUF address
	add	$t5, $t5, $t4	#loads address of last integer
	
#selection sort while outer loop and function exit
C32:	subi	$t1, $t5, 4 	#assigns inner loop variable value
	bgt	$t5, $t0, C33	#branches to inner while loop
	lw   	$s0, 0($sp)    	#stack: push $ra
        lw   	$ra, 4($sp)    	#stack: push $ra
        addi 	$sp, $sp, 8   	#stack: shrink size
	jr	$ra		#exits SORT function

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
# s0 : FD of OFILE 
# s1 : *BUF 
# s2 : 4 * NMAX = Max Bytes to Write Out
# t0 : *OFILE
# t1 : Ints Written
	
	.data
OutOEr:	.asciiz "Error: output file open error" #opening output file error
OutREr:	.asciiz "Error: output file write error" #writitng output file error

	.text
	
#starts FWRITE function by saving registers to stack and brings in arguments
FWRITE:	addi	$sp, $sp, -8	#stack: grows size
	sw	$ra, 4($sp)	#stack: stores $ra
	sw	$s0, 0($sp)	#stack: stores $s0
	move	$t0, $a0	#brings in a0
	move	$s1, $a1	#brings in a1
	move	$s2, $a2	#brings in a2
	mul	$s2, $s2, 4	#changes max ints to max bytes

#opens file for writing			
C16:	li 	$v0, 13		#opens file for writing
	move 	$a0, $t0	#puts file address in $a0
	li 	$a1, 1		#declears file for writing
	li	$a2, 0		
	syscall
	move 	$s0, $v0	#puts FD of ouput file in s0
	beq	$s0, -1, C17	#branches to error statement if error
	j	C18		#jumps to next instruction if no error

#outputs write file open error
C17:	la	$a0, OutOEr	#loads error message into a0
	jal	PRMSG		#calls print function
	j 	C110		#jumps to exit function

#writes to file	
C18:	li	$v0, 15		#write to file syscall
	move	$a0, $s0	#puts address of file in $a0
	move	$a1, $s1	#puts BUF address in $a1
	move	$a2, $s2	#tells syscall how many bytes to write to the file
	syscall
	move 	$t1, $v0	#puts v0 in t1 for preserve return value until after files are closed
	div 	$t1, $t1, 4	#turn bytes written into ints written
	beq	$t1, 0, C19	#branches to error message if error
	j 	C110		#jump to next intruction if ok

#outputs write error
C19:	la	$a0, OutREr	#loads error message into a0
	jal	PRMSG		#calls print function
	j 	C110		#jumps to exit function

#closes all the files and exits function
C110:	li 	$v0, 16		#close file syscall
	move 	$a0, $s0	#puts address of input file in $a0
	syscall
	
	li 	$v0, 16		#close file syscall
	move 	$a0, $s0	#puts address of output file in $a0
	syscall
	
	move 	$v0, $t1	#puts return value back in $v0
	lw   	$s0, 0($sp)    	# stack: push $s0
        lw   	$ra, 4($sp)    	# stack: push $ra
        addi 	$sp, $sp, 8   	# stack: shrink size
	jr	$ra		#exits swap function

############################################################## PRINT FUNCTION ############################################
# t0 : *text

#prints input text
PRMSG:	addi	$sp, $sp, -8	#grows stack
	sw	$ra, 4($sp)	#saves $ra to stack
	sw	$v0, 0($sp)	#preserves v0 so as to not disrupt the return value of functions that call PRINT
	move 	$t0, $a0	#loads in text address
	li	$v0, 4		#print string syscall
	la	$a0, ($t0)	#loads text address into syscall
	syscall
	
	addi $a0, $0, '\n'  	# loads a0 with newline
        li   $v0, 11        	# print
        syscall             	
        
        lw	$v0, 0($sp)	#pushes v0 to stack
	lw	$ra, 4($sp)	#pushes ra to stack
	addi	$sp, $sp, 8	#shrinks stack
	jr	$ra		#exits function
