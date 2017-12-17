##################################################
# test07
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

    # scope test07
main:
    #    -16(%ebp)   4  [ $t0       <ptr(4) to <array 14 of <char>>> %ebp-16 ]

    # stack offset 4
    # function prologue 4
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $4, %esp               
    cld                            
    xorl    %eax, %eax             
    movl    $1, %ecx               
    mov     %esp, %edi             
    rep     stosl                  

    leal    _str_1, %eax            #   0:     &()    t0 <- _str_1
    movl    %eax, -16(%ebp)        
    movl    -16(%ebp), %eax         #   1:     param  0 <- t0
    pushl   %eax                   
    call    WriteStr                #   2:     call   WriteStr
    addl    $4, %esp               
    call    WriteLn                 #   3:     call   WriteLn

l_test07_exit:
    # epilogue 
    addl    $4, %esp               
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

    # scope: test07
_str_1:                             # <array 14 of <char>>
    .long    1
    .long   14
    .asciz "Hello, world!"

    # end of global data section
    #-----------------------------------------

    .end
##################################################
