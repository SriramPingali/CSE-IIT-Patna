; AGE PREDICTOR, 64-bit assembly language program (x86_64)
; to compile and execute, use: nasm -f elf64  lab6.asm && gcc -g -no-pie lab6.o && ./a.out

; C functions used in this program
    extern printf
    extern scanf

; macro for printing string to stdout
%macro PutStr 1
    push rax

    mov rdi, %%format
    mov rsi, %1
    mov rax,0
    call printf
    jmp %%done

    %%format:   db "%s",0   ; format specifier
    %%done:     pop rax
%endmacro

; macro for printing integer to stdout
%macro PutInt 1
    mov rdi,%%format
    push rax
    mov rax,0
    mov ax, word %1
    mov rsi,rax
    mov rax,0
    call printf
    jmp %%done

    %%format:   db "%d",0   ; format specifier
    %%done:     pop rax
%endmacro

; macro for scanning integer from stdin
%macro GetInt 1
    push rax
    mov rdi, %%format
    xor rdx,rdx
    mov rsi, %1
    mov rax,0
    call scanf
    jmp %%done

    %%format: db "%d",0   ; format specifier
    %%done:   pop rax
%endmacro

section .data
    ;Welcome message
    welcome:        db "                 *************************",13,10,
                    db "Hey, I am the... *     AGE PREDICTOR     *",13,10,
                    db "                 *************************",13,10,0

    ;Instructions
    msg_1:          db "I can predict your age with hints from you…",13,10,0
    msg_2:          db "Just answer whether you are",13,10,
                    db "younger or older than my prediction …",13,10,
                    db "That’s it….",0
    
    ;Query message
    query_msg_p1:   db "Are you ",0
    query_msg_p2:   db " years old ?",13,10,0

    ;Message to display options for user
    menu_msg:       db "[0] Yes!",13,10,
                    db "[1] Younger than that",13,10,
                    db "[2] Older than that",13,10,0
    
    ; Prompt user for a choice
    prompt:         db ">> ",0

    ; if user enters any number other than 0 or 1 or 2, display this message
    invalid_msg:    db "Invalid option. Try Again.",13,10,0

    ; If user gives misleading information
    mislead_msg:    db "Hey, you are misleading me!",13,10,0

    ;Exit messages
    exit_msg_p1:    db "Got it... you are ",0
    exit_msg_p2:    db " years old!",13,10,0

    ;Minimum and Maximum age of a person, initially
    ILOW:           equ 1
    IHIGH:          equ 100

    newline:        db 13,10,0

    ;Check whether a particular age is already asked or not
    asked:          times IHIGH db 0

section .bss
    ; get user choice
    chr:            resw 1

section .text
    global main
main:
    ;Display introductory messages
    PutStr newline
    PutStr welcome
    PutStr newline
    PutStr msg_1
    PutStr newline
    PutStr msg_2
    PutStr newline
    
    ; Perform a user controlled 
    ; binary search to find user's age
    mov ebx,0
    ; AL stores minimum possible value of user's age
    ; AH stores maximum possible value of user's age
    mov al,ILOW
    mov ah,IHIGH
guess_bin_srch:
    ; Calculate average of AH and AL, store in BL
    ; this is the prediction of user's age
    mov bl,0
    add bl,al
    add bl,ah
    shr bl,1
    
    ; Check whether age in BL is already asked or not
    cmp byte [asked+ebx-1],0
    ; If asked, exit the program
    jne mislead

    ; Ask user if the prediction is right or wrong
    PutStr newline
    PutStr query_msg_p1
    PutInt bx
    PutStr query_msg_p2
    PutStr menu_msg
option:
    PutStr prompt    
    GetInt chr
    mov dx, word [chr]

    ; Note that this predicted age has been asked
    inc byte [asked+ebx-1]

    ; If input is 0, then user's age is matching
    cmp dx,0
    je exit

    ; If input is 1, then user's age is lower
    cmp dx,1
    je lower

    ; If input is 2, then user's age is higher
    cmp dx,2
    je higher

    ; If any other option is given, re-prompt the user
    PutStr invalid_msg
    jmp option

lower:
    ; Since age is lower than prediction,
    ; Update AH correspondingly and re-prompt
    mov ah,bl
    jmp guess_bin_srch

higher:
    ; Since age is higher than prediction,
    ; Update AL correspondingly and re-prompt
    mov al,bl
    jmp guess_bin_srch

exit:
    ; If predicted age is right,
    ; exit program after showing
    ; an appropriate message
    PutStr newline
    PutStr exit_msg_p1
    PutInt bx
    PutStr exit_msg_p2
    PutStr newline
    jmp quit

mislead:
    ; If inputs are misleading,
    ; exit program after showing
    ; an appropriate message
    PutStr newline
    PutStr mislead_msg
    PutStr newline
    jmp quit

quit:
    mov    rax, 60
    mov    rdi, 0
    syscall