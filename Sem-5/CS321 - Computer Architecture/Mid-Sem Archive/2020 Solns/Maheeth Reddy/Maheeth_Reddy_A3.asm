; Program to evaluate a quadratic expression x^2 + 3*x + 2
.model small
.stack 64
.data
    X DB 5      ; X is variable
    co1 DB 3    ; x-coefficient
    const DB 2  ; constant term
    poly DW ?   ; store end result

    prompt db 'Enter a number from (0-9): $'
    newline db ' ',13,10,'$'    ; used to print newline
    finish db 'For x = $'
    result db 'x.x + 3.x + 2 is $'
.code

main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; show prompt message
    ; ask user to enter a number
    mov ah,09h
    lea dx,prompt
    int 21h

    ; keyboard input interrupt
    mov ah, 1h
    ; read character into al
    int 21h
    sub al,'0'
    mov X,al ;copy number to cl

    ; print newline
    mov ah, 09h
    lea dx, newline
    int 21h

    ; end of prompt
    mov ah,09
    lea dx,finish
    int 21h

    ; character output interrupt
    mov dl,X
    add dl,'0'
    mov ah, 2h
    int 21h

    ; print newline
    mov ah, 09h
    lea dx, newline
    int 21h

    ; function to evaluate expression
    call eqn

    ; show result string
    mov ah,09
    lea dx,result
    int 21h

    ; print result on screen
    mov ax,poly
    call print

    ; return to DOS
    mov ah,4ch
    int 21h
main endp

eqn proc near
    ; calculate X^2
    mov al,X
    mul al
    mov bx,ax

    ; calculate 3*X
    mov al,X
    mul co1

    ; calculate X^2 + 3*X + 2
    add ax,bx
    mov dl,const
    mov dh,00
    add ax,dx

    ; store result and return
    mov poly,ax
    ret
eqn endp

; print function
print proc
    ; cx stores count of digits in number
    mov cx,0
    mov dx,0
cmd:
    cmp ax,0
    je printN
    mov bx,10 ; initialize bx to 10
    div bx
    ; extract the last digit
    push dx
    ; push it to stack
    inc cx
    ; increment the count
    mov dx,0
    jmp cmd
printN:
    ; check if count is greater than zero
    cmp cx,0
    je exit
    ; pop the top of stack
    pop dx
    ; convert to ASCII
    add dx,'0'
    ; print character interrupt
    mov ah,02h
    int 21h
    ; decrease the count
    dec cx
    jmp printN
exit: ret
print endp

     end main