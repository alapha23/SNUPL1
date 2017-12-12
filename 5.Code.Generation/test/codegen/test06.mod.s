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
    #      8(%ebp)  18  [ %a        <array 10 of <bool>> %ebp+8 ]
    #    -16(%ebp)   4  [ $i        <int> %ebp-16 ]
    #    -17(%ebp)   1  [ $t0       <bool> %ebp-17 ]
    #    -24(%ebp)   4  [ $t1       <ptr(4) to <array 10 of <bool>>> %ebp-24 ]
    #    -28(%ebp)   4  [ $t10      <ptr(4) to <array 10 of <bool>>> %ebp-28 ]
    #    -32(%ebp)   4  [ $t11      <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t12      <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t13      <int> %ebp-40 ]
    #    -44(%ebp)   4  [ $t14      <int> %ebp-44 ]
    #    -48(%ebp)   4  [ $t2       <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t3       <ptr(4) to <array 10 of <bool>>> %ebp-52 ]
    #    -56(%ebp)   4  [ $t4       <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t5       <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t6       <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t7       <int> %ebp-68 ]
    #    -72(%ebp)   4  [ $t8       <ptr(4) to <array 10 of <bool>>> %ebp-72 ]
    #    -76(%ebp)   4  [ $t9       <int> %ebp-76 ]

    # stack offset 64
    # function prologue 64
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $64, %esp              
    movl    $1, 8(%ebp)             # local array 'a':1dimensions
    movl    $10, 12(%ebp)           #   dimension 1: 10 elements

    movl    $0, %eax                #   0:     assign i <- 0
    movl    %eax, -16(%ebp)        
