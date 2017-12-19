#ifndef __GLOBAL_DESCRIPTOR_TABLE_H
#define __GLOBAL_DESCRIPTOR_TABLE_H

#include "types.h"

// Used in constructor for GlobalDescriptorTable
// when using the lgdt assembly instruction.
struct GdtDescriptionStructure {
	uint16_t sizeof_gdt;
	uint32_t addressof_gdt;
} __attribute__((packed));

/**
	This class defines a SegmentDescrptor which is a single entry in GlobalDescriptorTable.
	for more information read: http://wiki.osdev.org/Global_Descriptor_Table.
*/
class SegmentDescriptor {
	private:
		uint16_t limit_lo_;		// Limit 00:15
		uint16_t base_lo_;		// Base  00:15
		uint8_t base_mid_;		// Base  16:23
		uint8_t access_byte_;		// Access Byte
		uint8_t limit_hi_and_flags_;	// Limit 16:19 - Flags
		uint8_t base_hi_;		// Base  24:31
	public:
		SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access_byte);
		// Getters
		uint32_t GetBase();
		uint32_t GetLimit();
} __attribute__((packed));

class GlobalDescriptorTable {
	public:
		SegmentDescriptor null_segment_selector_;
		SegmentDescriptor unused_segment_selector_;
		SegmentDescriptor code_segment_selector_;
		SegmentDescriptor data_segment_selector_;

		GlobalDescriptorTable();
		~GlobalDescriptorTable();

		// Getters for segment offsets
		uint16_t GetCodeSegmentOffset();
		uint16_t GetDataSegmentOffset();
} __attribute__((packed));

#endif
