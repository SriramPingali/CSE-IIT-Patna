; program to convert hexadecimal to bcd
.model small
.stack 64
.data
    hex db 0fh  ; given number
    bcd dw ?    ; bcd representation
.code
main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; ax stores bcd form
    mov ax,0
    ; cl stores num
    mov cl,hex

; calculate bcd form
loop2:  add al,01h
        daa
        jnc loop1
        inc ah
loop1:  dec cx
        jnz loop2

    ; store bcd representation
    mov bcd,ax

    ; return to dos
    mov ah,4ch
    int 21h
main endp
    end main