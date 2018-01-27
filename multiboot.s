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
