#include "keyboard.h"
#include "printf.h"

/**
    The command and data ports for the keyboard are 0x60 and 0x64 respectively.
*/
KeyboardDriver::KeyboardDriver(InterruptManager* interrupt_manager) :
InterruptHandler(0x21, interrupt_manager),
data_port_(0x60),
command_port_(0x64)
{
    /**
        Flush all the key strikes.
    */
    while(this->command_port_.Read() & 0x1) {
        this->data_port_.Read();
    }

    this->command_port_.Write(0xAE); // Activate interrupts
    this->command_port_.Write(0x20); // get curent state

    // Read status, set the 1st bit and clear the 5th bit.
    uint8_t status = (this->data_port_.Read() | 1) & ~0x10;

    this->command_port_.Write(0x60); // set state
    this->data_port_.Write(status);
    this->data_port_.Write(0xF4);

    printf("Keyboard driver initialized.");
}

KeyboardDriver::~KeyboardDriver() {

}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    uint8_t key = this->data_port_.Read();
    const char* hex_digits = "0123456789ABCDEF";
	char message[] = "Key: 0x00";
	message[7] = hex_digits[(key >> 4) & 0x0F];
	message[8] = hex_digits[key & 0x0F];
	printf(message);
    return esp;
}
