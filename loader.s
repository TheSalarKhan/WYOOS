.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section multiboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

.section .text
.extern kernel_main
.global loader


loader:
	mov $kernel_stack, %esp
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
