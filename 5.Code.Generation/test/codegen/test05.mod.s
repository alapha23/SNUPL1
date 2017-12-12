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
    #      8(%ebp)  48  [ %a        <array 10 of <int>> %ebp+8 ]
    #    -16(%ebp)   4  [ $i        <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $t0       <ptr(4) to <array 10 of <int>>> %ebp-20 ]
    #    -24(%ebp)   4  [ $t1       <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t10      <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t11      <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t12      <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t13      <int> %ebp-40 ]
    #    -44(%ebp)   4  [ $t2       <ptr(4) to <array 10 of <int>>> %ebp-44 ]
    #    -48(%ebp)   4  [ $t3       <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t4       <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t5       <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t6       <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t7       <ptr(4) to <array 10 of <int>>> %ebp-64 ]
    #    -68(%ebp)   4  [ $t8       <int> %ebp-68 ]
    #    -72(%ebp)   4  [ $t9       <ptr(4) to <array 10 of <int>>> %ebp-72 ]

    # stack offset 60
    # function prologue 60
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $60, %esp              
    movl    $1, 8(%ebp)             # local array 'a':1dimensions
    movl    $10, 12(%ebp)           #   dimension 1: 10 elements

    leal    8(%ebp), %eax           #   0:     &()    t0 <- a
    movl    %eax, -20(%ebp)        
    movl    $0, %eax                #   1:     mul    t1 <- 0, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -24(%ebp)        
    leal    8(%ebp), %eax           #   2:     &()    t2 <- a
    movl    %eax, -44(%ebp)        
    movl    -44(%ebp), %eax         #   3:     param  1 <- t2
    pushl   %eax                   
    call    DOFS                    #   4:     call   t3 <- DOFS
    addl    $4, %esp               
    movl    %eax, -48(%ebp)        
    movl    -24(%ebp), %eax         #   5:     add    t4 <- t1, t3
    movl    -48(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -52(%ebp)        
    movl    -20(%ebp), %eax         #   6:     add    t5 <- t0, t4
    movl    -52(%ebp), %ebx        
    addl    %ebx, %eax             
    movl    %eax, -56(%ebp)        
    movl    $1, %eax                #   7:     assign @t5 <- 1
    movl    -56(%ebp), %edi        
    movl    %eax, (%edi)           
    movl    $1, %eax                #   8:     assign i <- 1
    movl    %eax, -16(%ebp)        
