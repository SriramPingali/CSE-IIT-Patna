; program to find gray code of a number
.model small
.stack 64
.data
    num db 38h   ; given number
    gray db ?    ; gray code
.code
main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; calculate gray code
    mov al,num
    mov bl,al
    shr al,01
    xor bl,al

    ; store gray code
    mov gray,bl

    ; return to dos
    mov ah,4ch
    int 21h
main endp
     end main