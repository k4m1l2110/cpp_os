.set MAGIC, 0x1BADB002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM


.section .text
.extern kernelMain
.extern callConstructors
.global loader


loader:
    mov $kernel_stack, %rsp
    call CallConstructors
    push %rax
    push %rbx
    call KernelMain


_stop:
    cli
    hlt
    jmp _stop


.section .bss
.space 16384;
kernel_stack: