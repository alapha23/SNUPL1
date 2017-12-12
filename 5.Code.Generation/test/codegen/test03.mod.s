##################################################
# test03
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

    # scope test03
main:
    #    -16(%ebp)   4  [ $t0       <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t2       <int> %ebp-24 ]

    # stack offset 12
    # function prologue 12
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $12, %esp              

    movl    $1, %eax                #   0:     assign a <- 1
    movb    %al, a                 
    movl    $0, %eax                #   1:     assign b <- 0
    movb    %al, b                 
    movzbl  a, %eax                 #   2:     and    t0 <- a, b
    movzbl  b, %ebx                
    andl    %eax, %ebx             
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   3:     assign c <- t0
    movb    %al, c                 
    movzbl  c, %eax                 #   4:     if     c = 1 goto 4_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    je      l_test03_4_if_true     
    jmp     l_test03_5_if_false     #   5:     goto   5_if_false
l_test03_4_if_true:
    movl    $1, %eax                #   7:     param  0 <- 1
    pushl   %eax                   
    call    WriteInt                #   8:     call   WriteInt
    addl    $4, %esp               
    jmp     l_test03_3              #   9:     goto   3
l_test03_5_if_false:
    movl    $0, %eax                #  11:     param  0 <- 0
    pushl   %eax                   
    call    WriteInt                #  12:     call   WriteInt
    addl    $4, %esp               
l_test03_3:
    movzbl  a, %eax                 #  14:     or     t1 <- a, b
    movzbl  b, %ebx                
    orl     %eax, %ebx             
    movl    %eax, -20(%ebp)        
    movl    -20(%ebp), %eax         #  15:     assign c <- t1
    movb    %al, c                 
    movzbl  c, %eax                 #  16:     if     c = 1 goto 10_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    je      l_test03_10_if_true    
    jmp     l_test03_11_if_false    #  17:     goto   11_if_false
l_test03_10_if_true:
    movl    $1, %eax                #  19:     param  0 <- 1
    pushl   %eax                   
    call    WriteInt                #  20:     call   WriteInt
    addl    $4, %esp               
    jmp     l_test03_9              #  21:     goto   9
l_test03_11_if_false:
    movl    $0, %eax                #  23:     param  0 <- 0
    pushl   %eax                   
    call    WriteInt                #  24:     call   WriteInt
    addl    $4, %esp               
l_test03_9:
    movzbl  a, %eax                 #  26:     or     t2 <- a, b
    movzbl  b, %ebx                
    orl     %eax, %ebx             
    movl    %eax, -24(%ebp)        
    movl    -24(%ebp), %eax         #  27:     assign c <- t2
    movb    %al, c                 
    movzbl  c, %eax                 #  28:     if     c = 1 goto 16_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    je      l_test03_16_if_true    
    jmp     l_test03_17_if_false    #  29:     goto   17_if_false
l_test03_16_if_true:
    movl    $1, %eax                #  31:     param  0 <- 1
    pushl   %eax                   
    call    WriteInt                #  32:     call   WriteInt
    addl    $4, %esp               
    jmp     l_test03_15             #  33:     goto   15
l_test03_17_if_false:
    movl    $0, %eax                #  35:     param  0 <- 0
    pushl   %eax                   
    call    WriteInt                #  36:     call   WriteInt
    addl    $4, %esp               
l_test03_15:

l_test03_exit:
    # epilogue 
    addl    $12, %esp              
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

    # scope: test03
a:                                  # <bool>
    .skip    1
b:                                  # <bool>
    .skip    1
    .align   4
b1:                                 # <int>
    .skip    4
c:                                  # <bool>
    .skip    1

    # end of global data section
    #-----------------------------------------

    .end
##################################################
