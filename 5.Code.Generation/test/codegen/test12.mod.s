##################################################
# test12
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

    # scope Int
Int:
    #    -16(%ebp)   4  [ $i        <int> %ebp-16 ]

    # stack offset 4
    # function prologue 4
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $4, %esp               

    movl    $75, %eax               #   0:     assign i <- 75
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   1:     return i
