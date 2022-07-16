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

%macro PrintSumRow 1

	xor eax, eax
	mov eax, 0
	mov [sum], eax
	
	add eax, nums[(0 + %1) * 4]
	add eax, nums[(1 + %1) * 4]
	add eax, nums[(2 + %1) * 4]
	add eax, nums[(3 + %1) * 4]

	mov [sum], eax
	PrintNum sum
	
%endmacro

%macro PrintSumCol 1
	
	xor eax, eax
	mov eax, 0
	mov [sum], eax

	add eax, nums[(0 + %1) * 4]
	add eax, nums[(4 + %1) * 4]
	add eax, nums[(8 + %1) * 4]

	mov [sum], eax
	PrintNum sum

%endmacro

section .data
	inputMsg db 'Please enter 12 numbers:', 0xa, 0xD
	lenInputMsg equ $-inputMsg

	outputMsg db 'Your numbers are:', 0xa, 0xD
	lenOutputMsg equ $-outputMsg
	
	sumMsg db 'The sum of the array is:', 0xa, 0xD
	lenSumMsg equ $-sumMsg

	row1Sum db 'The sum of row 1 is:', 0xa, 0xD
	lenRow1Sum equ $-row1Sum

	row2Sum db 'The sum of row 2 is:', 0xa, 0xD
	lenRow2Sum equ $-row2Sum

	row3Sum db 'The sum of row 3 is:', 0xa, 0xD
	lenRow3Sum equ $-row3Sum

	col1Sum db 'The sum of col 1 is:', 0xa, 0xD
	lenCol1Sum equ $-col1Sum

	col2Sum db 'The sum of col 2 is:', 0xa, 0xD
	lenCol2Sum equ $-col2Sum

	col3Sum db 'The sum of col 3 is:', 0xa, 0xD
	lenCol3Sum equ $-col3Sum

	col4Sum db 'The sum of col 4 is:', 0xa, 0xD
	lenCol4Sum equ $-col4Sum	

	ncMsg db 0xa
	lenNcMsg equ $-ncMsg

	testMsg db 'test'
	lenTest equ $-testMsg

	spaceMsg db ' '
	lenSpaceMsg equ $-spaceMsg

	nums dd 0, 0, 0, 0
		 dd 0, 0, 0, 0
		 dd 0, 0, 0, 0

	sum dd 0

	place3 dd 0
	place2 dd 0
	place1 dd 0
	num2 dd 0

section .bss
	num1 resb 1
	outputNum resb 1

section .text 
	global _start

_start:
	Print inputMsg, lenInputMsg
	xor eax, eax
	xor ecx, ecx
	mov ecx, 0
	push ecx

ReadInNum:
	mov [num2], eax
	mov eax, 0
	mov [num1], eax
	Read num1, 1
	xor eax, eax
	mov eax, [num2]
	xor ebx, ebx
	mov ebx, [num1]
	cmp ebx, ' '
	je insertNum
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

insertNum: 
	pop ecx
	add nums[ecx*4], eax

	mov edx, 0
	mov edx, [sum]
	add edx, eax
	mov [sum], edx

	inc ecx
	cmp ecx, 12
	push ecx
	mov eax, 0
	jl ReadInNum
	pop ecx

	call PrintNewline
	Print sumMsg, lenSumMsg

	PrintNum sum
	call PrintNewline
	call PrintNewline

	Print row1Sum, lenRow1Sum
	PrintSumRow 0
	call PrintNewline
	call PrintNewline

	Print row2Sum, lenRow2Sum
	PrintSumRow 4
	call PrintNewline
	call PrintNewline

	Print row3Sum, lenRow3Sum
	PrintSumRow 8
	call PrintNewline
	call PrintNewline

	Print col1Sum, lenCol1Sum
	PrintSumCol 0
	call PrintNewline
	call PrintNewline

	Print col2Sum, lenCol2Sum
	PrintSumCol 1
	call PrintNewline
	call PrintNewline

	Print col3Sum, lenCol3Sum
	PrintSumCol 2
	call PrintNewline
	call PrintNewline

	Print col4Sum, lenCol4Sum
	PrintSumCol 3
	call PrintNewline

exit:
	call PrintNewline
	mov eax, 1
	int 80h

PrintNewline:
	mov eax, 4
	mov ebx, 1
	mov ecx, ncMsg
	mov edx, lenNcMsg
	int 80h
ret
