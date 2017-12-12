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
    #    -36(%ebp)  18  [ $a        <array 10 of <bool>> %ebp-36 ]
    #    -40(%ebp)   4  [ $i        <int> %ebp-40 ]
    #    -41(%ebp)   1  [ $t0       <bool> %ebp-41 ]
    #    -48(%ebp)   4  [ $t1       <ptr(4) to <array 10 of <bool>>> %ebp-48 ]
    #    -52(%ebp)   4  [ $t10      <ptr(4) to <array 10 of <bool>>> %ebp-52 ]
    #    -56(%ebp)   4  [ $t11      <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t12      <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t13      <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t14      <int> %ebp-68 ]
    #    -72(%ebp)   4  [ $t2       <int> %ebp-72 ]
    #    -76(%ebp)   4  [ $t3       <ptr(4) to <array 10 of <bool>>> %ebp-76 ]
    #    -80(%ebp)   4  [ $t4       <int> %ebp-80 ]
    #    -84(%ebp)   4  [ $t5       <int> %ebp-84 ]
    #    -88(%ebp)   4  [ $t6       <int> %ebp-88 ]
    #    -92(%ebp)   4  [ $t7       <int> %ebp-92 ]
    #    -96(%ebp)   4  [ $t8       <ptr(4) to <array 10 of <bool>>> %ebp-96 ]
    #   -100(%ebp)   4  [ $t9       <int> %ebp-100 ]

    # stack offset 88
    # function prologue 88
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $88, %esp              
    movl    $1, -36(%ebp)           # local array 'a':1dimensions
    movl    $10, -32(%ebp)          #   dimension 1: 10 elements

    movl    $0, %eax                #   0:     assign i <- 0
    movl    %eax, -40(%ebp)        
