##################################################
# pfact
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

    # scope primefactor
primefactor:
    #    -16(%ebp)   4  [ $f        <int> %ebp-16 ]
    #      8(%ebp)   4  [ %n        <int> %ebp+8 ]
    #    -20(%ebp)   4  [ $t10      <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t5       <ptr(4) to <array 2 of <char>>> %ebp-24 ]
    #    -28(%ebp)   4  [ $t6       <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t7       <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t8       <ptr(4) to <array 2 of <char>>> %ebp-36 ]
    #    -40(%ebp)   4  [ $t9       <int> %ebp-40 ]

    # stack offset 28
    # function prologue 28
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $28, %esp              
    cld                            
    xorl    %eax, %eax             
    movl    $7, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    movl    8(%ebp), %eax           #   0:     if     n < 1 goto 2_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    jl      l_primefactor_2_if_true
    jmp     l_primefactor_1_if_false #   1:     goto   1_if_false
l_primefactor_2_if_true:
    jmp     l_primefactor_exit      #   3:     return 
    jmp     l_primefactor_0         #   4:     goto   0
l_primefactor_1_if_false:
    movl    8(%ebp), %eax           #   6:     if     n = 1 goto 7_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
    je      l_primefactor_7_if_true
    jmp     l_primefactor_6_if_false #   7:     goto   6_if_false
l_primefactor_7_if_true:
    leal    _str_17, %eax           #   9:     &()    t5 <- _str_17
    movl    %eax, -24(%ebp)        
    movl    -24(%ebp), %eax         #  10:     param  0 <- t5
    pushl   %eax                   
    call    WriteStr                #  11:     call   WriteStr
    addl    $4, %esp               
    movl    $1, %eax                #  12:     param  0 <- 1
    pushl   %eax                   
    call    WriteInt                #  13:     call   WriteInt
    addl    $4, %esp               
    jmp     l_primefactor_5         #  14:     goto   5
l_primefactor_6_if_false:
    movl    $2, %eax                #  16:     assign f <- 2
    movl    %eax, -16(%ebp)        
l_primefactor_14_while_cond:
    movl    -16(%ebp), %eax         #  18:     if     f <= n goto 13_while_body
    movl    8(%ebp), %ebx          
    cmpl    %ebx, %eax             
    jle     l_primefactor_13_while_body
    jmp     l_primefactor_12        #  19:     goto   12
l_primefactor_13_while_body:
    movl    8(%ebp), %eax           #  21:     div    t6 <- n, f
    movl    -16(%ebp), %ebx        
    cdq                            
    idivl   %ebx                   
    movl    %eax, -28(%ebp)        
    movl    -28(%ebp), %eax         #  22:     mul    t7 <- t6, f
    movl    -16(%ebp), %ebx        
    imull   %ebx                   
    movl    %eax, -32(%ebp)        
    movl    -32(%ebp), %eax         #  23:     if     t7 = n goto 18_if_true
    movl    8(%ebp), %ebx          
    cmpl    %ebx, %eax             
    je      l_primefactor_18_if_true
    jmp     l_primefactor_17_if_false #  24:     goto   17_if_false
l_primefactor_18_if_true:
    leal    _str_18, %eax           #  26:     &()    t8 <- _str_18
    movl    %eax, -36(%ebp)        
    movl    -36(%ebp), %eax         #  27:     param  0 <- t8
    pushl   %eax                   
    call    WriteStr                #  28:     call   WriteStr
    addl    $4, %esp               
    movl    -16(%ebp), %eax         #  29:     param  0 <- f
    pushl   %eax                   
    call    WriteInt                #  30:     call   WriteInt
    addl    $4, %esp               
    movl    8(%ebp), %eax           #  31:     div    t9 <- n, f
    movl    -16(%ebp), %ebx        
    cdq                            
    idivl   %ebx                   
    movl    %eax, -40(%ebp)        
    movl    -40(%ebp), %eax         #  32:     assign n <- t9
    movl    %eax, 8(%ebp)          
    jmp     l_primefactor_16        #  33:     goto   16
