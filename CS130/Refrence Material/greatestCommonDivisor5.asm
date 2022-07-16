# Compute greatest common divisor of two numbers
#
# 04/07/2020 JGregor Version 4 (nested subroutine calls)

        .data

TXT1:   .asciiz "Enter X> "
TXT2:   .asciiz "Enter Y> "
TXT3:   .asciiz "GCD(X,Y) = "

        .text
#------------------------------------------------------------
# Main program
# s0: X
# s1: Y
# s2: M=GCD(X,Y)

MAIN:   la   $a0, TXT1      # a0: X prompt
        jal  READ           #
        move $s0, $v0       # s0 <- X

        la   $a0, TXT2      # a0: Y prompt
        jal  READ           #
        move $s1, $v0       # s1 <- Y

        move $a0, $s0       # a0: X
        move $a1, $s1       # a1: Y
        jal  GCD
        move $s2, $v0       # s2 <- GCD(X,Y)

        la   $a0, TXT3      # a0: message
        move $a1, $s2       # a1: GCD(X,Y)
        jal  PRINT
        
EXIT:   li   $v0, 10        # exit program
        syscall             # 

#------------------------------------------------------------
# int READ(a0: *text)
# t0: *text

        .text
READ:   addi $sp, $sp, -4   # stack: grow size
        sw   $ra, 0($sp)    # stack: push $ra

        move $t0, $a0       # text

        move $a0, $t0       # a0: text
        li   $v0, 4         # print 
        syscall             # 

        li   $v0, 5         # read integer 
        syscall             # 

        lw   $ra, 0($sp)    # stack: pop $ra
        addi $sp, $sp, 4    # stack: shrink size

        jr   $ra            # return

#------------------------------------------------------------
# int GCD(a0: X, a1: Y)
# s0: X copy
# s1: Y copy
# t0: computation

        .data  
DBG:    .asciiz "R "

        .text
GCD:    addi $sp, $sp, -12  # stack: grow size
        sw   $ra, 8($sp)    # stack: push $ra
        sw   $s1, 4($sp)    # stack: push $s1
        sw   $s0, 0($sp)    # stack: push $s0

        move $s0, $a0       # s0 <- X
        move $s1, $a1       # s1 <- Y

C0:     beq  $s1, $0, C1    # while (Y != 0) {

        move $t0, $s1       #   tmp = Y
        div  $s0, $s1       #   Y = X % Y
        mfhi $s1            #  
        move $s0, $t0       #   X = tmp

        la   $a0, DBG       # a0: text
        move $a1, $s1       # a1: Y (remainder)
        jal  PRINT          # print message

        b    C0             # }

C1:     move $v0, $s0       # v0 <- result (X)

        lw   $s0, 0($sp)    # stack: push $ra
        lw   $s1, 4($sp)    # stack: push $ra
        lw   $ra, 8($sp)    # stack: push $ra
        addi $sp, $sp, 12   # stack: shrink size

        jr   $ra            # return

#------------------------------------------------------------
# void PRINT(a0: *text, a1: M)
# t0: *text
# t1: M

        .text
PRINT:  addi $sp, $sp, -4   # stack: grow size
        sw   $ra, 0($sp)    # stack: push $ra

        move $t0, $a0       # text
        move $t1, $a1       # M

        move $a0, $t0       # a0: text
        li   $v0, 4         # print 
        syscall             #

        move $a0, $a1       # a0: M
        li   $v0, 1         # print 
        syscall             #

        addi $a0, $0, '\n'  # a0: newline
        li   $v0, 11        # print
        syscall             #

        lw   $ra, 0($sp)    # stack: push $ra
        addi $sp, $sp, 4    # stack: shrink size

        jr   $ra            # return