# Print friendly message to console
#
# 03/26/2020 JGregor Version 1

        .data 
TXT:    .asciiz "Hello world!"
    
        .text
PHW:    la   $a0, TXT    # print TXT string
        li   $v0, 4      # 
        syscall 
     
        li   $v0, 10     # exit program
        syscall
