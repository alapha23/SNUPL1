##################################################
# fibonacci
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

    # scope fib
fib:
    #      8(%ebp)   4  [ %n        <int> %ebp+8 ]
    #    -16(%ebp)   4  [ $t10      <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $t11      <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t12      <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t8       <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t9       <int> %ebp-32 ]

    # stack offset 20
    # function prologue 20
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $20, %esp              
    cld                            
    xorl    %eax, %eax             
    movl    $5, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    movl    8(%ebp), %eax           #   0:     if     n <= 1 goto 2_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    jle     l_fib_2_if_true        
    jmp     l_fib_1_if_false        #   1:     goto   1_if_false
l_fib_2_if_true:
    movl    8(%ebp), %eax           #   3:     return n
    jmp     l_fib_exit              #   3:     return n
    jmp     l_fib_0                 #   4:     goto   0
l_fib_1_if_false:
    movl    8(%ebp), %eax           #   6:     sub    t8 <- n, 1
    movl    $1, %ebx               
    subl    %ebx, %eax             
    movl    %eax, -28(%ebp)        
    movl    -28(%ebp), %eax         #   7:     param  0 <- t8
    pushl   %eax                   
    call    fib                     #   8:     call   t9 <- fib
    addl    $4, %esp               
    movl    %eax, -32(%ebp)        
    movl    8(%ebp), %eax           #   9:     sub    t10 <- n, 2
    movl    $2, %ebx               
    subl    %ebx, %eax             
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #  10:     param  0 <- t10
    pushl   %eax                   
    call    fib                     #  11:     call   t11 <- fib
    addl    $4, %esp               
    movl    %eax, -20(%ebp)        
    movl    -32(%ebp), %eax         #  12:     add    t12 <- t9, t11
    movl    -20(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -24(%ebp)        
    movl    -24(%ebp), %eax         #  13:     return t12
    jmp     l_fib_exit              #  13:     return t12
l_fib_0:

l_fib_exit:
    # epilogue 
    addl    $20, %esp              
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # scope ReadNumber
ReadNumber:
    #    -16(%ebp)   4  [ $i        <int> %ebp-16 ]
    #      8(%ebp)   4  [ %str      <ptr(4) to <array of <char>>> %ebp+8 ]
    #    -20(%ebp)   4  [ $t8       <int> %ebp-20 ]

    # stack offset 8
    # function prologue 8
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $8, %esp               
    cld                            
    xorl    %eax, %eax             
    movl    $2, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    movl    8(%ebp), %eax           #   0:     param  0 <- str
    pushl   %eax                   
    call    WriteStr                #   1:     call   WriteStr
    addl    $4, %esp               
    call    ReadInt                 #   2:     call   t8 <- ReadInt
    movl    %eax, -20(%ebp)        
    movl    -20(%ebp), %eax         #   3:     assign i <- t8
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   4:     return i
    jmp     l_ReadNumber_exit       #   4:     return i

l_ReadNumber_exit:
    # epilogue 
    addl    $8, %esp               
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # scope fibonacci
main:
    #    -16(%ebp)   4  [ $t0       <ptr(4) to <array 18 of <char>>> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <ptr(4) to <array 29 of <char>>> %ebp-20 ]
    #    -24(%ebp)   4  [ $t2       <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t3       <ptr(4) to <array 11 of <char>>> %ebp-28 ]
    #    -32(%ebp)   4  [ $t4       <ptr(4) to <array 5 of <char>>> %ebp-32 ]
    #    -36(%ebp)   4  [ $t5       <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t6       <ptr(4) to <array 29 of <char>>> %ebp-40 ]
    #    -44(%ebp)   4  [ $t7       <int> %ebp-44 ]

    # stack offset 32
    # function prologue 32
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $32, %esp              
    cld                            
    xorl    %eax, %eax             
    movl    $8, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    leal    _str_6, %eax            #   0:     &()    t0 <- _str_6
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   1:     param  0 <- t0
    pushl   %eax                   
    call    WriteStr                #   2:     call   WriteStr
    addl    $4, %esp               
    call    WriteLn                 #   3:     call   WriteLn
    call    WriteLn                 #   4:     call   WriteLn
    leal    _str_7, %eax            #   5:     &()    t1 <- _str_7
    movl    %eax, -20(%ebp)        
    movl    -20(%ebp), %eax         #   6:     param  0 <- t1
    pushl   %eax                   
    call    ReadNumber              #   7:     call   t2 <- ReadNumber
    addl    $4, %esp               
    movl    %eax, -24(%ebp)        
    movl    -24(%ebp), %eax         #   8:     assign i <- t2
    movl    %eax, i                
l_fibonacci_6_while_cond:
    movl    i, %eax                 #  10:     if     i > 0 goto 5_while_body
    movl    $0, %ebx               
    cmpl    %ebx, %eax             
    jg      l_fibonacci_5_while_body
    jmp     l_fibonacci_4           #  11:     goto   4
l_fibonacci_5_while_body:
    leal    _str_8, %eax            #  13:     &()    t3 <- _str_8
    movl    %eax, -28(%ebp)        
    movl    -28(%ebp), %eax         #  14:     param  0 <- t3
    pushl   %eax                   
    call    WriteStr                #  15:     call   WriteStr
    addl    $4, %esp               
    movl    i, %eax                 #  16:     param  0 <- i
    pushl   %eax                   
    call    WriteInt                #  17:     call   WriteInt
    addl    $4, %esp               
    leal    _str_9, %eax            #  18:     &()    t4 <- _str_9
    movl    %eax, -32(%ebp)        
    movl    -32(%ebp), %eax         #  19:     param  0 <- t4
    pushl   %eax                   
    call    WriteStr                #  20:     call   WriteStr
    addl    $4, %esp               
    movl    i, %eax                 #  21:     param  0 <- i
    pushl   %eax                   
    call    fib                     #  22:     call   t5 <- fib
    addl    $4, %esp               
    movl    %eax, -36(%ebp)        
    movl    -36(%ebp), %eax         #  23:     param  0 <- t5
    pushl   %eax                   
    call    WriteInt                #  24:     call   WriteInt
    addl    $4, %esp               
    call    WriteLn                 #  25:     call   WriteLn
    leal    _str_10, %eax           #  26:     &()    t6 <- _str_10
    movl    %eax, -40(%ebp)        
    movl    -40(%ebp), %eax         #  27:     param  0 <- t6
    pushl   %eax                   
    call    ReadNumber              #  28:     call   t7 <- ReadNumber
    addl    $4, %esp               
    movl    %eax, -44(%ebp)        
    movl    -44(%ebp), %eax         #  29:     assign i <- t7
    movl    %eax, i                
    jmp     l_fibonacci_6_while_cond #  30:     goto   6_while_cond
l_fibonacci_4:

l_fibonacci_exit:
    # epilogue 
    addl    $32, %esp              
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

    # scope: fibonacci
_str_10:                            # <array 29 of <char>>
    .long    1
    .long   29
    .asciz "Enter a number (0 to exit): "
    .align   4
_str_6:                             # <array 18 of <char>>
    .long    1
    .long   18
    .asciz "Fibonacci numbers"
    .align   4
_str_7:                             # <array 29 of <char>>
    .long    1
    .long   29
    .asciz "Enter a number (0 to exit): "
    .align   4
_str_8:                             # <array 11 of <char>>
    .long    1
    .long   11
    .asciz "fibonacci("
    .align   4
_str_9:                             # <array 5 of <char>>
    .long    1
    .long    5
    .asciz ") = "
    .align   4
i:                                  # <int>
    .skip    4



    # end of global data section
    #-----------------------------------------

    .end
##################################################
