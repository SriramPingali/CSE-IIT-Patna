; Program to find factorial of a number
.model small
.stack 64
.data
    n dw 8  ; number
    f dd 0  ; factorial
.code
main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; load n value to cx
    xor cx,cx
    mov cx, n

    ; load address of f in di
    lea di, f

    ; ax stores less significant byte
    mov ax, 0001
    ; dx stores more significant byte
    mov dx, 0000

    ; calculate factorial
    cmd: mul cx
    loop cmd

    mov [di], ax
    inc di
    mov [di], dx

    ; return to dos
    mov ah,4ch
    int 21h

main endp
     end main