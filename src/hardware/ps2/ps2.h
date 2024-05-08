#ifndef PS2_H
#define PS2_H

// PS/2 Ports
#define PS2_DATA    0x60
#define PS2_STATUS  0x64
#define PS2_COMMAND PS2_STATUS

// Common PS/2 commands
#define GET_PS2_CONTROLLER_CONFIGURATION_BYTE 0x20
#define SET_PS2_CONTROLLER_CONFIGURATION_BYTE 0x60

#define TEST_PS2_CONTROLLER                   0xAA
#define PS2_CONTROLLER_TEST_SUCCESS           0x55

#define ENABLE_FIRST_PS2_CHANNEL              0xAE
#define DISABLE_FIRST_PS2_CHANNEL             0xAD
#define TEST_FIRST_PS2_CHANNEL                0xAB
#define FIRST_PS2_CHANNEL_TEST_SUCCESS        0x00

#define ENABLE_SECOND_PS2_CHANNEL             0xA8
#define DISABLE_SECOND_PS2_CHANNEL            0xA7
#define TEST_SECOND_PS2_CHANNEL               0xA9
#define SECOND_PS2_CHANNEL_TEST_SUCCESS       FIRST_PS2_CHANNEL_TEST_SUCCESS

void initialise_ps2();

#endif