section .data
    subs db "ram"
    start db "Please enter your first name...", 0, 10
    Pass_Okay db 0Dh , 0Ah ,"Gryffindor", 0, 10
    Pass_fail db 0Dh , 0Ah ,"Slytherin", 0, 10
    len1 : equ $-start
    len2 : equ $-Pass_Okay
    len3 : equ $-Pass_fail

section .bss
    buffer resb 10
    buffB resb 1

section .text
    global _start
    _start:   
        mov rcx, 0
        mov rsi, 0

        mov rax, 1  ;Print starting message
        mov rdi, 1
        mov rsi, start
        mov rdx, 31
        syscall

    readUntilEnter: ;Read input into buffer
        mov rax, 0
        mov rdi, 0
        mov rsi, buffer
        mov rdx, 10
        syscall

    PassOK:         ;Check length ok
    	mov rdx, 10
    	mov rsi, 0
    	mov rdi, 0
    	mov rcx, 3

    find:           ;Start searching for first match
    	MOV al, byte[buffer + rsi]
    	MOV ah, byte[subs + rdi]
    	cmp al, ah
    	je eql
    	jne not_eql

    not_eql:       ;If not equal reset search
    	inc rsi
    	dec rdx
        mov rdi, 0
        jz not_passed
    	jnz find

    eql:           ;If equal,  search for continuity
    	inc rsi
    	inc rdi

    	dec rcx
    	jz passed
        dec rdx
        jz not_passed

        MOV al, [buffer + esi]
    	MOV ah, [subs + edi]

    	cmp al, ah
    	je eql
    	mov rdi, 0
    	jne find

    passed:       ;Print substring exist message
        mov rax, 1
        mov rdi, 1
        mov rsi, Pass_Okay
        mov rdx, 12
        syscall
        jmp exit 

    not_passed:   ;Print substring not found message
        mov rax, 1
        mov rdi, 1
        mov rsi, Pass_fail
        mov rdx, 12
        syscall
        jmp exit

    exit:        ;Terminate
        mov rax, 60      ; return 0 status on exit - 'No Errors'
        mov rdx, 0      ; invoke SYS_EXIT (kernel opcode 1)
        syscall