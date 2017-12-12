##################################################
# test11
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

    # scope foo
foo:
    #    -16(%ebp)   4  [ $a        <int> %ebp-16 ]
    #    -17(%ebp)   1  [ $b        <bool> %ebp-17 ]
    #    -24(%ebp)   4  [ $c        <int> %ebp-24 ]
    #    -25(%ebp)   1  [ $d        <char> %ebp-25 ]
    #    -32(%ebp)   4  [ $e        <int> %ebp-32 ]
    #    -33(%ebp)   1  [ $f        <bool> %ebp-33 ]
    #    -88(%ebp)  48  [ $g        <array 10 of <int>> %ebp-88 ]
    #    -89(%ebp)   1  [ $h        <bool> %ebp-89 ]
    #   -104(%ebp)  15  [ $i        <array 7 of <char>> %ebp-104 ]
    #   -108(%ebp)   4  [ $j        <int> %ebp-108 ]

    # stack offset 96
    # function prologue 96
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $96, %esp              
    movl    $1, -88(%ebp)           # local array 'g':1dimensions
    movl    $10, -84(%ebp)          #   dimension 1: 10 elements
    movl    $1, -104(%ebp)          # local array 'i':1dimensions
    movl    $7, -100(%ebp)          #   dimension 1: 7 elements


