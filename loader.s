
.section .text
.extern kernel_main
.global loader


loader:
	mov $kernel_stack, %esp
	call kernel_main
# The control is never supposed to
# leave the kernel_main function. If it does
# we will fall into the following forever loop.
_loop_forever:
	cli
	htl
	jmp _loop_forever
.section .bss
# 2 Mib space for the kernel_stack
.space 2*1024*1024
kernel_stack:
