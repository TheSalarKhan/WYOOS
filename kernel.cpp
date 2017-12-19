#include "types.h"
#include "global_descriptor_table.h"

void printf(const char* str) {
	uint16_t* VIDEO_MEMORY = (uint16_t*)0xb8000;
	/**
	  Make sure that the string 'str' is null terminated
	  or this loop will continue past the string.
	 */
	for(int i=0; str[i] != '\0'; i++) {
		VIDEO_MEMORY[i] = (VIDEO_MEMORY[i]&0xFF00) | str[i];
	}
}

typedef void (*constructor) ();
extern "C" constructor* start_ctors;
extern "C" constructor* end_ctors;
extern "C" void call_constructors()
{
	/**
	  Start from the address in start_ctors, and move until
	  that of just before end_ctors, and call all the constructors in between.
	  */
	for(constructor* i = start_ctors; i != end_ctors; i++) {
		(*i)();
	}
}

extern "C" void kernel_main(const void* multiboot_struct, uint32_t magic_number)
{
	printf("Hello World");

	// Instantiate the global descriptor table.
	GlobalDescriptorTable table;
	
	// At the end of everything, we enter an
	// infinite loop Because:
	// 1) Control should not be here in the first place -_-
	// 2) But now that we are here. We cannot simply do nothing,
	// that's just not how computers work.
	while(1);
}
