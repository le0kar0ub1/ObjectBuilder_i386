%ifndef _MACROS_INC_
%define _MACROS_INC_

;
; MACRO ASM x86_64
;

%macro printf 0
    call printf
%endmacro

%macro printk 0
    call printk
%endmacro

%macro push_base_reg 0
    push rdi
    push rsi
    push rbx
    push rdx
    push rcx
    push rax
%endmacro

%macro pop_base_reg 0
    pop rax
    pop rcx
    pop rdx
    pop rbx
    pop rsi
    pop rdi
%endmacro

%macro push_seg_reg 0
    mov rax, ds
    push rax
    mov rax, es
    push rax
    mov rax, fs
    push rax
    mov rax, gs
    push rax
%endmacro

%macro pop_seg_reg 0
    pop rax
    mov gs, rax
    pop rax
    mov fs, rax
    pop rax
    mov es, rax
    pop rax
    mov ds, rax
%endmacro

%macro exit 0
    mov rax, 0x3C
    mov rdi, 0x0
    syscall
%endmacro

%macro pro1 0
    push rbp
    mov rbp, rsp
    sub rsp, 0x8
    mov QWORD [rbp-0x8], rdi
%endmacro

%macro epi1 0
    add rsp, 0x8
    leave
    ret
%endmacro

%macro pro2 0
    push rbp
    mov rbp, rsp
    sub rsp, 0x10
    mov QWORD [rbp-0x8], rdi
    mov QWORD [rbp-0x10], rsi
%endmacro

%macro epi2 0
    add rsp, 0x10
    leave
    ret
%endmacro

%macro pro3 0
    push rbp
    mov rbp, rsp
    sub rsp, 0x18
    mov QWORD [rbp-0x8], rdi
    mov QWORD [rbp-0x10], rsi
    mov QWORD [rbp-0x18], rdx
%endmacro

%macro epi3 0
    add rsp, 0x18
    leave
    ret
%endmacro

%endif
