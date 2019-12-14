global _start

section .text
_start:
	mov esi, DATA
print:
	cmp BYTE [esi], 0x0
    je end
	mov eax, 0x4
	mov ebx, 0x1
	mov ecx, esi
	mov edx, 0x1
	int 0x80
	inc esi 
	jmp print
end:
	mov eax, 0x1
	mov ebx, 0x0
	int 0x80

section .data
	DATA db 'You just have compiled me!\n\0'
