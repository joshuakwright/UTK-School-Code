# Compute greatest common divisor of two numbers
#
# 04/05/2020 JGregor Version 1

        .data

TXT1:   .asciiz "Enter X> "
TXT2:   .asciiz "Enter Y> "
TXT3:   .asciiz "GCD(X,Y) = "

        .text
# Prompt user for X and Y values
# s0: X
# s1: Y

C0:     la   $a0, TXT1      # a0: X prompt
        li   $v0, 4         # print text
        syscall             # 

        li   $v0, 5         # read integer
        syscall             # 
        move  $s0, $v0      # s0 <- X

        la   $a0, TXT2      # a0: Y prompt
        li   $v0, 4         # print text
        syscall             # 

        li   $v0, 5         # read integer
        syscall             # 
        move  $s1, $v0      # s1 <- Y

# Compute GCD(X,Y) using Euclid's Algorithm
# t0: X copy
# t1: Y copy
# t2: computation

        move $t0, $s0       # t0 <- X
        move $t1, $s1       # t1 <- Y

C1:     beq  $t1, $0, C2    # while (Y != 0) {
        move $t2, $t1       #   tmp = Y
        div  $t0, $t1       #   Y = X % Y
        mfhi $t1            #  
        move $t0, $t2       #   X = tmp
        b    C1             # }

# Print GCD(X,Y) and exit

C2:     la   $a0, TXT3      # a0: message
        li   $v0, 4         # print 
        syscall             #

        move $a0, $t0       # a0: integer
        li   $v0, 1         # print
        syscall             #

	addi $a0, $0, '\n'  # a0: newline
        li   $v0, 11        # print
        syscall             #
        
EXIT:   li   $v0, 10        # exit program
        syscall             # 
