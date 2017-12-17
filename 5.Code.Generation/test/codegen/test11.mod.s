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
    #    -84(%ebp)  48  [ $g        <array 10 of <int>> %ebp-84 ]
    #    -85(%ebp)   1  [ $h        <bool> %ebp-85 ]
    #   -100(%ebp)  15  [ $i        <array 7 of <char>> %ebp-100 ]
    #   -104(%ebp)   4  [ $j        <int> %ebp-104 ]

    # stack offset 92
    # function prologue 92
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $92, %esp              
    cld                            
    xorl    %eax, %eax             
    movl    $23, %ecx              
    mov     %esp, %edi             
    rep     stosl                  
    movl    $1, -84(%ebp)           # local array 'g':1dimensions
    movl    $10, -80(%ebp)          #   dimension 1: 10 elements
    movl    $1, -100(%ebp)          # local array 'i':1dimensions
    movl    $7, -96(%ebp)           #   dimension 1: 7 elements


l_foo_exit:
    # epilogue 
    addl    $92, %esp              
    popl    %edi                   
    popl    %esi                   
    popl    %ebx                   
    popl    %ebp                   
    ret                            

    # scope test11
main:

    # stack offset 0
    # function prologue 0
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $0, %esp               
    cld                            
    xorl    %eax, %eax             
    movl    $0, %ecx               
    mov     %esp, %edi             
    rep     stosl                  


l_test11_exit:
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

    # scope: test11
A:                                  # <int>
    .skip    4
B:                                  # <bool>
    .skip    1
    .align   4
C:                                  # <int>
    .skip    4
D:                                  # <char>
    .skip    1
    .align   4
E:                                  # <int>
    .skip    4
F:                                  # <bool>
    .skip    1
    .align   4
G:                                  # <array 10 of <int>>
    .long    1
    .long   10
    .skip   40
H:                                  # <bool>
    .skip    1
    .align   4
I:                                  # <array 7 of <char>>
    .long    1
    .long    7
    .skip    7
    .align   4
J:                                  # <int>
    .skip    4


    # end of global data section
    #-----------------------------------------

    .end
##################################################
