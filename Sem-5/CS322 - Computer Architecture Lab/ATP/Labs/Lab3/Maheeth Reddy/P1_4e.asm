; Program For Reading and Displaying a Character
.model small
.stack 64
.data
    prompt db 'Enter a character: $'
    newline db ' ',13,10,'$'
    finish db 'You entered $'
.code
main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax

    ; show prompt message
    ; ask user to enter a character
    mov ah,09
    lea dx,prompt
    int 21h

    ; keyboard input interrupt
    mov ah, 1h
    ; read character into al
    int 21h
    mov cl, al  ; copy character to cl

    ; print newline
    mov   ah, 09      
	lea   dx, newline    
	int   21h

    ; end of prompt
    mov ah,09
    lea dx,finish
    int 21h

    ; character output interrupt
    mov dl,cl
    mov ah, 2h
    int 21h

    ; return to dos
    mov ah,4ch
    int 21h
main endp
     end main