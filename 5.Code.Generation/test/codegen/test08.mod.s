##################################################
# test08
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
    #    -60(%ebp)  48  [ $a        <array 10 of <int>> %ebp-60 ]
    #    -64(%ebp)   4  [ $i        <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t0       <ptr(4) to <array 10 of <int>>> %ebp-68 ]
    #    -72(%ebp)   4  [ $t1       <int> %ebp-72 ]
    #    -76(%ebp)   4  [ $t10      <int> %ebp-76 ]
    #    -80(%ebp)   4  [ $t11      <int> %ebp-80 ]
    #    -84(%ebp)   4  [ $t12      <int> %ebp-84 ]
    #    -88(%ebp)   4  [ $t13      <int> %ebp-88 ]
    #    -92(%ebp)   4  [ $t14      <ptr(4) to <array 10 of <int>>> %ebp-92 ]
    #    -96(%ebp)   4  [ $t15      <int> %ebp-96 ]
    #   -100(%ebp)   4  [ $t16      <ptr(4) to <array 10 of <int>>> %ebp-100 ]
    #   -104(%ebp)   4  [ $t17      <int> %ebp-104 ]
    #   -108(%ebp)   4  [ $t18      <int> %ebp-108 ]
    #   -112(%ebp)   4  [ $t19      <int> %ebp-112 ]
    #   -116(%ebp)   4  [ $t2       <ptr(4) to <array 10 of <int>>> %ebp-116 ]
    #   -120(%ebp)   4  [ $t20      <int> %ebp-120 ]
    #   -124(%ebp)   4  [ $t3       <int> %ebp-124 ]
    #   -128(%ebp)   4  [ $t4       <int> %ebp-128 ]
    #   -132(%ebp)   4  [ $t5       <int> %ebp-132 ]
    #   -136(%ebp)   4  [ $t6       <int> %ebp-136 ]
    #   -140(%ebp)   4  [ $t7       <ptr(4) to <array 10 of <int>>> %ebp-140 ]
    #   -144(%ebp)   4  [ $t8       <int> %ebp-144 ]
    #   -148(%ebp)   4  [ $t9       <ptr(4) to <array 10 of <int>>> %ebp-148 ]

    # stack offset 136
    # function prologue 136
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $136, %esp             
    movl    $1, -60(%ebp)           # local array 'a':1dimensions
    movl    $10, -56(%ebp)          #   dimension 1: 10 elements

    leal    -60(%ebp), %eax         #   0:     &()    t0 <- a
    movl    %eax, -68(%ebp)        
    movl    $0, %eax                #   1:     mul    t1 <- 0, 4
    movl    $4, %ebx               
    imull   %ebx                   
    movl    %eax, -72(%ebp)        
    leal    -60(%ebp), %eax         #   2:     &()    t2 <- a
    movl    %eax, -116(%ebp)       
    movl    -116(%ebp), %eax        #   3:     param  1 <- t2
    pushl   %eax                   
    call    DOFS                    #   4:     call   t3 <- DOFS
    addl    $4, %esp               
    movl    %eax, -124(%ebp)       
    movl    -72(%ebp), %eax         #   5:     add    t4 <- t1, t3
    movl    -124(%ebp), %ebx       
    addl    %ebx, %eax             
    movl    %eax, -128(%ebp)       
    movl    -68(%ebp), %eax         #   6:     add    t5 <- t0, t4
    movl    -128(%ebp), %ebx       
    addl    %ebx, %eax             
    movl    %eax, -132(%ebp)       
    movl    $1, %eax                #   7:     assign @t5 <- 1
    movl    -132(%ebp), %edi       
    movl    %eax, (%edi)           
    movl    $1, %eax                #   8:     assign i <- 1
    movl    %eax, -64(%ebp)        
