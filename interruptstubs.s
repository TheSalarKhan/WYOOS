.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15HandleInterruptEhj

.global _ZN16InterruptManager15IgnoreInterruptEv

.macro HandleExceptionMacro num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
	movb $\num, (interrupt_number)
	jmp int_bottom
.endm

.macro HandleInterruptRequestMacro num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (interrupt_number)
	jmp int_bottom
.endm

HandleInterruptRequestMacro 0x00
HandleInterruptRequestMacro 0x01

int_bottom:
	# save the current state by pushing stuff to the stack
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	# push the current esp and the
	# interrupt number to the arguments
	# and call the interrupt handler
	push %esp
	push (interrupt_number)
	call _ZN16InterruptManager15HandleInterruptEhj
	movl %eax, %esp # restore the returned esp

	# restore the state by reloading the registers in the
	# reverse order.
	pushl %gs
	pushl %fs
	pushl %es
	pushl %ds
	pusha

	# return from the interrupt handler
	iret

_ZN16InterruptManager15IgnoreInterruptEv:
	iret
.data
	interrupt_number: .byte 0
