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

    # stack offset 0
    # function prologue 0
    pushl   %ebp                   
    movl    %esp, %ebp             
    pushl   %ebx                   
    pushl   %esi                   
    pushl   %edi                   
    subl    $0, %esp               

    movl    _str_1, %eax            #   0:     param  0 <- _str_1
    pushl   %eax                   
    call    WriteStr                #   1:     call   WriteStr
    addl    $4, %esp               
    call    WriteLn                 #   2:     call   WriteLn

l_test07_exit:
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

    # scope: test07
_str_1:                             # <array 14 of <char>>
    .long    1
    .long   14
    .asciz "Hello, world!"

    # end of global data section
    #-----------------------------------------

    .end
##################################################
