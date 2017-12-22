.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section multiboot
	# the .align 4 is very important to have
	# it ensures that our multiboot header is aligned
	# properly with the 32-bit boundary.
	# This was found after hours of trying different
	# things out. Finally found this in a snippet
	# on: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

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
