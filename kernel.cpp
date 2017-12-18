void printf(const char* str) {
	unsigned short* VIDEO_MEMORY = (unsigned short*)0xb8000;
	/**
	  Make sure that the string 'str' is null terminated
	  or this loop will continue past the string.
	 */
	for(int i=0; str[i] != '\0'; i++) {
		VIDEO_MEMORY[i] = (VIDEO_MEMORY[i]&0xFF00) | str[i];
	}
}


extern "C" void kernel_main(const void* multiboot_struct, unsigned int magic_number)
{
	printf("Hello World");
	
	// At the end of everything, we enter an
	// infinite loop Because:
	// 1) Control should not be here in the first place -_-
	// 2) But now that we are here. We cannot simply do nothing,
	// that's just not how computers work.
	while(1);
}
