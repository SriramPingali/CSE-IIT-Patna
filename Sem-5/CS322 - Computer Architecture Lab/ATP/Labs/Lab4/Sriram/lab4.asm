; "MOVE CURSOR WITH ARROWS"
;AH=07h - DIRECT CHARACTER INPUT, WITHOUT ECHO
;Return: AL = character read from standard input

;Set cursor position
;10h
;AH=02h  BH = Page Number, DH = Row, DL = Column

.model small
.stack 64
.data
CHARS dw 010h
ENTER_KEY equ 0Dh
ESC_KEY equ 1Bh ; exit
U_KEY equ 48h
D_KEY equ 50h
L_KEY equ 4Bh
R_KEY equ 4Dh

color_red db 0CCh
color_green db 0AAh
space db 020h 

row db 0
col db 0

buffer db 16+1 dup (0) , "$"    ; password buffer
buffB db 1 ; byte buffer
start db "Please enter your first name..." , 0Dh , 0Ah , "$"
PassOkay db 0Dh , 0Ah ,"Nice name bud!! Now Search for all the letters in the console!!" , "$"

.code
    main:   mov ax , @DATA
    mov ds , ax
    mov bx, ENTER_KEY
    mov cx, 0

    lea dx , start
    mov ah , 09h
    int 21h

    readUntilEnter:
        mov bx , ENTER_KEY
        mov ah , 00h
        int 16h
        
        mov buffB , al
        cmp bl , al ; pressed ENTER ?
        je CheckLength
        
        mov ah , 02h
        mov dl , al
        int 21h
        
        mov bx , cx
        mov al , buffB
        mov buffer[bx] , al 
        inc cx
        cmp cx , CHARS
        jle readUntilEnter ; loop for 16 times

    CheckLength:
        cmp cx , CHARS
        jl PassOK

    PassOK:
        lea si, buffer
        lea dx , PassOkay
        mov ah , 09h
        int 21h
    
    ; cursor starting position
    mov ah , 02h
    mov bh , 00h
    mov dx , 00h
    int 10h

    mov bx , ESC_KEY
    
    readUntilESC:
        mov ah , 07h
        int 21h
       
        cmp al , U_KEY
        je UP
        
        cmp al , D_KEY
        je DOWN
        
        cmp al , L_KEY
        je LEFT
        
        cmp al , R_KEY
        je RIGHT

        cmp al, ENTER_KEY
        je ENTER
        
        cmp al , ESC_KEY
        jne readUntilESC ; loop until ESC
        jmp Exit
    
    ; update cursor poistion
    ; console resolution 80x25 (0-79 x 0-24)
    UP:
        cmp dh , 0
        je printCursor
        dec dh
        jmp printCursor
    DOWN:
        cmp dh , 24
        je printCursor
        inc dh 
        jmp printCursor
    LEFT:
        cmp dl , 0
        je printCursor
        dec dl
        jmp printCursor
    RIGHT:
        cmp dl , 79
        je printCursor
        inc dl
        jmp printCursor
    ENTER:
        mov ah, 08h
        int 10h
        cmp al, [si]
        je match
        jne no_match

    match:
        mov ah , 02h
        mov bl , color_green
        mov bh , 00h
        int 10h

        mov ah , 09h
        mov al , space   ; space
        push cx
        mov cx , 01h
        int 10h
        pop cx

        inc si
        dec cx
        jnz readUntilESC
        jz exit

    no_match:
        mov ah , 02h
        mov bl , color_red
        mov bh , 00h
        int 10h

        mov ah , 09h
        mov al , space   ; space
        push cx
        mov cx , 01h
        int 10h
        pop cx
    
    ; print char and go back to main loop
    printCursor:
    mov ah , 02h
    mov bh , 00h
    int 10h
    jmp readUntilESC   
 
 exit:  
      mov ah, 4ch
      int 21h
      end main
     .end