%include "io.mac"

segment .data
    ENTER_KEY equ 0Dh
    ESC_KEY equ 1Bh ; exit
    U_KEY equ 48h
    D_KEY equ 50h
    L_KEY equ 4Bh
    R_KEY equ 4Dh

    CHARS dw 010h
    color_red db 0CCh
    color_green db 0AAh
    space db 020h 

    row db 0
    col db 0

    subs db "ram"
    start db "Please enter your first name...", 0
    Pass_Okay db 0Dh , 0Ah ,"Gryffindor", 10, 0
    Pass_fail db 0Dh , 0Ah ,"Slytherin", 10, 0

segment .bss
    buffer resb 10
    buffB resb 1

segment .text
    _main:   
        mov bx, ENTER_KEY
        mov ecx, 0
        mov esi, 0

        PutStr start
        nwln

    readUntilEnter:
        mov eax, 3
        mov ebx, 0
        mov ecx, buffer
        mov edx, 10
        int 0x80

    PassOK:
    	mov edx, 10
    	mov esi, 0
    	mov edi, 0
    	mov ecx, 3

    find:
    	MOV al, byte[buffer + esi]
    	MOV ah, byte[subs + edi]
    	cmp al, ah
    	je eql
    	jne not_eql

    not_eql:
    	inc esi
    	dec edx
        mov edi, 0
        jz not_passed
    	jnz find

    eql:
    	inc esi
    	inc edi

    	dec ecx
    	jz passed
        dec edx
        jz not_passed

        MOV al, [buffer + esi]
    	MOV ah, [subs + edi]

    	cmp al, ah
    	je eql
    	mov edi, 0
    	jne find

    passed:
    	PutStr Pass_Okay 
        jmp exit 

    not_passed:
    	PutStr Pass_fail
        jmp exit

     exit:  
        mov     ebx, 0      ; return 0 status on exit - 'No Errors'
        mov     eax, 1      ; invoke SYS_EXIT (kernel opcode 1)
        int     80h