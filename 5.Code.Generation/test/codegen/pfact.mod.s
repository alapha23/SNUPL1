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
    #    -20(%ebp)   4  [ $t0       <ptr(4) to <array 2 of <char>>> %ebp-20 ]
    #    -24(%ebp)   4  [ $t1       <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t2       <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t3       <ptr(4) to <array 2 of <char>>> %ebp-32 ]
    #    -36(%ebp)   4  [ $t4       <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t5       <int> %ebp-40 ]

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

    movl    8(%ebp), %eax           #   0:     if     n < 1 goto 1_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
