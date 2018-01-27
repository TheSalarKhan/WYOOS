#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "types.h"
#include "interrupts.h"
#include "port.h"

class KeyboardDriver : public InterruptHandler {
    Port8Bit data_port_;
    Port8Bit command_port_;

public:
    KeyboardDriver(InterruptManager* interrupt_manager);
    ~KeyboardDriver();
    uint32_t HandleInterrupt(uint32_t esp);

};

#endif
