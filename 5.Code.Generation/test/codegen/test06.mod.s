##################################################
# test06
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

    # scope test
test:
    #      8(%ebp)   4  [ %a        <ptr(4) to <array 10 of <bool>>> %ebp+8 ]
    #    -16(%ebp)   4  [ $i        <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t10      <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t11      <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t2       <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t3       <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t4       <int> %ebp-40 ]
    #    -41(%ebp)   1  [ $t5       <bool> %ebp-41 ]
    #    -48(%ebp)   4  [ $t6       <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t7       <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t8       <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t9       <int> %ebp-60 ]

    # stack offset 48
    # function prologue 48
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $48, %esp              
    cld                            
    xorl    %eax, %eax             
    movl    $12, %ecx              
    mov     %esp, %edi             
    rep     stosl                  

    movl    $0, %eax                #   0:     assign i <- 0
    movl    %eax, -16(%ebp)        
l_test_3_while_cond:
    movl    -16(%ebp), %eax         #   2:     if     i < 10 goto 2_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test_2_while_body    
    jmp     l_test_1                #   3:     goto   1
l_test_2_while_body:
    movl    -16(%ebp), %eax         #   5:     mul    t1 <- i, 1
    movl    $1, %ebx               
    imull   %ebx                   
    movl    %eax, -20(%ebp)        
    movl    8(%ebp), %eax           #   6:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #   7:     call   t2 <- DOFS
    addl    $4, %esp               
    movl    %eax, -32(%ebp)        
    movl    -20(%ebp), %eax         #   8:     add    t3 <- t1, t2
    movl    -32(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -36(%ebp)        
    movl    8(%ebp), %eax           #   9:     add    t4 <- a, t3
    movl    -36(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -40(%ebp)        
    movl    -16(%ebp), %eax         #  10:     if     i > 2 goto 6
    movl    $2, %ebx               
    cmpl    %ebx, %eax             
    jg      l_test_6               
    jmp     l_test_7                #  11:     goto   7
l_test_6:
    movl    $1, %eax                #  13:     assign t5 <- 1
    movb    %al, -41(%ebp)         
    jmp     l_test_8                #  14:     goto   8
l_test_7:
    movl    $0, %eax                #  16:     assign t5 <- 0
    movb    %al, -41(%ebp)         
l_test_8:
    movzbl  -41(%ebp), %eax         #  18:     assign @t4 <- t5
    movl    -40(%ebp), %edi        
    movb    %al, (%edi)            
    movl    -16(%ebp), %eax         #  19:     add    t6 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -48(%ebp)        
    movl    -48(%ebp), %eax         #  20:     assign i <- t6
    movl    %eax, -16(%ebp)        
    jmp     l_test_3_while_cond     #  21:     goto   3_while_cond
l_test_1:
    movl    $0, %eax                #  23:     assign i <- 0
    movl    %eax, -16(%ebp)        
l_test_14_while_cond:
    movl    -16(%ebp), %eax         #  25:     if     i < 10 goto 13_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test_13_while_body   
    jmp     l_test_12               #  26:     goto   12
l_test_13_while_body:
    movl    -16(%ebp), %eax         #  28:     mul    t7 <- i, 1
    movl    $1, %ebx               
    imull   %ebx                   
    movl    %eax, -52(%ebp)        
    movl    8(%ebp), %eax           #  29:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #  30:     call   t8 <- DOFS
    addl    $4, %esp               
    movl    %eax, -56(%ebp)        
    movl    -52(%ebp), %eax         #  31:     add    t9 <- t7, t8
    movl    -56(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -60(%ebp)        
    movl    8(%ebp), %eax           #  32:     add    t10 <- a, t9
    movl    -60(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -24(%ebp)        
    movl    -24(%ebp), %edi        
    movzbl  (%edi), %eax            #  33:     if     @t10 = 1 goto 18_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    je      l_test_18_if_true      
    jmp     l_test_17_if_false      #  34:     goto   17_if_false
l_test_18_if_true:
    movl    $1, %eax                #  36:     param  0 <- 1
    pushl   %eax                   
    call    WriteInt                #  37:     call   WriteInt
    addl    $4, %esp               
    jmp     l_test_16               #  38:     goto   16
l_test_17_if_false:
    movl    $0, %eax                #  40:     param  0 <- 0
    pushl   %eax                   
    call    WriteInt                #  41:     call   WriteInt
    addl    $4, %esp               
l_test_16:
    movl    -16(%ebp), %eax         #  43:     add    t11 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -28(%ebp)        
    movl    -28(%ebp), %eax         #  44:     assign i <- t11
    movl    %eax, -16(%ebp)        
    jmp     l_test_14_while_cond    #  45:     goto   14_while_cond
l_test_12:

l_test_exit:
    # epilogue 
    addl    $48, %esp              
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # scope test06
main:
    #    -16(%ebp)   4  [ $t0       <ptr(4) to <array 10 of <bool>>> %ebp-16 ]

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

    leal    a, %eax                 #   0:     &()    t0 <- a
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   1:     param  0 <- t0
    pushl   %eax                   
    call    test                    #   2:     call   test
    addl    $4, %esp               

l_test06_exit:
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

    # scope: test06
a:                                  # <array 10 of <bool>>
    .long    1
    .long   10
    .skip   10


    # end of global data section
    #-----------------------------------------

    .end
##################################################
