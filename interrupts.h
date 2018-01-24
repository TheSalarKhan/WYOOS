#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "global_descriptor_table.h"
// forward declaring InterruptManager
class InterruptManager;

class InterruptHandler {
protected:
	// The interrupt number for this interrupt handler.
	uint8_t interrupt_number_;
	// The interrupt manager that this interrupt
	// handler is connected to.
	InterruptManager* interrupt_manager_;

	// We have made the *tructors protected so that noone
	// can instantiate the objects for this class.
	InterruptHandler(uint8_t interrupt_number, InterruptManager* interrupt_manager);
	~InterruptHandler();
public:
	uint32_t HandleInterrupt(uint32_t esp);
};

class InterruptManager {
friend class InterruptHandler;
protected:
	static InterruptManager* ActiveInterruptManager;
	InterruptHandler* handlers[256];
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
	
	Port8BitSlow pic_master_command;
	Port8BitSlow pic_master_data;
	Port8BitSlow pic_slave_command;
	Port8BitSlow pic_slave_data;
public:
	InterruptManager(GlobalDescriptorTable* gdt);
	~InterruptManager();
	void Activate();
	void Deactivate();
	static uint32_t HandleInterrupt(uint8_t interrupt_number, uint32_t esp);
	uint32_t HandleInterruptNonStatic(uint8_t interrupt_number, uint32_t esp);

	static void IgnoreInterrupt();
	static void HandleInterruptRequest0x00();
	static void HandleInterruptRequest0x01();

};

#endif
