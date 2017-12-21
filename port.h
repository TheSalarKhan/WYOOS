#ifndef __PORT_H
#define __PORT_H

#include "types.h"

class PortBase {
protected:
	uint16_t port_number_;
	PortBase(uint16_t port_number);
	~PortBase();
};


class Port8Bit : public PortBase {
public:
	Port8Bit(uint16_t port_number);
	~Port8Bit();
	virtual void Write(uint8_t data);
	virtual uint8_t Read();
};

class Port8BitSlow : public Port8Bit {
public:
	Port8BitSlow(uint16_t port_number);
	~Port8BitSlow();
	virtual void Write(uint8_t data);
};

class Port16Bit : public PortBase {
public:
	Port16Bit(uint16_t port_number);
	~Port16Bit();
	virtual void Write(uint16_t data);
	virtual uint16_t Read();
};

class Port32Bit : public PortBase {
public:
	Port32Bit(uint16_t port_number);
	~Port32Bit();
	virtual void Write(uint32_t data);
	virtual uint32_t Read();
};
#endif
