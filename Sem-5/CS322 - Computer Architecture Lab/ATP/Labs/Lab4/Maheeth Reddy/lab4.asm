; Name of Student: M.Maheeth Reddy
;         Roll No: 1801CS31

; A Simple Bat and Ball game in 8086 assembly language

.model large
.stack 64
.data
    ; default x- and y-coordinates of the ball
    ball_initx     dw 0ah
    ball_inity     dw 0ah

    ; current x- and y-coordinates of the ball
    ; to track its motion
    ball_x         dw 0a0h
    ball_y         dw 64h

    ; dimensions of the ball
    ; height and width (in pixels)
    ball_size      dw 04h

    ; velocity of ball in x- and y-direction
    ball_velx      dw 04h
    ball_vely      dw 04h

    ; current x- and y-coordinates of the bat
    ; to track its motion
    ; initialized to appear at the middle of the bottom of the screen
    bat_x          dw 98h
    bat_y          dw 0b0h

    ; dimensions of the bat (in pixels)
    bat_length     dw 30h
    bat_breadth    dw 05h

    ; velocity of bat
    bat_vel        dw 08h

    ; window dimensions (in pixels)
    window_length  dw 140h
    window_breadth dw 0c8h

    ; helps to identify a possible collision beforehand
    ; and update velocity of the ball at the right time
    buffer_width   dw 06h

    ; used for checking change in time
    delta          db 00h
.code
main proc far
    ; initialize data segment register
    mov ax,@data
    mov ds,ax
    
    ; clear screen and set video mode
    call clear
    
    ; continuously check time and
    ; move objects on screen as per
    ; keyboard input
    isTimeChange:
        ; get system time
        mov ah,2ch
        int 21h
        
        ; check for change in time
        cmp dl,delta
        je isTimeChange
        
        ; if time has passed
        ; update time
        mov delta,dl
        call clear   ; avoid traces while moving the objects
        
        ; update ball location
        call move_ball
        call draw_ball
        
        ; update bat location
        call move_bat
        call draw_bat
        
        jmp isTimeChange
    
    ret
main endp

; calculate position of the ball
move_ball proc near
    ; update x-coordinate of the ball
    mov ax,ball_velx    
    add ball_x,ax                   

    ; if ball is colliding with left edge
    ; change its x-direction velocity
    mov ax,buffer_width
    cmp ball_x,ax
    jl change_velx
    
    ; if ball is colliding with right edge
    ; change its x-direction velocity
    mov ax,window_length
    sub ax,ball_size
    sub ax,buffer_width
    cmp ball_x,ax
    jg change_velx
    
    ; update y-coordinate of the ball
    mov ax,ball_vely
    add ball_y,ax
    
    ; if ball is colliding with top edge
    ; change its y-direction velocity
    mov ax,buffer_width
    cmp ball_y,ax
    jl change_vely

    ; if ball is colliding with bottom edge
    ; restart game 
    mov ax,window_breadth   
    sub ax,ball_size
    sub ax,buffer_width
    cmp ball_y,ax
    jg reset_position
    
    ; ball hits the bat when
    ; x- and y-coordinates of ball are between those of bat
    ball_bat_contact:
    mov ax,ball_x
    add ax,ball_size
    cmp ax,bat_x
    jng return

    mov ax,bat_x
    add ax,bat_length
    cmp ball_x,ax
    jnl return

    mov ax,ball_y
    add ax,ball_size
    cmp ax,bat_y
    jng return

    mov ax,bat_y
    add ax,bat_breadth
    cmp ball_y,ax
    jnl return

    ; if the ball hits the bat
    neg ball_vely
    return: ret

    ; reverse x-direction velocity of ball
    change_velx:
        neg ball_velx
        ret                              

    reset_position:                  
        call reset_ball_position
        ret

    ; reverse y-direction velocity of ball
    change_vely:
        neg ball_vely
        ret
move_ball endp

; calculate position of the bat
move_bat proc near
    ; is there any keyboard input
    mov ah,01h
    int 16h
    ; if no, don't change bat position
    jz return_bat_loc
    
    ; if yes, check the key
    mov ah,00h
    int 16h
    
    ;if key is ',' move left
    cmp al,2ch
    je move_bat_left
    
    ;if key is '.' move right
    cmp al,2eh
    je move_bat_right
    jmp return_bat_loc
    
    move_bat_left:
        mov ax,bat_vel
        sub bat_x,ax
        
        mov ax,buffer_width
        cmp bat_x,ax
        jl restrict_bat_left
        jmp return_bat_loc
        
        ; bat should not move out of the screen
        restrict_bat_left:
            mov bat_x,ax
            jmp return_bat_loc

    move_bat_right:
        mov ax,bat_vel
        add bat_x,ax
        mov ax,window_length
        sub ax,buffer_width
        sub ax,bat_length
        cmp bat_x,ax
        jg restrict_bat_right
        jmp return_bat_loc
        
        ; bat should not move out of the screen
        restrict_bat_right:
            mov bat_x,ax
            jmp return_bat_loc
    
    return_bat_loc: ret
move_bat endp

; reset ball location for game restarts
reset_ball_position proc near    
    mov ax,ball_initx
    mov ball_x,ax

    mov ax,ball_inity
    mov ball_y,ax

    ret
reset_ball_position endp

; display the ball on the screen
draw_ball proc near
    ; initial ball location
    mov cx,ball_x
    mov dx,ball_y
    
    draw_ball1:
        ; pixel writing mode
        mov ah,0ch
        mov al,0fh
        mov bh,00h
        int 10h
        
        ; draw a line completely
        inc cx
        mov ax,cx
        sub ax,ball_x
        cmp ax,ball_size
        jng draw_ball1
        
        ; then go to next line
        mov cx,ball_x
        inc dx
        
        mov ax,dx
        sub ax,ball_y
        cmp ax,ball_size
        jng draw_ball1

    ret
draw_ball endp

; display the bat on the screen
draw_bat proc near
    ; current bat location
    mov cx,bat_x
    mov dx,bat_y
    
    draw_bat1:
        ; pixel writing mode
        mov ah,0ch
        mov al,0fh
        mov bh,00h
        int 10h
        
        ; draw a line completely
        inc cx
        mov ax,cx
        sub ax,bat_x
        cmp ax,bat_length
        jng draw_bat1
        
        ; then go to next line
        mov cx,bat_x
        inc dx
        
        mov ax,dx
        sub ax,bat_y
        cmp ax,bat_breadth
        jng draw_bat1
        
    ret
draw_bat endp

; clear the screen
clear proc near
    ; set graphical video mode
    ; 320 x 200 pixels in 1 page
    ; note that window borders have same values
    mov ah,00h
    mov al,13h
    int 10h

    ; set background as black
    mov ah,0bh
    mov bh,00h
    mov bl,00h
    int 10h
    
    ret    
clear endp

end main