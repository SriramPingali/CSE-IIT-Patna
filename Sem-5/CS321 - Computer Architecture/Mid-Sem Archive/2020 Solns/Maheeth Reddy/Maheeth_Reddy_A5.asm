; Program to count number of vowels in an alphabet string
.model small
.stack 64
.data
	input DB 'MaheEth$'
	len	DW 0
	vowCount DB 0
	vowels DB 'aeiou$'
	numV EQU 05h
	buffer DB ?
.code
main proc far
	; initialize data segment register
	mov ax,@data
	mov ds,ax

	; initialize extra segment register
	mov es,ax

	; load address of string to DI register
	lea di,input

	; count length of string
	mov al,'$'	; end of string character
	lea si,input
count:
	; compare [di] with end of string character
	scasb
	; if equal, stop counting
	je copy_str
	; otherwise increment length of string
	inc len
	; keep counting
	jmp count

copy_str:
	; copy string from input to buffer using movsb
	mov cx, len
	lea si, input
	lea di, buffer
	rep movsb

	; convert buffer to lowercase
	mov cx, len
	lea si, buffer
	lea di, buffer
to_lower:
	; retrieve character from buffer
	lodsb
	or al,20h
	; change case and store in buffer
	stosb
	dec cx
	jnz to_lower

	; search for vowels in buffer
	lea si,buffer
	mov bx,len
search:
	mov al,[si]
	lea di,vowels
	mov cx,numV
	cld
	repne scasb
	jne not_found

	; if a character is vowel increment vowCount
found:
	inc vowCount
	mov dl,vowCount

	; otherwise, read next character
not_found:
	inc si
	dec bx
	jnz search

	; copy vowCount to dl register to see in debugger
	mov dl,vowCount

	; exit program
	mov ah,4ch
	int 21h	
main endp		
	 end main