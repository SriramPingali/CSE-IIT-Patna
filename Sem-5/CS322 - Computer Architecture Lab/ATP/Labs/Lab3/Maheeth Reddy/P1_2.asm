; Program to find average of given 16-bit numbers
.model small
.stack 64
.data
	num dw 0005h, 0002h, 0004h, 0001h, 0003h
	avg dw 0000h
.code
main proc far
	; initialize data segment register
	mov ax, @data
	mov ds, ax

	; load address of num into si
	lea si, num
	; cx stores count of remaining numbers
	mov cx, 05h
	mov dl, 05h

	; ax stores sum of given numbers
	mov ax, 0000h

	; calculate sum of given numbers
	repeat:	add ax, [si]
			add si,2
			dec cx
			;if cx is not zero, jump to repeat
			jnz repeat
	
	div dl          ;Divide Acc with DL
	mov avg, ax      ;Store value

	mov ah,4ch
	int 21h
main endp
	 end main