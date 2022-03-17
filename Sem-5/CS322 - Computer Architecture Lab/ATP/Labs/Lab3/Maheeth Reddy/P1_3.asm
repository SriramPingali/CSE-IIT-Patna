; program to find largest and smallest among an array of numbers and print the difference between them
.model small
.stack 256
.data
    ; numbers list
    num   db 07,04,02,07,15,09,06,03,08,04
    large db 00     ; largest number
    small db 00     ; smallest number
    diff  db 00     ; difference
.code
main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; find smalles number
    mov ax,0000h
    lea si,num
    mov cl,0ah
    mov al,[si]

    dec cl
    inc si
    repeat: cmp al,[si]
            jc cmd
            mov al,[si]
            
            cmd: inc si
            loop repeat

    ; store smallest number    
    mov small,al

    ; find largest number
    lea si,num
    mov cl,0ah
    mov al,[si]

    dec cl
    inc si
    repeat2: cmp al,[si]
            jnc cmd2
                mov al,[si]
            
            cmd2: inc si
            loop repeat2
    
    ; find difference
    sub al,small
    mov diff,al

    ; store difference
    xor ax,ax
    mov al,diff
    call print

    ; return to dos
    mov ah,4ch
    int 21h
main endp

    print proc
        ; cx stores count of digits in number
        ; set count to zero
        mov cx,0
        mov dx,0
        cmd:
            ; if ax is zero
            cmp ax,0
            je printN
            ; initialize bx to 10
            mov bx,10
            ; extract the last digit
            div bx
            ; push it to stack
            push dx
            ; increment the count
            inc cx
            ; set dx to 0
            mov dx,0
            jmp cmd
        printN:
            ; check if count
            ; is greater than zero
            cmp cx,0
            je exit
            ; pop the top of stack
            pop dx
            ; convert to ASCII
            add dx,'0'
            ; print character interupt
            mov ah,02h
            int 21h
            ; decrease the count
            dec cx
            jmp printN
        exit:
            ret
    print endp
    end main
