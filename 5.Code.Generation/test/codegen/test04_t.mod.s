##################################################
# test04
#

    #-----------------------------------------
    # text section
    #
    .text
    .align 4

    # entry point and pre-defined functions
    .global main
    .extern DIM
    .extern DOFS
    .extern ReadInt
    .extern WriteInt
    .extern WriteStr
    .extern WriteChar
    .extern WriteLn

    # scope test04
main:
    #    -16(%ebp)   4  [ $t0       <int> %ebp-16 ]

    # stack offset 4
    # function prologue 4
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $4, %esp               
    cld                            
    xorl    %eax, %eax             
    movl    $1, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    movl    $0, %eax                #   0:     assign i <- 0
    movl    %eax, i                
l_test04_2_while_cond:
    movl    i, %eax                 #   2:     if     i < 10 goto 3_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test04_3_while_body  
    jmp     l_test04_1              #   3:     goto   1
l_test04_3_while_body:
    movl    i, %eax                 #   5:     assign a <- i
    movl    %eax, a                
    movl    a, %eax                 #   6:     param  0 <- a
    pushl   %eax                   
    call    WriteInt                #   7:     call   WriteInt
    addl    $4, %esp               
    movl    i, %eax                 #   8:     add    t0 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   9:     assign i <- t0
    movl    %eax, i                
    jmp     l_test04_2_while_cond   #  10:     goto   2_while_cond
l_test04_1:

l_test04_exit:
    # epilogue 
    addl    $4, %esp               
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # end of text section
    #-----------------------------------------

    #-----------------------------------------
    # global data section
    #
    .data
    .align 4

    # scope: test04
a:                                  # <int>
    .skip    4
i:                                  # <int>
    .skip    4

    # end of global data section
    #-----------------------------------------

    .end
##################################################
