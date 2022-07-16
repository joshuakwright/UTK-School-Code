# Print friendly message to console
# along with length of message (strlen)
#
# 04/02/2020 JGregor Version 3

        .data 
TXT:    .asciiz "Hello world!"
    
        .text
PHW:    la   $t0, TXT    # char *p = &TXT[0]
        
        li   $t2, 0      # int N = 0

        lbu  $t1, 0($t0) # char c = *p
        beqz $t1, DONE   # if (c == 0), exit

L0:                      # do {
        move $a0, $t1    #   print c
        li   $v0, 11     # 
        syscall          #

        addi $t2, $t2, 1 #   N++

        addi $t0, $t0, 1 #   p++
        lbu  $t1, 0($t0) #   c = *p
        bgtz $t1, L0     # } while (0 < c)
     
DONE:   li   $a0, '\n'   # print newline
        li   $v0, 11     #
        syscall          #

        move $a0, $t2    # print N
        li   $v0, 1      #
        syscall          #

EXIT:   li   $v0, 10     # exit program
        syscall
