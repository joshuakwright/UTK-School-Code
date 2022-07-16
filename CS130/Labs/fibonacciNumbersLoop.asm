# Compute Fibonacci number sequence, store values in array, then print
#
# 03/25/2020 JGregor Modified sequence to 0, 1, 1, 2, 3, ..., changed
#                    array bounds handling, loop control, etc

        .data
K:      .word   0
F:      .word   0 : 25        # allocate array F[25] initialized to 0 : array F[25] = 0;
N:      .word   25            # max number of elements in F array     : int N = 25;
M:      .word   25            # number of Fibonacci numbers to compute: int M = 25;

TXT3:   .asciiz "Num. iterations (0 <= K): " #default value of K is 0 : string TXT3 = ""
	
TXT1:   .asciiz "Last index (0 <= M < 25): "  # default value is N-1  : string TXT1 = ""

        .text
	
L3:     la $s3, K             # load address of K variable 	: 
	lw $s3, 0($s3)	      # load value of K			: k = 0;
	
#Have user input last fibonacci loop to generate
	la $a0, TXT3	      #load address of text prompt for syscall :
	li $v0, 4	      # specify print string service	       : cout << TXT3;
	syscall		      # print prompt text string	       :
	
	li $v0, 5	      # specify read integer service	       : cin >> K;
	syscall		      # read K (result stored in $v0)	       :
	
	blt $v0, $0, L3       # check lower bound -- if invalid, restart: while (k < 0) {cin >> K}
	
	move $s3, $v0         #sets the value input to the register s3 which is K\
	
	addi $a0, $0, '\n'    # set ASCII for newline
        addi $v0, $0, 11      # specify Print Single Char: cout << "\n";
        syscall
	
	beqz $s3, EXIT        # while (0 < k), repeat computation : if (K == 0) {return 0;}

FNS:    la   $s0, F           # load address of F array

        la   $s1, N           # load address of N variable
        lw   $s1, 0($s1)      # load value of N

# Have user input last Fibonacci number to generate (0 <= M < N)
L0:    la   $a0, TXT1        # load address of text prompt for syscall :
       li   $v0, 4           # specify Print String service            : cout << TXT1;
       syscall               # print prompt text string                :

       li   $v0, 5           # specify Read Integer service :
       syscall               # read M (result stored in $v0): cin >> M;

       bgeu $v0, $s1, L0      #while ( M > 25 || M < 0) {L0}
       #blt  $v0, $0, L0      # check lower bound -- if invalid, restart
       #bge  $v0, $s1, L0     # check upper bound -- if invalid, restart

       addi $t0, $v0, 1      # set M = M + 1 : M++;

       la   $t1, M           # load address of M variable
       sw   $t0, ($t1)       # store value of M 

        la   $s2, M           # load address of M variable
        lw   $s2, 0($s2)      # load value of M

# Compute each Fibonacci number using the previous two Fibonacci numbers
        sw   $0, 0($s0)       # store F[0] = 0
        li   $t0, 1           #
        sw   $t0, 4($s0)      # store F[1] = 1

        addi $s2, $s2, -2     # decrement M (two numbers done) : M--;
      
L1:     lw   $t0, 0($s0)      # load F[i-2] (initially, $s0+0 == &F[0]) 
        lw   $t1, 4($s0)      # load F[i-1] (initially, $s0+4 == &F[1])
        add  $t2, $t0, $t1    # F[i] = F[i-2] + F[i-1]
        sw   $t2, 8($s0)      # store computed number  ($s0+8 == &F[i])

        addi $s0, $s0, 4      # increment base address (to &F[i+1])
        addi $s2, $s2, -1     # decrement M (one more number done) : M--;

        bgtz $s2, L1          # while (0 < M), repeat computation
      
# Print Fibonacci number sequence using subroutine (function)
        la   $a0, F           # load address of F array
        la   $a1, M           # load address of M variable
        jal  PRINT            # call print subroutine : print();
        
	addi $s3, $s3, -1     # decrement K (one more number done) : K--;
	
	bgtz $s3, FNS          # while (0 < k), repeat computation
        
EXIT:   li   $v0, 10          # specify Exit service : return 0;
        syscall               # 
               
# Subroutine: PRINT 

        .data
TXT2:   .asciiz  "F:"	#: string TXT2 = "F:"

        .text
        #for (int i; i < M; i++) {cout << F[i] << " ";}
        #cout << "\n";
PRINT:  add  $t0, $a0, $0     # set starting address of array of data to be printed
        lw   $t1, 0($a1)      # set loop counter to M: 

        la   $a0, TXT2        # load address of print heading text string : 
        li   $v0, 4           # specify Print String service  		  :cout << TXT2; 
        syscall               # print heading text string		  :
      
L2:     addi $a0, $0, ' '     # set ASCII for space
        addi $v0, $0, 11      # specify Print Single Char
        syscall

        lw   $a0, 0($t0)      # load F[i] (initially, F[0])
        li   $v0, 1           # specify Print Integer service : cout << F[i];
        syscall               # print Fibonacci number

        addi $t0, $t0, 4      # increment base address : i++
        addi $t1, $t1, -1     # decrement loop counter

        bgtz $t1, L2          # while not finished, repeat computation: for loop

        addi $a0, $0, '\n'    # set ASCII for newline
        addi $v0, $0, 11      # specify Print Single Char: cout << "\n";
        syscall
      
        jr   $ra              # return from subroutine 
