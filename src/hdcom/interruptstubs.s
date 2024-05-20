.set IRQ_BASE, 0x20

hello:
    .string "Interrupt handled!\n"


.section .text
.extern Print
.extern _ZN6cpp_os5hdcom16InterruptManager15HandleInterruptEhj

.macro HandleException num
.global _ZN6cpp_os5hdcom16InterruptManager15HandleException\num\()Ev
_ZN6cpp_os5hdcom16InterruptManager15HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN6cpp_os5hdcom16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN6cpp_os5hdcom16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C

int_bottom:
    pushl %ebp
	pushl %edi
	pushl %esi

	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax

	pushl %esp
    push (interruptnumber)
    call _ZN6cpp_os5hdcom16InterruptManager15HandleInterruptEhj
    # addl $5, %esp
    movl %eax, %esp

    popl %eax
	popl %ebx
	popl %ecx
	popl %edx

	popl %esi
	popl %edi
	popl %ebp

	add $4, %esp

.global _ZN6cpp_os5hdcom16InterruptManager22IgnoreInterruptRequestEv
_ZN6cpp_os5hdcom16InterruptManager22IgnoreInterruptRequestEv:
    iret

.data
    interruptnumber: .byte 0
