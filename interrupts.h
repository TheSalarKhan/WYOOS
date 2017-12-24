#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "global_descriptor_table.h"

class InterruptManager {
protected:
	struct GateDescriptor {
		uint16_t handler_address_lo;
		uint16_t gdt_code_segment_selector;
		uint8_t reserved;
		uint8_t access_rights;
		uint16_t handler_address_hi;
	}__attribute__((packed));

	static GateDescriptor interrupt_descriptor_table[256];

	struct InterruptDescriptorTablePointer {
		uint16_t size;
		uint32_t base;
	} __attribute__((packed));

	static void SetInterruptDescriptorTableEntry(
		uint8_t interrupt_number,
		uint16_t gdt_code_segment_selector,
		void (*handler)(),
		uint8_t descriptor_priviledge_level,
		uint8_t descriptor_type
	);
public:
	InterruptManager(GlobalDescriptorTable* gdt);
	~InterruptManager();
	void Activate();
	static uint32_t HandleInterrupt(uint8_t interrupt_number, uint32_t esp);

	static void IgnoreInterrupt();
	static void HandleInterruptRequest0x00();
	static void HandleInterruptRequest0x01();

};

#endif
