#include <stdint.h>
#include "hardware/port/port.h"
#include "hardware/ps2/ps2.h"
#include "io/vga/vga.h"

/* 
 * Initialises the PS2 controller 
 */
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