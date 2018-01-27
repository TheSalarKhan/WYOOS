#ifndef __PRINT_H
#define __PRINT_H
#include "types.h"
/**
	This file defines logic to print on the 80*25 video memory when the processor is
	initially in protected mode.
*/
struct ColorVideoMemoryWord {
	uint8_t ascii_char;
	uint8_t foreground_color : 4;
	uint8_t background_color : 4;
} __attribute__((packed));

// Our printf will print a newline at the end of every string,
// unless print_newline is specified as false. 
void printf(const char* str,bool print_newline = true);

#endif
