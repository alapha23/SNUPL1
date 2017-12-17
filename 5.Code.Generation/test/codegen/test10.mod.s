##################################################
# test10
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

    # scope sum_rec
sum_rec:
    #      8(%ebp)   4  [ %n        <int> %ebp+8 ]
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
    cld                            
    xorl    %eax, %eax             
    movl    $3, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    movl    8(%ebp), %eax           #   0:     if     n > 0 goto 1_if_true
    movl    $0, %ebx               
    cmpl    %ebx, %eax             
