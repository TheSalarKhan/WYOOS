.section .text
.extern kernel_main
.extern call_constructors
.global loader


loader:
	mov $kernel_stack, %esp
	# Before calline kernel_main, call
	# the function that calls all static
	# constructors.
	call call_constructors
	push %eax
	push %ebx
	call kernel_main
	# The control is never supposed to
	# leave the kernel_main function. If it does
	# we will fall into the following forever loop.
	_loop_forever:
		cli
		hlt
		jmp _loop_forever
.section .bss
# 2 Mib space for the kernel_stack
.space 2*1024*1024
kernel_stack:
