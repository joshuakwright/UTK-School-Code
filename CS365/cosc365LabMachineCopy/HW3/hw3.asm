; Joshua Wright jwrig117

%macro Print 2
	mov eax, 4
	mov ebx, 1
	mov ecx, %1
	mov edx, %2
	int 80h
%endmacro

%macro Read 2
	mov eax, 3
	mov ebx, 0
	mov ecx, %1
	mov edx, %2
	int 80h
%endmacro

%macro PrintNum 1

%%PrintingArray:
	xor eax, eax
	xor ecx, ecx
	mov eax, [%1]
%%ParsingNumbers:
	mov ecx, 10
	mov edx, 0
	div ecx
	add edx, '0'
	mov [place3], edx
	xor edx, edx
	mov edx, 0
	div ecx
	add edx, '0'
	mov [place2], edx
	add eax, '0'
	mov [place1], eax

	xor edx, edx
	mov edx, [place1]
	cmp edx, '0'
	je %%print2
	Print place1, 1
	jmp %%print3
	
%%print2: 
	xor edx, edx
	mov edx, [place2]
	cmp edx, '0'
	je %%print1

%%print3:
	Print place2, 1

%%print1:
	Print place3, 1

%endmacro

section .data
	inputMsg db 'Please enter a list of numbers, separated by any whitespace. At the end of the list, add an X.', 0xa, 0xD
	lenInputMsg equ $-inputMsg

	sortedMsg db 'The sorted list is the following:', 0xa, 0xD
	lenSortedMsg equ $-sortedMsg

	endListMsg db 'End of list', 0xa, 0xD
	lenEndListMsg equ $-endListMsg

	ncMsg db 0xa
	lenNcMsg equ $-ncMsg

	place3 dd 0
	place2 dd 0
	place1 dd 0
	num2 dd 0
	num dd 0
	endList dd 0

	head dd 0
	tail dd 0
	temp dd 0
	heap_base dd 0

section .bss
	num1 resb 1

section .text 
	global _start

_start:	
	Print inputMsg, lenInputMsg
	xor eax, eax
	xor ecx, ecx
	mov ecx, 0

ReadInNum:
	mov [num2], eax
	mov eax, 0
	mov [num1], eax
	Read num1, 1
	xor eax, eax
	mov eax, [num2]
	xor ebx, ebx
	mov ebx, [num1]
	cmp ebx, 'X'
	je endInput
	cmp ebx, '0'
	jl insertNum
	cmp ebx, '9'
	jg insertNum
	sub ebx, '0'
	cmp eax, 0
	jne addNum
	mov eax, ebx
	jmp ReadInNum

addNum:
	mov [num1], ebx
	mov ebx, 10
	mul ebx
	mov ebx, [num1]
	add eax, ebx
	jmp ReadInNum

endInput:
	cmp eax, 0
	je outputList
	xor ebx, ebx
	mov ebx, 1
	mov [endList], ebx
	jmp insertNum

insertNum: 
	cmp eax, 0
	je ReadInNum

	mov [num], eax
	xor eax, eax

	mov eax, 45
	xor ebx, ebx
	int 80h
	mov [heap_base], eax

	mov eax, 45
	mov ebx, [heap_base]
	add ebx, 8
	int 80h

	mov [temp], eax
	mov ecx, [num]
	mov DWORD [eax], ecx
	
	add eax, 4
	mov DWORD [eax], 0

	cmp DWORD [tail], 0
	jnz listExists
	
	mov eax, [temp]
	mov [head], eax
	mov [tail], eax
	
	mov eax, 0
	cmp DWORD [endList], 0
	jne outputList
	jmp ReadInNum

listExists:
	mov eax, [head]
	mov edx, eax

findPlace:
	mov ebx, [eax]
	mov ecx, [num]
	cmp ecx, ebx
	jle placeNum
	
	mov edx, eax
	add eax, 4
	mov eax, [eax]
	cmp eax, 0
	je endOfList
	jmp findPlace

placeNum:
	cmp eax, edx
	je newHead
	mov ebx, [temp]
	add edx, 4
	mov [edx], ebx
	add ebx, 4
	mov [ebx], eax

	mov eax, 0
	cmp DWORD[endList], 0
	jne outputList
	jmp ReadInNum

newHead:
	mov ebx, [temp]
	mov [head], ebx
	add ebx, 4
	mov [ebx], eax
	mov eax, 0
	cmp DWORD[endList], 0
	jne outputList
	jmp ReadInNum

endOfList:
	mov eax, [tail]
	add eax, 4
	mov ebx, [temp]
	mov [eax], ebx
	mov [tail], ebx
	mov eax, 0
	cmp DWORD[endList], 0
	jne outputList
	jmp ReadInNum

outputList:
	Call PrintNewline
	Print sortedMsg, lenSortedMsg
	mov eax, [head]

outputPostHead:
	mov ebx, [eax]
	mov [num], ebx
	mov [temp], eax
	PrintNum num
	call PrintNewline
	mov eax, [temp]
	add eax, 4
	mov eax, [eax]
	cmp eax, 0
	je exit
	jmp outputPostHead

exit:
	Print endListMsg, lenEndListMsg
	mov eax, 1
	int 80h

PrintNewline:
	mov eax, 4
	mov ebx, 1
	mov ecx, ncMsg
	mov edx, lenNcMsg
	int 80h
ret