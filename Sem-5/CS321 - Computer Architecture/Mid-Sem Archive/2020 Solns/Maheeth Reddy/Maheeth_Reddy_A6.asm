; Program for monitoring Port A temperature in 8255
.model small
.stack 64
.data
    ; required temp is 64h
    req_temp equ 64h
    ; port A is at 80h
    portA equ 80h
    ; port B is at 81h
    portB equ 81h

.code
main proc far
begin:
    mov ax,@data
    mov ds,ax

    ; obtain temperature of port A into al register
    in al,portA

    ; compare temperature and 64h
    cmp al,req_temp

    ; if it is lesser keep comparing
    jle begin

    ; otherwise send it to port B
    out portB,al

    ; exit program
    mov ah,4ch
    int 21h

main endp
     end main