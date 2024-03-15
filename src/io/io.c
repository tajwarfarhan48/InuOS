#include "io/io.h"
#include "kernel.h"

static inline void io_wait(void)
{
    outb(0x80, 0);
}

void pic_remap(uint8_t master_offset, uint8_t slave_offset)
{
    uint8_t master_mask, slave_mask;

    // Save the interrupt masks
    master_mask = insb(PIC1_DATA);
    slave_mask = insb(PIC2_DATA);

    // Give the initialization command to the PICs
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    
    // Give the vector offsets to the PICs
    outb(PIC1_DATA, master_offset);
    io_wait();
    outb(PIC2_DATA, slave_offset);
    io_wait();

    // Tell the master and slave PICs how they are wired to each other
    outb(PIC1_DATA, 0b00000100);  // Tell master PIC there is a slave at IRQ 2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 0b00000010);  // Tell slave PIC its cascade identity (0000 0010)
    io_wait();

    // Give additional information about the environment
    outb(PIC1_DATA, ICW4_8086);  // Have the PICs use 8086 mode (not 8080)
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Restore the interrupt masks
    outb(PIC1_DATA, master_mask);
    outb(PIC2_DATA, slave_mask);
}

void initialise_ps2()
{
	uint8_t configurationByte, controllerTestResult, firstChannelTestResult, secondChannelTestResult;
	int secondChannelPresent;

	// Disable both PS/2 devices
	outb(PS2_COMMAND, DISABLE_FIRST_PS2_CHANNEL);
	outb(PS2_COMMAND, DISABLE_SECOND_PS2_CHANNEL);
	
	// Flush output buffer
	insb(PS2_DATA);
	
	// Get configuration byte
	outb(PS2_COMMAND, GET_PS2_CONTROLLER_CONFIGURATION_BYTE);
	while (!(insb(PS2_STATUS) & 0x01));  // Wait until output buffer is full
	configurationByte = insb(PS2_DATA);
	
	// Disable translation and disable interrupts on both PS/2 devices
	configurationByte &= 0b10111100;

	// Write back configuration byte
	outb(PS2_COMMAND, SET_PS2_CONTROLLER_CONFIGURATION_BYTE);
	while (insb(PS2_STATUS) & 0x02);  // Wait until the input buffer is empty
	outb(PS2_DATA, configurationByte);

	// Perform PS/2 controller self test
	outb(PS2_COMMAND, TEST_PS2_CONTROLLER);
	while(!(insb(PS2_STATUS) & 0x01)); // Wait until output buffer is populated with self test result
	controllerTestResult = insb(PS2_DATA);

	// Check if PS/2 controller passed self test
	if (controllerTestResult != PS2_CONTROLLER_TEST_SUCCESS) {
		print("Error: PS/2 controller failed self test. Aborting initialisation...\n");
		return;
	}

	// Restore initial configuration byte (self test resets it on some architectures)
	outb(PS2_COMMAND, SET_PS2_CONTROLLER_CONFIGURATION_BYTE);
	while (insb(PS2_STATUS) & 0x02);  // Wait until the input buffer is empty	
	outb(PS2_DATA, configurationByte);

	// Check for 2nd PS/2 channel/device
	outb(PS2_COMMAND, ENABLE_SECOND_PS2_CHANNEL);
	outb(PS2_COMMAND, GET_PS2_CONTROLLER_CONFIGURATION_BYTE);
	while (!(insb(PS2_STATUS) & 0x01)); // Wait until output buffer is full
	configurationByte = insb(PS2_DATA);
	
	// Check if bit 5 is low (i.e., second channel clock signal is enabled)
	secondChannelPresent = !(configurationByte & 0x20);

    if (secondChannelPresent) {
        print("Second channel is present!\n");
    }

	// Test first PS/2 channel
	outb(PS2_COMMAND, TEST_FIRST_PS2_CHANNEL);
	while (!(insb(PS2_STATUS) & 0x01)); // Wait until output buffer is full
	firstChannelTestResult = insb(PS2_DATA);

	if (firstChannelTestResult != FIRST_PS2_CHANNEL_TEST_SUCCESS)
		print("First PS/2 channel test failed...");

	// Test second PS/2 channel
	if (secondChannelPresent) {
		outb(PS2_COMMAND, TEST_SECOND_PS2_CHANNEL);
		while (!(insb(PS2_STATUS) & 0x01)); // Wait until output buffer is full
		secondChannelTestResult = insb(PS2_DATA);

		if (secondChannelTestResult != SECOND_PS2_CHANNEL_TEST_SUCCESS)
			print("Second PS/2 channel test failed...");
	}

	// Get configuration byte
	outb(PS2_COMMAND, GET_PS2_CONTROLLER_CONFIGURATION_BYTE);
	while (!(insb(PS2_STATUS) & 0x01));  // Wait until output buffer is full
	configurationByte = insb(PS2_DATA);
	
	// Enable interrupts on both PS/2 channels
	configurationByte |= 0x03;

	// Write back configuration byte
	outb(PS2_COMMAND, SET_PS2_CONTROLLER_CONFIGURATION_BYTE);
	while (insb(PS2_STATUS) & 0x02);  // Wait until the input buffer is empty
	outb(PS2_DATA, configurationByte);

    // Re-enable PS/2 channels
    outb(PS2_COMMAND, ENABLE_FIRST_PS2_CHANNEL);
    outb(PS2_COMMAND, ENABLE_SECOND_PS2_CHANNEL);

	return;
}