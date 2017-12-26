#include "interrupts.h"
#include "printf.h"

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

// Setup PICs (Programmable Interrupt Controller)s.
inline void SetupPics(
	Port8BitSlow pic_master_command,
	Port8BitSlow pic_master_data,
	Port8BitSlow pic_slave_command,
	Port8BitSlow pic_slave_data
) {
	// The CPU internally uses interrupt 0-31 for Exceptions
	// So in order to avoid that we are offsetting the master
	// and slave PICs by 32 (0x20) and 40 (0x28) respectively. Every PIC has
	// 8 interrupts so the master is from 32-39, and the slave
	// will fire interrupts from 40-47.
	pic_master_command.Write(0x11);
	pic_slave_command.Write(0x11);
	
	pic_master_data.Write(0x20);
	pic_slave_data.Write(0x28);

	pic_master_data.Write(0x04);
	pic_slave_data.Write(0x02);

	pic_master_data.Write(0x01);
	pic_slave_data.Write(0x01);

	pic_master_data.Write(0x00);
	pic_slave_data.Write(0x00);


}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) 
: pic_master_command(0x20),
  pic_master_data(0x21),
  pic_slave_command(0xA0),
  pic_slave_data(0xA1)
{
	uint16_t code_segment = gdt->GetCodeSegmentOffset();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;

	// Initialize everything with ignore interrupt handler
	for(uint16_t i=0; i<256; i++)
		SetInterruptDescriptorTableEntry(i, code_segment, &IgnoreInterrupt, 0, IDT_INTERRUPT_GATE);

	// Register interrupts explicitly.
	SetInterruptDescriptorTableEntry(0x20, code_segment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, code_segment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

	// Setup PICs
	SetupPics(
		pic_master_command,
		pic_master_data,
		pic_slave_command,
		pic_slave_data
	);

	// load the idt.
	InterruptDescriptorTablePointer idt_pointer;
	idt_pointer.size = 256 * sizeof(GateDescriptor) -1;
	idt_pointer.base = (uint32_t) interrupt_descriptor_table;
	asm volatile("lidt %0": : "m" (idt_pointer));
}

InterruptManager::~InterruptManager(){
}

void InterruptManager::Activate() {
	asm("sti");
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt_number, uint32_t esp) {
	printf(" GOT INTERRUPT ");
	return esp;
}
