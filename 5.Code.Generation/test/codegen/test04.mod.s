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
    #    -16(%ebp)   4  [ $t0       <ptr(4) to <array 10 of <bool>>> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t10      <ptr(4) to <array 10 of <bool>>> %ebp-24 ]
    #    -28(%ebp)   4  [ $t11      <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t12      <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t13      <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t14      <int> %ebp-40 ]
    #    -44(%ebp)   4  [ $t2       <ptr(4) to <array 10 of <bool>>> %ebp-44 ]
    #    -48(%ebp)   4  [ $t3       <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t4       <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t5       <int> %ebp-56 ]
    #    -57(%ebp)   1  [ $t6       <bool> %ebp-57 ]
    #    -64(%ebp)   4  [ $t7       <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t8       <ptr(4) to <array 10 of <bool>>> %ebp-68 ]
    #    -72(%ebp)   4  [ $t9       <int> %ebp-72 ]

    # stack offset 60
    # function prologue 60
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $60, %esp              
    cld                            
    xorl    %eax, %eax             
    movl    $15, %ecx              
    mov     %esp, %edi             
    rep     stosl                  

    movl    $0, %eax                #   0:     assign i <- 0
    movl    %eax, i                
l_test04_3_while_cond:
    movl    i, %eax                 #   2:     if     i < 10 goto 2_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test04_2_while_body  
    jmp     l_test04_1              #   3:     goto   1
l_test04_2_while_body:
    leal    a, %eax                 #   5:     &()    t0 <- a
    movl    %eax, -16(%ebp)        
    movl    i, %eax                 #   6:     mul    t1 <- i, 1
    movl    $1, %ebx               
    imull   %ebx                   
    movl    %eax, -20(%ebp)        
    leal    a, %eax                 #   7:     &()    t2 <- a
    movl    %eax, -44(%ebp)        
    movl    -44(%ebp), %eax         #   8:     param  0 <- t2
    pushl   %eax                   
    call    DOFS                    #   9:     call   t3 <- DOFS
    addl    $4, %esp               
    movl    %eax, -48(%ebp)        
    movl    -20(%ebp), %eax         #  10:     add    t4 <- t1, t3
    movl    -48(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -52(%ebp)        
    movl    -16(%ebp), %eax         #  11:     add    t5 <- t0, t4
    movl    -52(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -56(%ebp)        
    movl    i, %eax                 #  12:     if     i > 2 goto 6
    movl    $2, %ebx               
    cmpl    %ebx, %eax             
    jg      l_test04_6             
    jmp     l_test04_7              #  13:     goto   7
l_test04_6:
    movl    $1, %eax                #  15:     assign t6 <- 1
    movb    %al, -57(%ebp)         
    jmp     l_test04_8              #  16:     goto   8
l_test04_7:
    movl    $0, %eax                #  18:     assign t6 <- 0
    movb    %al, -57(%ebp)         
l_test04_8:
    movzbl  -57(%ebp), %eax         #  20:     assign @t5 <- t6
    movl    -56(%ebp), %edi        
    movb    %al, (%edi)            
    movl    i, %eax                 #  21:     add    t7 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -64(%ebp)        
    movl    -64(%ebp), %eax         #  22:     assign i <- t7
    movl    %eax, i                
    jmp     l_test04_3_while_cond   #  23:     goto   3_while_cond
l_test04_1:
    movl    $0, %eax                #  25:     assign i <- 0
    movl    %eax, i                
l_test04_14_while_cond:
    movl    i, %eax                 #  27:     if     i < 10 goto 13_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test04_13_while_body 
    jmp     l_test04_12             #  28:     goto   12
l_test04_13_while_body:
    leal    a, %eax                 #  30:     &()    t8 <- a
    movl    %eax, -68(%ebp)        
    movl    i, %eax                 #  31:     mul    t9 <- i, 1
    movl    $1, %ebx               
    imull   %ebx                   
    movl    %eax, -72(%ebp)        
    leal    a, %eax                 #  32:     &()    t10 <- a
    movl    %eax, -24(%ebp)        
    movl    -24(%ebp), %eax         #  33:     param  0 <- t10
    pushl   %eax                   
    call    DOFS                    #  34:     call   t11 <- DOFS
    addl    $4, %esp               
    movl    %eax, -28(%ebp)        
    movl    -72(%ebp), %eax         #  35:     add    t12 <- t9, t11
    movl    -28(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -32(%ebp)        
    movl    -68(%ebp), %eax         #  36:     add    t13 <- t8, t12
    movl    -32(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -36(%ebp)        
    movl    -36(%ebp), %edi        
    movzbl  (%edi), %eax            #  37:     if     @t13 = 1 goto 18_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    je      l_test04_18_if_true    
    jmp     l_test04_17_if_false    #  38:     goto   17_if_false
l_test04_18_if_true:
    movl    $1, %eax                #  40:     param  0 <- 1
    pushl   %eax                   
    call    WriteInt                #  41:     call   WriteInt
    addl    $4, %esp               
    jmp     l_test04_16             #  42:     goto   16
l_test04_17_if_false:
    movl    $0, %eax                #  44:     param  0 <- 0
    pushl   %eax                   
    call    WriteInt                #  45:     call   WriteInt
    addl    $4, %esp               
l_test04_16:
    movl    i, %eax                 #  47:     add    t14 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -40(%ebp)        
    movl    -40(%ebp), %eax         #  48:     assign i <- t14
    movl    %eax, i                
    jmp     l_test04_14_while_cond  #  49:     goto   14_while_cond
l_test04_12:

l_test04_exit:
    # epilogue 
    addl    $60, %esp              
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
a:                                  # <array 10 of <bool>>
    .long    1
    .long   10
    .skip   10
    .align   4
i:                                  # <int>
    .skip    4

    # end of global data section
    #-----------------------------------------

    .end
##################################################
