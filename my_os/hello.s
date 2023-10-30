.section .text
.globl _start

_start:
    mov $hello, %rdi       # Argument: Address of the string
    call printf            # Call printf function
    call halt              # Call halt function

hello:
    .ascii "Hello, World!\n"  # String to print

# printf function
printf:
    mov $1, %rax             # syscall: write
    mov $1, %rdi             # file descriptor: stdout
    mov $hello, %rsi         # pointer to the string
    mov $14, %rdx            # string length
    syscall
    ret

# halt function (infinite loop)
halt:
    jmp halt