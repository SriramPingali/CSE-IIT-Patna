
default:
	gcc -g -o testme -z execstack -fno-stack-protector testme.c
	gcc -o manges mangesh.c	
	gcc -o exploit exploit.c
	./exploit
	./testme