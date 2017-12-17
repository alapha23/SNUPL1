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
    #    -37(%ebp)   1  [ $t0       <bool> %ebp-37 ]
    #    -44(%ebp)   4  [ $t1       <ptr(4) to <array 10 of <bool>>> %ebp-44 ]
    #    -48(%ebp)   4  [ $t10      <ptr(4) to <array 10 of <bool>>> %ebp-48 ]
    #    -52(%ebp)   4  [ $t11      <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t12      <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t13      <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t14      <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t2       <int> %ebp-68 ]
    #    -72(%ebp)   4  [ $t3       <ptr(4) to <array 10 of <bool>>> %ebp-72 ]
    #    -76(%ebp)   4  [ $t4       <int> %ebp-76 ]
    #    -80(%ebp)   4  [ $t5       <int> %ebp-80 ]
    #    -84(%ebp)   4  [ $t6       <int> %ebp-84 ]
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
