##################################################
# test05
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
    #      8(%ebp)   4  [ %a        <ptr(4) to <array 10 of <int>>> %ebp+8 ]
    #    -16(%ebp)   4  [ $i        <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t10      <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t2       <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t3       <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t4       <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t5       <int> %ebp-40 ]
    #    -44(%ebp)   4  [ $t6       <int> %ebp-44 ]
    #    -48(%ebp)   4  [ $t7       <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t8       <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t9       <int> %ebp-56 ]

    # stack offset 44
    # function prologue 44
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $44, %esp              

    movl    $0, %eax                #   0:     mul    t1 <- 0, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -20(%ebp)        
    movl    8(%ebp), %eax           #   1:     param  0 <- a
    pushl   %eax                   
    call    DOFS                    #   2:     call   t2 <- DOFS
    addl    $4, %esp               
    movl    %eax, -28(%ebp)        
    movl    -20(%ebp), %eax         #   3:     add    t3 <- t1, t2
    movl    -28(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -32(%ebp)        
    movl    8(%ebp), %eax           #   4:     add    t4 <- a, t3
    movl    -32(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -36(%ebp)        
    movl    $1, %eax                #   5:     assign @t4 <- 1
    movl    -36(%ebp), %edi        
    movl    %eax, (%edi)           
    movl    $1, %eax                #   6:     assign i <- 1
    movl    %eax, -16(%ebp)        
