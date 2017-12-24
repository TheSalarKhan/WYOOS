#include "interrupts.h"
void printf(const char* str);

InterruptManager::GateDescriptor InterruptManager::interrupt_descriptor_table[256];

void InterruptManager::SetInterruptDescriptorTableEntry(
		uint8_t interrupt_number,
		uint16_t gdt_code_segment_selector,
		void (*handler)(),
		uint8_t descriptor_priviledge_level,
		uint8_t descriptor_type
	)
{
	const uint8_t IDT_DESC_PRESENT = 0x80;
	interrupt_descriptor_table[interrupt_number].handler_address_lo = ((uint32_t)handler) & 0xFFFF;
	interrupt_descriptor_table[interrupt_number].handler_address_hi = (((uint32_t)handler) >> 16) & 0xFFFF;
	interrupt_descriptor_table[interrupt_number].gdt_code_segment_selector = gdt_code_segment_selector;
	interrupt_descriptor_table[interrupt_number].access_rights = IDT_DESC_PRESENT | descriptor_type | ((descriptor_priviledge_level&3) << 5);
	interrupt_descriptor_table[interrupt_number].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt){
	uint16_t code_segment = gdt->GetCodeSegmentOffset();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;

	// Initialize everything with ignore interrupt handler
	for(uint16_t i=0; i<256; i++)
		SetInterruptDescriptorTableEntry(i, code_segment, &IgnoreInterrupt, 0, IDT_INTERRUPT_GATE);

	// Register interrupts explicitly.
	SetInterruptDescriptorTableEntry(0x20, code_segment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, code_segment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

	// load the idt.
	InterruptDescriptorTablePointer idt;
	idt.size = 256 * sizeof(GateDescriptor) -1;
	idt.base = (uint32_t) interrupt_descriptor_table;
	asm volatile("lidt %0": : "m" (idt));
}

InterruptManager::~InterruptManager(){
	asm("sti");
}

void InterruptManager::Activate() {
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt_number, uint32_t esp) {
	printf(" INTERRUPT ");

	return esp;
}
