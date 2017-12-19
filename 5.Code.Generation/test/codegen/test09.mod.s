##################################################
# test09
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
    #    -32(%ebp)  18  [ $a        <array 10 of <bool>> %ebp-32 ]
    #    -36(%ebp)   4  [ $i        <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t0       <ptr(4) to <array 10 of <bool>>> %ebp-40 ]
    #    -44(%ebp)   4  [ $t1       <int> %ebp-44 ]
    #    -48(%ebp)   4  [ $t10      <ptr(4) to <array 10 of <bool>>> %ebp-48 ]
    #    -52(%ebp)   4  [ $t11      <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t12      <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t13      <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t14      <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t2       <ptr(4) to <array 10 of <bool>>> %ebp-68 ]
    #    -72(%ebp)   4  [ $t3       <int> %ebp-72 ]
    #    -76(%ebp)   4  [ $t4       <int> %ebp-76 ]
    #    -80(%ebp)   4  [ $t5       <int> %ebp-80 ]
    #    -81(%ebp)   1  [ $t6       <bool> %ebp-81 ]
    #    -88(%ebp)   4  [ $t7       <int> %ebp-88 ]
    #    -92(%ebp)   4  [ $t8       <ptr(4) to <array 10 of <bool>>> %ebp-92 ]
    #    -96(%ebp)   4  [ $t9       <int> %ebp-96 ]

    # stack offset 84
    # function prologue 84
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $84, %esp              
    cld                            
    xorl    %eax, %eax             
    movl    $21, %ecx              
    mov     %esp, %edi             
    rep     stosl                  
    movl    $1, -32(%ebp)           # local array 'a':1dimensions
    movl    $10, -28(%ebp)          #   dimension 1: 10 elements

    movl    $0, %eax                #   0:     assign i <- 0
    movl    %eax, -36(%ebp)        
l_test_3_while_cond:
    movl    -36(%ebp), %eax         #   2:     if     i < 10 goto 2_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test_2_while_body    
    jmp     l_test_1                #   3:     goto   1
l_test_2_while_body:
    leal    -32(%ebp), %eax         #   5:     &()    t0 <- a
    movl    %eax, -40(%ebp)        
    movl    -36(%ebp), %eax         #   6:     mul    t1 <- i, 1
    movl    $1, %ebx               
    imull   %ebx                   
    movl    %eax, -44(%ebp)        
    leal    -32(%ebp), %eax         #   7:     &()    t2 <- a
    movl    %eax, -68(%ebp)        
    movl    -68(%ebp), %eax         #   8:     param  0 <- t2
    pushl   %eax                   
    call    DOFS                    #   9:     call   t3 <- DOFS
    addl    $4, %esp               
    movl    %eax, -72(%ebp)        
    movl    -44(%ebp), %eax         #  10:     add    t4 <- t1, t3
    movl    -72(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -76(%ebp)        
    movl    -40(%ebp), %eax         #  11:     add    t5 <- t0, t4
    movl    -76(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -80(%ebp)        
    movl    -36(%ebp), %eax         #  12:     if     i > 2 goto 6
    movl    $2, %ebx               
    cmpl    %ebx, %eax             
    jg      l_test_6               
    jmp     l_test_7                #  13:     goto   7
l_test_6:
    movl    $1, %eax                #  15:     assign t6 <- 1
    movb    %al, -81(%ebp)         
    jmp     l_test_8                #  16:     goto   8
l_test_7:
    movl    $0, %eax                #  18:     assign t6 <- 0
    movb    %al, -81(%ebp)         
l_test_8:
    movzbl  -81(%ebp), %eax         #  20:     assign @t5 <- t6
    movl    -80(%ebp), %edi        
    movb    %al, (%edi)            
    movl    -36(%ebp), %eax         #  21:     add    t7 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -88(%ebp)        
    movl    -88(%ebp), %eax         #  22:     assign i <- t7
    movl    %eax, -36(%ebp)        
    jmp     l_test_3_while_cond     #  23:     goto   3_while_cond
l_test_1:
    movl    $0, %eax                #  25:     assign i <- 0
    movl    %eax, -36(%ebp)        
l_test_14_while_cond:
    movl    -36(%ebp), %eax         #  27:     if     i < 10 goto 13_while_body
    movl    $10, %ebx              
    cmpl    %ebx, %eax             
    jl      l_test_13_while_body   
    jmp     l_test_12               #  28:     goto   12
l_test_13_while_body:
    leal    -32(%ebp), %eax         #  30:     &()    t8 <- a
    movl    %eax, -92(%ebp)        
    movl    -36(%ebp), %eax         #  31:     mul    t9 <- i, 1
    movl    $1, %ebx               
    imull   %ebx                   
    movl    %eax, -96(%ebp)        
    leal    -32(%ebp), %eax         #  32:     &()    t10 <- a
    movl    %eax, -48(%ebp)        
    movl    -48(%ebp), %eax         #  33:     param  0 <- t10
    pushl   %eax                   
    call    DOFS                    #  34:     call   t11 <- DOFS
    addl    $4, %esp               
    movl    %eax, -52(%ebp)        
    movl    -96(%ebp), %eax         #  35:     add    t12 <- t9, t11
    movl    -52(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -56(%ebp)        
    movl    -92(%ebp), %eax         #  36:     add    t13 <- t8, t12
    movl    -56(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -60(%ebp)        
    movl    -60(%ebp), %edi        
    movzbl  (%edi), %eax            #  37:     if     @t13 = 1 goto 18_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    je      l_test_18_if_true      
    jmp     l_test_17_if_false      #  38:     goto   17_if_false
l_test_18_if_true:
    movl    $1, %eax                #  40:     param  0 <- 1
    pushl   %eax                   
    call    WriteInt                #  41:     call   WriteInt
    addl    $4, %esp               
    jmp     l_test_16               #  42:     goto   16
l_test_17_if_false:
    movl    $0, %eax                #  44:     param  0 <- 0
    pushl   %eax                   
    call    WriteInt                #  45:     call   WriteInt
    addl    $4, %esp               
l_test_16:
    movl    -36(%ebp), %eax         #  47:     add    t14 <- i, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -64(%ebp)        
    movl    -64(%ebp), %eax         #  48:     assign i <- t14
    movl    %eax, -36(%ebp)        
    jmp     l_test_14_while_cond    #  49:     goto   14_while_cond
l_test_12:

l_test_exit:
    # epilogue 
    addl    $84, %esp              
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # scope test09
main:

    # stack offset 0
    # function prologue 0
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $0, %esp               
    cld                            
    xorl    %eax, %eax             
    movl    $0, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    call    test                    #   0:     call   test

l_test09_exit:
    # epilogue 
    addl    $0, %esp               
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



    # end of global data section
    #-----------------------------------------

    .end
##################################################
