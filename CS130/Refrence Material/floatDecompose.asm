# Decompose floating-point number
#
# 03/30/2020 JGregor Version 1

        .data

SIGN:   .word   0x80000000
BEXP:   .word   0x7f800000
FRAC:   .word   0x007fffff

TXT1:   .asciiz "Enter X> "
TXT2:   .asciiz "+-"
TXT3:   .asciiz " x 2^"

        .text

FLOAT:  la   $a0, TXT1      # print prompt
        li   $v0, 4         # 
        syscall             # 

        li   $v0, 6         # read float 
        syscall             # 
        mfc1 $t0, $f0       # t0 <- f0

        la   $t1, SIGN      # extract sign 
        lw   $t1, 0($t1)    #
        and  $t1, $t0, $t1  # t1 = t0 & SIGN
        srl  $t1, $t1, 31   # t1 = t1 >> 31

        la   $t2, BEXP      # extract exponent
        lw   $t2, 0($t2)    #
        and  $t2, $t0, $t2  # t2 = t0 & BEXP
        srl  $t2, $t2, 23   # t2 = t2 >> 23
        subi $t2, $t2, 127  # t2 = t2 - 127

        la   $t3, FRAC      # extract fraction
        lw   $t3, 0($t3)    #
        and  $t3, $t0, $t3  # t3 = t0 & FRAC
        ori  $t3, 0x800000  # t3 = 1.fraction

        la   $a0, TXT2      # print sign (char)
        add  $a0, $a0, $t1  # 0 is '+', 1 is '-'
        lbu  $a0, 0($a0)    # 
        li   $v0, 11        # 
        syscall

        lui  $t4, 0x3f80    # print fraction (float)
        or   $t4, $t3, $t4  # t4 = t3 | 0x3f80000
        mtc1 $t4, $f12      # f12 <- t4
        li   $v0, 2         #
        syscall             #

        la   $a0, TXT3      # print exponent (string)
        li   $v0, 4         # 
        syscall

        move $a0, $t2       # print exponent (integer)
        li   $v0, 1         #
        syscall             #

        li   $a0, '\n'      # print newline (char)
        li   $v0, 11        # 
        syscall

EXIT:   li   $v0, 10        # exit program
        syscall             # 
