; Program to Change colour of Display when a key is pressed on keyboard
.model small
.stack 64
.data
    color db 10h
.code
main proc near
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; clear screen
    mov ax,3h
    int 10h

    repeat:
        mov ax,0600h    
        mov bh,color
        mov ch,08h           ; row of window's upper left corner
        mov cl,13h           ; column of window's upper left corner
        mov dh,0eh           ; row of window's lower right corner
        mov dl,36h           ; column of window's lower right corner
        int 10h
        add color,10h        ;change background color

    ; keyboard input interrupt
    mov ah,8h
    int 21h
    ; if 0 is pressed then change display color
    cmp al,'0'
    ; otherwise exit
    je repeat

    ; clear screen
    mov ah,06
    mov bh,07h
    mov cx,0000h
    mov dx,184fh
    int 10h

    ; return to dos
    mov ah,4ch
    int 21h

main endp
     end main