##################################################
# test01
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

    # scope test01
main:
    #      0(4  [ $t0       <int> ]
    #      0(4  [ $t1       <int> ]
    #      0(4  [ $t2       <int> ]
    #      0(4  [ $t3       <int> ]
    # stack offset 0
    # function prologue 0
    pushl   %ebp                   
    movl    %esp %ebp              
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $0, %esp               
    movl    $2, %eax                #   0:     assign a <- 2
    movl    %eax, a                
    movl    $5, %eax                #   1:     assign b <- 5
    movl    %eax, b                
    movl    a, %eax                 #   2:     add    t0 <- a, b
    movl    b, %ebx                
    addl    %ebx, %eax             
    movl    %eax, 0()              
    movl    0(), %eax               #   3:     assign c <- t0
    movl    %eax, c                
    movl    c, %eax                 #   4:     param  0 <- c
    pushl   %eax                   
    call    WriteInt                #   5:     call   WriteInt
    addl    $4, %esp               
    movl    a, %eax                 #   6:     mul    t1 <- a, b
    movl    b, %ebx                
    imull   %ebx                   
    movl    %eax, 0()              
    movl    0(), %eax               #   7:     assign c <- t1
    movl    %eax, c                
    movl    c, %eax                 #   8:     param  0 <- c
    pushl   %eax                   
    call    WriteInt                #   9:     call   WriteInt
    addl    $4, %esp               
    movl    b, %eax                 #  10:     neg    t2 <- b
    negl    %eax                   
    movl    %eax, 0()              
    movl    0(), %eax               #  11:     add    t3 <- t2, a
    movl    a, %ebx                
    addl    %ebx, %eax             
    movl    %eax, 0()              
    movl    0(), %eax               #  12:     assign c <- t3
    movl    %eax, c                
    movl    c, %eax                 #  13:     param  0 <- c
    pushl   %eax                   
    call    WriteInt                #  14:     call   WriteInt
    addl    $4, %esp               

l_test01_exit:
    # epilogue 
    addl    $0, %esp               
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # end of text section
    #-----------------------------------------

    #-----------------------------------------
    # global data section
    #
    .data
    .align 4

    # scope: test01
a:                                  # <int>
    .skip    4
b:                                  # <int>
    .skip    4
c:                                  # <int>
    .skip    4

    # end of global data section
    #-----------------------------------------

    .end
##################################################
