# Print friendly message to console
#
# 03/30/2020 JGregor Version 1

        .data 
TXT:    .asciiz "Hello world!"
    
        .text
PHW:    la   $t0, TXT    # char *p = &TXT[0]

        lbu  $t1, 0($t0) # char c = *p
        beqz $t1, EXIT   # if (c == 0), exit

L0:                      # do {
        move $a0, $t1    #   print c
        li   $v0, 11     # 
        syscall          #
        addi $t0, $t0, 1 #   p++
        lbu  $t1, 0($t0) #   c = *p
        bgtz $t1, L0     # } while (0 < c)
     
EXIT:   li   $v0, 10     # exit program
        syscall
