##################################################
# primes
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

    # scope CalcPrimes
CalcPrimes:
    #    -16(%ebp)   4  [ $N        <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $i        <int> %ebp-20 ]
    #    -21(%ebp)   1  [ $isprime  <bool> %ebp-21 ]
    #     12(%ebp)   4  [ %n        <int> %ebp+12 ]
    #      8(%ebp)   4  [ %p        <ptr(4) to <array of <int>>> %ebp+8 ]
    #    -28(%ebp)   4  [ $pidx     <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $psqrt    <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t0       <ptr(4) to <array 30 of <char>>> %ebp-36 ]
    #    -40(%ebp)   4  [ $t1       <ptr(4) to <array 4 of <char>>> %ebp-40 ]
    #    -44(%ebp)   4  [ $t10      <int> %ebp-44 ]
    #    -48(%ebp)   4  [ $t11      <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t12      <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t13      <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t14      <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t15      <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t16      <int> %ebp-68 ]
    #    -72(%ebp)   4  [ $t17      <int> %ebp-72 ]
    #    -76(%ebp)   4  [ $t18      <int> %ebp-76 ]
    #    -80(%ebp)   4  [ $t19      <int> %ebp-80 ]
    #    -84(%ebp)   4  [ $t2       <int> %ebp-84 ]
    #    -88(%ebp)   4  [ $t20      <int> %ebp-88 ]
    #    -92(%ebp)   4  [ $t21      <int> %ebp-92 ]
    #    -96(%ebp)   4  [ $t22      <int> %ebp-96 ]
    #   -100(%ebp)   4  [ $t23      <int> %ebp-100 ]
    #   -104(%ebp)   4  [ $t24      <int> %ebp-104 ]
    #   -108(%ebp)   4  [ $t25      <int> %ebp-108 ]
    #   -112(%ebp)   4  [ $t26      <int> %ebp-112 ]
    #   -116(%ebp)   4  [ $t27      <ptr(4) to <array 45 of <char>>> %ebp-116 ]
    #   -120(%ebp)   4  [ $t28      <int> %ebp-120 ]
    #   -124(%ebp)   4  [ $t29      <int> %ebp-124 ]
    #   -128(%ebp)   4  [ $t3       <int> %ebp-128 ]
    #   -132(%ebp)   4  [ $t30      <int> %ebp-132 ]
    #   -136(%ebp)   4  [ $t31      <int> %ebp-136 ]
    #   -140(%ebp)   4  [ $t32      <int> %ebp-140 ]
    #   -144(%ebp)   4  [ $t33      <int> %ebp-144 ]
    #   -148(%ebp)   4  [ $t34      <int> %ebp-148 ]
    #   -152(%ebp)   4  [ $t35      <int> %ebp-152 ]
    #   -156(%ebp)   4  [ $t36      <int> %ebp-156 ]
    #   -160(%ebp)   4  [ $t37      <int> %ebp-160 ]
    #   -164(%ebp)   4  [ $t38      <int> %ebp-164 ]
    #   -168(%ebp)   4  [ $t39      <ptr(4) to <array 7 of <char>>> %ebp-168 ]
    #   -172(%ebp)   4  [ $t4       <int> %ebp-172 ]
    #   -176(%ebp)   4  [ $t40      <ptr(4) to <array 7 of <char>>> %ebp-176 ]
    #   -180(%ebp)   4  [ $t41      <ptr(4) to <array 15 of <char>>> %ebp-180 ]
    #   -184(%ebp)   4  [ $t5       <int> %ebp-184 ]
    #   -188(%ebp)   4  [ $t6       <int> %ebp-188 ]
    #   -192(%ebp)   4  [ $t7       <int> %ebp-192 ]
    #   -196(%ebp)   4  [ $t8       <int> %ebp-196 ]
    #   -200(%ebp)   4  [ $t9       <int> %ebp-200 ]
    #   -204(%ebp)   4  [ $v        <int> %ebp-204 ]

    # stack offset 192
    # function prologue 192
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $192, %esp             
    cld                            
    xorl    %eax, %eax             
    movl    $48, %ecx              
    mov     %esp, %edi             
    rep     stosl                  

    leal    _str_1, %eax            #   0:     &()    t0 <- _str_1
    movl    %eax, -36(%ebp)        
    movl    -36(%ebp), %eax         #   1:     param  0 <- t0
    pushl   %eax                   
    call    WriteStr                #   2:     call   WriteStr
    addl    $4, %esp               
    movl    12(%ebp), %eax          #   3:     param  0 <- n
    pushl   %eax                   
    call    WriteInt                #   4:     call   WriteInt
    addl    $4, %esp               
    leal    _str_2, %eax            #   5:     &()    t1 <- _str_2
    movl    %eax, -40(%ebp)        
    movl    -40(%ebp), %eax         #   6:     param  0 <- t1
    pushl   %eax                   
    call    WriteStr                #   7:     call   WriteStr
    addl    $4, %esp               
    movl    $1, %eax                #   8:     param  1 <- 1
    pushl   %eax                   
    movl    8(%ebp), %eax           #   9:     param  0 <- p
    pushl   %eax                   
    call    DIM                     #  10:     call   t2 <- DIM
    addl    $8, %esp               
    movl    %eax, -84(%ebp)        
    movl    -84(%ebp), %eax         #  11:     assign N <- t2
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #  12:     if     N < 1 goto 5_if_true
    movl    $1, %ebx               
    cmpl    %ebx, %eax             
