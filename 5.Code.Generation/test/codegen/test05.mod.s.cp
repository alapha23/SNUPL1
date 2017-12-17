##################################################
# test05
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
    # stack offsets:
    #      8(%ebp)   4  [ %a        <ptr(4) to <array 10 of <int>>> %ebp+8 ]
    #    -16(%ebp)   4  [ $i        <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t10      <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t11      <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t12      <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t13      <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t14      <int> %ebp-40 ]
    #    -44(%ebp)   4  [ $t15      <int> %ebp-44 ]
    #    -48(%ebp)   4  [ $t16      <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t17      <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t18      <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t19      <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t2       <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t20      <int> %ebp-68 ]
    #    -72(%ebp)   4  [ $t21      <int> %ebp-72 ]
    #    -76(%ebp)   4  [ $t22      <int> %ebp-76 ]
    #    -80(%ebp)   4  [ $t23      <int> %ebp-80 ]
    #    -84(%ebp)   4  [ $t3       <int> %ebp-84 ]
    #    -88(%ebp)   4  [ $t4       <int> %ebp-88 ]
    #    -92(%ebp)   4  [ $t5       <int> %ebp-92 ]
    #    -96(%ebp)   4  [ $t6       <int> %ebp-96 ]
    #   -100(%ebp)   4  [ $t7       <int> %ebp-100 ]
    #   -104(%ebp)   4  [ $t8       <int> %ebp-104 ]
    #   -108(%ebp)   4  [ $t9       <int> %ebp-108 ]

    # prologue
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                    # save callee saved registers
    pushl   %esi                   
    pushl   %edi                   
    subl    $96, %esp               # make room for locals

    cld                             # memset local stack area to 0
    xorl    %eax, %eax             
    movl    $24, %ecx              
    mov     %esp, %edi             
    rep     stosl                  

    # function body
    movl    $0, %eax                #   0:     mul    t1 <- 0, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -20(%ebp)        
    movl    8(%ebp), %eax           #   1:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #   2:     call   t2 <- DOFS
    addl    $4, %esp               
    movl    %eax, -64(%ebp)        
    movl    -20(%ebp), %eax         #   3:     add    t3 <- t1, t2
    movl    -64(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -84(%ebp)        
    movl    8(%ebp), %eax           #   4:     add    t4 <- a, t3
    movl    -84(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -88(%ebp)        
    movl    $1, %eax                #   5:     assign @t4 <- 1
    movl    -88(%ebp), %edi        
    movl    %eax, (%edi)           
    movl    $1, %eax                #   6:     mul    t5 <- 1, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -92(%ebp)        
    movl    8(%ebp), %eax           #   7:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #   8:     call   t6 <- DOFS
    addl    $4, %esp               
    movl    %eax, -96(%ebp)        
    movl    -92(%ebp), %eax         #   9:     add    t7 <- t5, t6
    movl    -96(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -100(%ebp)       
    movl    8(%ebp), %eax           #  10:     add    t8 <- a, t7
    movl    -100(%ebp), %ebx       
    addl    %ebx, %eax             
    movl    %eax, -104(%ebp)       
    movl    $12, %eax               #  11:     assign @t8 <- 12
    movl    -104(%ebp), %edi       
    movl    %eax, (%edi)           
    movl    $2, %eax                #  12:     mul    t9 <- 2, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -108(%ebp)       
    movl    8(%ebp), %eax           #  13:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #  14:     call   t10 <- DOFS
    addl    $4, %esp               
    movl    %eax, -24(%ebp)        
    movl    -108(%ebp), %eax        #  15:     add    t11 <- t9, t10
    movl    -24(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -28(%ebp)        
    movl    8(%ebp), %eax           #  16:     add    t12 <- a, t11
    movl    -28(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -32(%ebp)        
    movl    $1123, %eax             #  17:     assign @t12 <- 1123
    movl    -32(%ebp), %edi        
    movl    %eax, (%edi)           
    movl    $1, %eax                #  18:     assign i <- 1
    movl    %eax, -16(%ebp)        
l_test_5_while_cond:
    movl    -16(%ebp), %eax         #  20:     if     i < 10 goto 6_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test_6_while_body    
    jmp     l_test_4                #  21:     goto   4
l_test_6_while_body:
    movl    $10, %eax               #  23:     sub    t13 <- 10, i
    movl    -16(%ebp), %ebx        
    subl    %ebx, %eax             
    movl    %eax, -36(%ebp)        
    movl    -16(%ebp), %eax         #  24:     mul    t14 <- i, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -40(%ebp)        
    movl    8(%ebp), %eax           #  25:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #  26:     call   t15 <- DOFS
    addl    $4, %esp               
    movl    %eax, -44(%ebp)        
    movl    -40(%ebp), %eax         #  27:     add    t16 <- t14, t15
    movl    -44(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -48(%ebp)        
    movl    8(%ebp), %eax           #  28:     add    t17 <- a, t16
    movl    -48(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -52(%ebp)        
    movl    -36(%ebp), %eax         #  29:     assign @t17 <- t13
    movl    -52(%ebp), %edi        
    movl    %eax, (%edi)           
    movl    -16(%ebp), %eax         #  30:     add    t18 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -56(%ebp)        
    movl    -56(%ebp), %eax         #  31:     assign i <- t18
    movl    %eax, -16(%ebp)        
    jmp     l_test_5_while_cond     #  32:     goto   5_while_cond
l_test_4:
    movl    $0, %eax                #  34:     assign i <- 0
    movl    %eax, -16(%ebp)        
l_test_12_while_cond:
    movl    -16(%ebp), %eax         #  36:     if     i < 10 goto 13_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test_13_while_body   
    jmp     l_test_11               #  37:     goto   11
l_test_13_while_body:
    movl    -16(%ebp), %eax         #  39:     mul    t19 <- i, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -60(%ebp)        
    movl    8(%ebp), %eax           #  40:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #  41:     call   t20 <- DOFS
    addl    $4, %esp               
    movl    %eax, -68(%ebp)        
    movl    -60(%ebp), %eax         #  42:     add    t21 <- t19, t20
    movl    -68(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -72(%ebp)        
    movl    8(%ebp), %eax           #  43:     add    t22 <- a, t21
    movl    -72(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -76(%ebp)        
    movl    -76(%ebp), %edi        
    movl    (%edi), %eax            #  44:     param  0 <- @t22
    pushl   %eax                   
    call    WriteInt                #  45:     call   WriteInt
    addl    $4, %esp               
    movl    -16(%ebp), %eax         #  46:     add    t23 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -80(%ebp)        
    movl    -80(%ebp), %eax         #  47:     assign i <- t23
    movl    %eax, -16(%ebp)        
    jmp     l_test_12_while_cond    #  48:     goto   12_while_cond
l_test_11:

l_test_exit:
    # epilogue
    addl    $96, %esp               # remove locals
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # scope test05
main:
    # stack offsets:
    #    -16(%ebp)   4  [ $t0       <ptr(4) to <array 10 of <int>>> %ebp-16 ]

    # prologue
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                    # save callee saved registers
    pushl   %esi                   
    pushl   %edi                   
    subl    $4, %esp                # make room for locals

    xorl    %eax, %eax              # memset local stack area to 0
    movl    %eax, 0(%esp)          

    # function body
    leal    a, %eax                 #   0:     &()    t0 <- a
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   1:     param  0 <- t0
    pushl   %eax                   
    call    test                    #   2:     call   test
    addl    $4, %esp               

l_test05_exit:
    # epilogue
    addl    $4, %esp                # remove locals
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

    # scope: test05
a:                                  # <array 10 of <int>>
    .long    1
    .long   10
    .skip   40


    # end of global data section
    #-----------------------------------------

    .end
##################################################