l_primefactor_17_if_false:
    movl    -16(%ebp), %eax         #  35:     add    t10 <- f, 1
    movl    $1, %ebx               
    addl    %ebx, %eax             
    movl    %eax, -20(%ebp)        
    movl    -20(%ebp), %eax         #  36:     assign f <- t10
    movl    %eax, -16(%ebp)        
l_primefactor_16:
    jmp     l_primefactor_14_while_cond #  38:     goto   14_while_cond
l_primefactor_12:
l_primefactor_5:
l_primefactor_0:

l_primefactor_exit:
    # epilogue 
    addl    $28, %esp              
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # scope pfact
main:
    #    -16(%ebp)   4  [ $t0       <ptr(4) to <array 16 of <char>>> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <ptr(4) to <array 25 of <char>>> %ebp-20 ]
    #    -24(%ebp)   4  [ $t2       <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t3       <ptr(4) to <array 20 of <char>>> %ebp-28 ]
    #    -32(%ebp)   4  [ $t4       <ptr(4) to <array 3 of <char>>> %ebp-32 ]

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

    leal    _str_19, %eax           #   0:     &()    t0 <- _str_19
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   1:     param  0 <- t0
    pushl   %eax                   
    call    WriteStr                #   2:     call   WriteStr
    addl    $4, %esp               
    call    WriteLn                 #   3:     call   WriteLn
    call    WriteLn                 #   4:     call   WriteLn
    leal    _str_20, %eax           #   5:     &()    t1 <- _str_20
    movl    %eax, -20(%ebp)        
    movl    -20(%ebp), %eax         #   6:     param  0 <- t1
    pushl   %eax                   
    call    WriteStr                #   7:     call   WriteStr
    addl    $4, %esp               
    call    ReadInt                 #   8:     call   t2 <- ReadInt
    movl    %eax, -24(%ebp)        
    movl    -24(%ebp), %eax         #   9:     assign n <- t2
    movl    %eax, n                
    leal    _str_21, %eax           #  10:     &()    t3 <- _str_21
    movl    %eax, -28(%ebp)        
    movl    -28(%ebp), %eax         #  11:     param  0 <- t3
    pushl   %eax                   
    call    WriteStr                #  12:     call   WriteStr
    addl    $4, %esp               
    movl    n, %eax                 #  13:     param  0 <- n
    pushl   %eax                   
    call    WriteInt                #  14:     call   WriteInt
    addl    $4, %esp               
    leal    _str_22, %eax           #  15:     &()    t4 <- _str_22
    movl    %eax, -32(%ebp)        
    movl    -32(%ebp), %eax         #  16:     param  0 <- t4
    pushl   %eax                   
    call    WriteStr                #  17:     call   WriteStr
    addl    $4, %esp               
    movl    n, %eax                 #  18:     param  0 <- n
    pushl   %eax                   
    call    primefactor             #  19:     call   primefactor
    addl    $4, %esp               
    call    WriteLn                 #  20:     call   WriteLn

l_pfact_exit:
    # epilogue 
    addl    $20, %esp              
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

    # scope: pfact
_str_17:                            # <array 2 of <char>>
    .long    1
    .long    2
    .asciz " "
    .align   4
_str_18:                            # <array 2 of <char>>
    .long    1
    .long    2
    .asciz " "
    .align   4
_str_19:                            # <array 16 of <char>>
    .long    1
    .long   16
    .asciz "Prime factoring"
_str_20:                            # <array 25 of <char>>
    .long    1
    .long   25
    .asciz "Enter number to factor: "
    .align   4
_str_21:                            # <array 20 of <char>>
    .long    1
    .long   20
    .asciz "  prime factors of "
_str_22:                            # <array 3 of <char>>
    .long    1
    .long    3
    .asciz ": "
    .align   4
n:                                  # <int>
    .skip    4


    # end of global data section
    #-----------------------------------------

    .end
##################################################
