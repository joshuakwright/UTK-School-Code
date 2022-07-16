#Register List:	s0 - FD of Input File
#		s1 - Bytes read in Input File
#		s2 - FD of Output File
#		s3 - Bytes written to Output File

#Function List: C1X - Input / Ouput File Open / Read / Write
#		C2X, PXXX, SET, FIND, RESET, FINDE - Prints Processed Ints
	
	.data
BUF:	.word   0 : 200	#allocate array BUF[200] initialized to 0

InOEr:	.asciiz "Error: input file open error" #opening input file erro
OutOEr:	.asciiz "Error: output file open error" #opening output file error
InREr:	.asciiz "Error: input file read error" #reading input file error
OutREr:	.asciiz "Error: output file write error" #writitng output file error
IFILE:	.asciiz "C:/Users/mrjos/Desktop/Lab_6.2_Input_Files/ints_200.dat" 
OFILE:	.asciiz "C:/Users/mrjos/Desktop/Lab_6.2_Output_Files/ints.dat"
PInts:	.asciiz "Processed  ints"

	.text

############################################################ Read File Function #####################################################

#opens read file		
C11: 	addi	$t7, $t7, '0'
	li 	$v0, 13		#open file
	la 	$a0, IFILE	#put file address in $a0
	li	$a1, 0		#declares file for reading
	li	$a2, 0
	syscall			#open INFILE, puts fd in $v0
	move 	$s0, $v0	#puts fd in $s0
	beq	$s0, -1, B1	#branches to input file error statement
	j C12			#jumps to next instruction if no error

#outputs read file open error
B1:	li 	$v0, 4		#syscall string ouput 
	la 	$a0, InOEr	#loads error message into a0
	syscall
	j 	EXIT		#jump to program exit

#reads in file	
C12:	li 	$v0, 14		#read file syscall
	move 	$a0, $s0	#puts address of file in $a0
	la 	$a1, BUF	#loads the buffer address
	li	$a2, 800	#tells syscall how many bytes to read
	syscall
	move 	$s1, $v0	#puts bytes read in $s1
	div	$s1, $s1, 4
	beq	$s1, 0, B2	#branches to input file read error statement
	j 	C13		#jumps to next instruction if no error

#file read error output	
B2:	li 	$v0, 4		#syscall string ouput 
	la 	$a0, InREr	#loads error message into a0
	syscall
	j 	EXIT		#jump to program exit

################################################################## Write File Function ################################################

#opens file for writing			
C13:	li 	$v0, 13		#opens file for writing
	la 	$a0, OFILE	#puts file address in $a0
	li 	$a1, 1		#declears file for writing
	li	$a2, 0		
	syscall
	move 	$s2, $v0	#puts FD of ouput file in s2
	beq	$s2, -1, B3	#branches to error statement if error
	j	C14		#jumps to next instruction if no error

#outputs write file open error
B3:	li 	$v0, 4		#print string syscall
	la 	$a0, OutOEr	#loads output error message
	syscall
	j 	EXIT		#jumps to exit

#writes to file	
C14:	li	$v0, 15		#write to file syscall
	move	$a0, $s2	#puts address of file in $a0
	la	$a1, BUF	#puts BUF address in $a1
	mul	$s1, $s1, 4
	move	$a2, $s1	#tells syscall how many bytes to write to the file
	syscall
	move 	$s3, $v0	#puts bytes read in $s3
	beq	$s3, 0, B4	#branches to error message if error
	j 	C15		#jump to next intruction if ok

#outputs write error
B4:	li 	$v0, 4		#output string syscall
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
