#include "global_descriptor_table.h"


GlobalDescriptorTable::GlobalDescriptorTable() :
	null_segment_selector_(0,0,0),
	unused_segment_selector_(0,0,0),
	code_segment_selector_(0, 64*1024*1024, 0x9A),
	data_segment_selector_(0, 64*1024*1024, 0x92)
{
	/**
		This is the constructor. Assuming that the segment selectors have been initialized
		we will now load the table using the lgdt assembly instruction. The format for this
		instruction is:
			lgdt (address_of_gdt_description_struct)
		gdt description struct is a structure layed out as follows:
		Bytes 0-1: Size of the GDT subtracted by 1.
		Bytes 2-5: 4 byte linear base address of GDT.
	  */
	// Allocate two 4 byte integers
	GdtDescriptionStructure structure;
	structure.sizeof_gdt = sizeof(GlobalDescriptorTable) - 1;
	structure.addressof_gdt = (uint32_t) this;

	asm volatile ("lgdt (%0)": :"p" ((uint8_t*) &structure));
}

GlobalDescriptorTable::~GlobalDescriptorTable() {
	// Destructor
}


uint16_t GlobalDescriptorTable::GetDataSegmentOffset(){
	return (uint8_t*)&data_segment_selector_ - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::GetCodeSegmentOffset(){
	return (uint8_t*)&code_segment_selector_ - (uint8_t*)this;
}

/**
	All of the mumbo jumbo inside the function is to shove the arguments into the
	different places where they need to go. Like all bad code, this needs to be
	refactored.
  */
SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access_byte) {
	uint8_t* target = (uint8_t*) this;

	// If 16-bit address
	if(limit <= 65536) {
		// Set the Sz bit to 1, specifying that
		// we are going to use the 32 bit protected mode
		// but will have Gr - granularity - to one byte.
		target[6] = 0x40;
	} else {
		// If not 16-bit address, we will
		// Set the 'Sz' and 'Gr' bits to 1
		// which would mean that we want this segment
		// to be adressed using protected mode, and that
		// we'll be using a granularity of 4Kib
		target[6] = 0xC0;

		// if the last 12 bits of limit are not all 1
		// then shift limit 12 places, and subtract 1
		// that will mean that the last 12 bits are all 1.
		if((limit & 0xFFF) != 0xFFF) {
			limit = (limit >> 12) -1;
		} else {
			limit = limit >> 12;
		}

	}

	// limit_lo
	target[0] = limit & 0xFF;
	target[1] = (limit >> 8) & 0xFF;
	// limit_hi
	target[6] |= (limit >> 16) & 0xF;

	// base_lo
	target[2] = base & 0xFF;
	target[3] = (base >> 8) & 0xFF;
	// base_mid
	target[4] = (base >> 16) & 0xFF;
	// base_hi
	target[7] = (base >> 24) & 0xFF;

	// access_byte
	target[5] = access_byte;
}


uint32_t SegmentDescriptor::GetBase() {
	uint8_t* target = (uint8_t*) this;
	uint32_t result = target[7];
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];
}


uint32_t SegmentDescriptor::GetLimit() {
	uint8_t* target = (uint8_t*) this;
	uint32_t result = target[6] & 0xF;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];

	if((target[6] & 0xC0) == 0xC0) {
		result = (result << 12) | 0xFFF;
	}
}
