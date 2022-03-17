; Program to generate arithmetic progression of n numbers
.model small
.stack 256
.data
    n db 05h    ; number of terms
    a db 03h    ; first term
    d db 09h    ; common difference
    s db ?      ; sequence
.code
main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; cl stores count remaining numbers
    xor cx,cx
    mov cl,n

    ; al holds next term
    xor ax,ax
    mov al,a

    ; bl holds common difference
    mov bl,d

    ; load address of s into di register
    lea di,s

    ; store the sequence
    ; by calculating and storing 
    ; each term
    mov [di],al
    inc di
    dec cl
    repeat: add al,bl
            mov [di],al

            inc di
            loop repeat

    ; return to dos
    mov ah,4ch
    int 21h
main endp
     end main