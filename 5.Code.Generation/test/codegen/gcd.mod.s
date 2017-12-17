##################################################
# gcd
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

    # scope gcd_iter
gcd_iter:
    #      8(%ebp)   4  [ %a        <ptr(4) to <array 2 of <int>>> %ebp+8 ]
    #    -16(%ebp)   4  [ $t0       <int> %ebp-16 ]
    #    -20(%ebp)   4  [ $t1       <int> %ebp-20 ]
    #    -24(%ebp)   4  [ $t10      <int> %ebp-24 ]
    #    -28(%ebp)   4  [ $t11      <int> %ebp-28 ]
    #    -32(%ebp)   4  [ $t12      <int> %ebp-32 ]
    #    -36(%ebp)   4  [ $t13      <int> %ebp-36 ]
    #    -40(%ebp)   4  [ $t14      <int> %ebp-40 ]
    #    -44(%ebp)   4  [ $t15      <int> %ebp-44 ]
    #    -48(%ebp)   4  [ $t16      <int> %ebp-48 ]
    #    -52(%ebp)   4  [ $t17      <int> %ebp-52 ]
    #    -56(%ebp)   4  [ $t18      <int> %ebp-56 ]
    #    -60(%ebp)   4  [ $t19      <int> %ebp-60 ]
    #    -64(%ebp)   4  [ $t2       <int> %ebp-64 ]
    #    -68(%ebp)   4  [ $t20      <int> %ebp-68 ]
    #    -72(%ebp)   4  [ $t21      <int> %ebp-72 ]
    #    -76(%ebp)   4  [ $t22      <int> %ebp-76 ]
    #    -80(%ebp)   4  [ $t23      <int> %ebp-80 ]
    #    -84(%ebp)   4  [ $t24      <int> %ebp-84 ]
    #    -88(%ebp)   4  [ $t25      <int> %ebp-88 ]
    #    -92(%ebp)   4  [ $t26      <int> %ebp-92 ]
    #    -96(%ebp)   4  [ $t27      <int> %ebp-96 ]
    #   -100(%ebp)   4  [ $t28      <int> %ebp-100 ]
    #   -104(%ebp)   4  [ $t29      <int> %ebp-104 ]
    #   -108(%ebp)   4  [ $t3       <int> %ebp-108 ]
    #   -112(%ebp)   4  [ $t30      <int> %ebp-112 ]
    #   -116(%ebp)   4  [ $t31      <int> %ebp-116 ]
    #   -120(%ebp)   4  [ $t32      <int> %ebp-120 ]
    #   -124(%ebp)   4  [ $t33      <int> %ebp-124 ]
    #   -128(%ebp)   4  [ $t34      <int> %ebp-128 ]
    #   -132(%ebp)   4  [ $t35      <int> %ebp-132 ]
    #   -136(%ebp)   4  [ $t36      <int> %ebp-136 ]
    #   -140(%ebp)   4  [ $t37      <int> %ebp-140 ]
    #   -144(%ebp)   4  [ $t38      <int> %ebp-144 ]
    #   -148(%ebp)   4  [ $t39      <int> %ebp-148 ]
    #   -152(%ebp)   4  [ $t4       <int> %ebp-152 ]
    #   -156(%ebp)   4  [ $t40      <int> %ebp-156 ]
    #   -160(%ebp)   4  [ $t41      <int> %ebp-160 ]
    #   -164(%ebp)   4  [ $t42      <int> %ebp-164 ]
    #   -168(%ebp)   4  [ $t43      <int> %ebp-168 ]
    #   -172(%ebp)   4  [ $t44      <int> %ebp-172 ]
    #   -176(%ebp)   4  [ $t45      <int> %ebp-176 ]
    #   -180(%ebp)   4  [ $t5       <int> %ebp-180 ]
    #   -184(%ebp)   4  [ $t6       <int> %ebp-184 ]
    #   -188(%ebp)   4  [ $t7       <int> %ebp-188 ]
    #   -192(%ebp)   4  [ $t8       <int> %ebp-192 ]
    #   -196(%ebp)   4  [ $t9       <int> %ebp-196 ]

    # stack offset 184
    # function prologue 184
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $184, %esp             
    cld                            
    xorl    %eax, %eax             
    movl    $46, %ecx              
    mov     %esp, %edi             
    rep     stosl                  

