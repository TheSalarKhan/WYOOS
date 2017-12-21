#include "port.h"

PortBase::PortBase(uint16_t port_number) : port_number_(port_number) { }
PortBase::~PortBase() {}

Port8Bit::Port8Bit(uint16_t port_number) : PortBase(port_number) { }
Port8Bit::~Port8Bit() {}
void Port8Bit::Write(uint8_t data) {
	__asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (this->port_number_));
}
uint8_t Port8Bit::Read() {
	uint8_t result;
	__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (this->port_number_));
	return result;
}


Port8BitSlow::Port8BitSlow(uint16_t port_number) : Port8Bit(port_number) { }
Port8BitSlow::~Port8BitSlow() {}
void Port8BitSlow::Write(uint8_t data) {
	// A Write followed by two jumps to waste time. -_-
	__asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (this->port_number_));
}


Port16Bit::Port16Bit(uint16_t port_number) : PortBase(port_number) { }
Port16Bit::~Port16Bit() {}
void Port16Bit::Write(uint16_t data) {
	__asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (this->port_number_));
}
uint16_t Port16Bit::Read() {
	uint16_t result;
	__asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (this->port_number_));
	return result;
}


Port32Bit::Port32Bit(uint16_t port_number) : PortBase(port_number) { }
Port32Bit::~Port32Bit() {}
void Port32Bit::Write(uint32_t data) {
	__asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (this->port_number_));
}
uint32_t Port32Bit::Read() {
	uint32_t result;
	__asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (this->port_number_));
	return result;
}
