; Program to demonstrate various addressing modes in 8086
.model small
.stack 64
.data
    ; data segment variables are stored from 
    ; effective address 7000h onwards
    org 7000h
    var db 52h,65h,0FFh,54h,99h,74h
    source db "NO$"
    dest   db "PA$"
.code
main proc far
    mov ax,@data
    ; register addressing
    mov ds,ax
    mov es,ax

    ; immediate addressing
    mov ax,2024h

    ; direct addressing
    mov ch,ds:[7009h]

    lea bx,var
    ;register indirect addressing
    mov cl,[bx]

    ;based addressing
    mov ch,[bx+01h]

    lea si,var
    ;indexed addressing
    mov cl,[si+02h]

    mov si,0001h
    ; based index addressing
    mov dl,[bx+si+0002h]

    lea si,source
    lea di,dest
    ; string addressing
    movsw

    ; direct i/o port addressing
    in al,[0009h]

    mov al,01h
ABC:
    ; implied addressing
    clc

    sub al,01h
    ; relative addressing
    jz ABC

    mov ah,4ch
    int 21h

main endp
     end main