#include "printf.h"
#define VIDEO_MEMORY_BASE_ADDRESS (void*)0xb8000
// Defining a pointer to the memory, so that it can be
// addressed as a 2d array with dimensions 80 columns and 'n' rows.
ColorVideoMemoryWord (* const screen)[80] = static_cast<ColorVideoMemoryWord (*)[80]> (VIDEO_MEMORY_BASE_ADDRESS);

void clear_screen(ColorVideoMemoryWord (*screen)[80]) {
	// Write the whole video memory clean
	for(register int i=0; i<25; i++) {
		for(register int j=0; j<80; j++) {
			screen[i][j].ascii_char = (uint8_t) ' ';
		}
	}
}

void printf(const char* str) {
	/**
	  This function handles writing strings to colored video memory in protected mode.
	  Video memory is a 2d array of 80*25 words - two bytes - starting at address
	  0x000b8000. For each word, the first byte represents the ascii code for the character, and the second
	  byte is the color byte for the character. In the color byte the first 4 bits represent
	  foreground color, and the last 4 bits represent the background color.
	 **/

	// These two variables will track the cursor position.
	static uint8_t cursor_row = 0, cursor_col = 0;

	// BEWARE: The string 'str' must be null terminated.
	// Printing loop
	for(register int i=0; str[i] != '\0'; i++) {
		// Perform a line break if '\n' else
		// print character.
		switch(str[i]) {
			case '\n':
				cursor_row++;
				cursor_col = 0;
				break;
			default:
				screen[cursor_row][cursor_col].foreground_color = 0xa;
				screen[cursor_row][cursor_col].background_color = 0x0;
				screen[cursor_row][cursor_col++].ascii_char = str[i];
		}

		// If we have just written the last row member.
		// We must perform a line break
		if(cursor_col > 80) {
			cursor_row++;
			cursor_col = 0;
		}

		// If we have just written the last row
		// Then we must clear the screen and start over.
		if(cursor_row > 25) {
			clear_screen(screen);
			cursor_row = cursor_col = 0;
		}
	}
}
