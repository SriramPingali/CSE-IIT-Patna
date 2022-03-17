# Two_Pass_Assembler_Emulator

Assembler and Emulator implementation in C Language for the given Assembly Language. Project as a part of CS321-CS322 Computer Architecture Course IIT-Patna.

## Rules of the given Assembly Language

* Two registers, A & B, arranged as an internal stack
* A program counter, PC
* A stack pointer, SP
* These registers are 32 bits in size
* Instructions have either no operands or a single operand
* The operand is a signed 2's complement value.
* The encoding uses the bottom 8 bits for opcode and the upper 24 bits for operand.
* Comments are denoted using ';' (semicolon)
* An operand is either a label or a number, the number can be decimal, hex or octal.
* Label definitions consist of the label name followed by a ':'
* Label Name is alpha_numberic
* Label used to calculate displacement in case of branch instructions.
* An operand is either a label or a number, the number can be decimal, hex or octal.

<img width="581" alt="instructions" src="https://user-images.githubusercontent.com/41947720/102636143-4ab31d00-417a-11eb-9676-a6e4798f2a0e.png">

## Assembler

* Has two data structures to store respectively labels and mnemonic table with expected operands.
* Reads the file once, converts to internal format using linked list and then parses.
* detects label errors.
* consistent and sensible formatting with sensible program structure.
* sensible variable, function & type names with explanatory comments.
* advanced listing file.
* assembles ALL test programs.                                 
* CAN IMPLEMENT and demonstrates the instruction SET.

## Emulator

* Three data structures are used to store respectively memory, 	mnemonic table with expected operands and a linked list structure 	code.
* loads object file,
* Does automatic trace i.e at each step displays the value of PC, SP, reg A, and Reg B
* Does automatic memory dump, before and after the program to show the state of memory elements.

## TestCases

Test cases consists of sample programs written in the simple assembly language. There are a total of 10 such programs. Test cases are present in the folder cs33testcases. The listing and object file obtained after execution from the assembler is also provided.

## Compiling and Running

![image](https://user-images.githubusercontent.com/41947720/102637661-57d10b80-417c-11eb-8a4c-0238e6dc7a3d.png)


1) For assembler
gcc asm.c -o asm

2) For emulator
gcc emu.c -o emu

### Running on given test cases
Assembler -
./asm cs33testcases/filename.asm

Emulator -
./emu cs33testcases/filename_obj.o

### Sample Execution

Compiling and linking the bubble sort program written in the assembly Language

./asm cs33bubble.asm
./emu cs33bubble_obj.o

Before sorting (numbers in random order)

![image](https://user-images.githubusercontent.com/41947720/102637811-98c92000-417c-11eb-860d-b6f0a0277076.png)

After soring (numbers in sorted order)

![image](https://user-images.githubusercontent.com/41947720/102637881-b0080d80-417c-11eb-911e-a579c1df9db7.png)
