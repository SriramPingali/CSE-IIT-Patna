; Program to find sum of 10 random numbers
.model small
.stack 64
.data
    ; numbers are 1,4,2,7,5,9,6,3,8,0
    num db 01h,04h,02h,07h,05h,09h,06h,03h,08h,00h
    sum db 0        ; variable to store sum
.code
main proc far
    mov ax,@data
    ; load data segment address
    mov ds,ax

    ; bl stores count of remaining numbers
    mov bl,0ah
    ; load num
    lea si,num
    mov ax,0000h

repeat: add al,[si]
        inc si
        dec bl
        jnz repeat

    mov sum,al

    ; return to dos
    mov ah,4ch
    int 21h
main endp
     end main
