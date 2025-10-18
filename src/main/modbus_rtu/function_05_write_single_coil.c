#include "server.h"

void modbus_write_single_coil(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    // Parse request fields
    const uint16_t outputAddress = modbus_output_address(frame);
    const uint16_t rawValue = modbus_output_value(frame);
    // Address check
    if (outputAddress >= (uint16_t)numOfCoils) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_COIL, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    // Modbus spec: 0xFF00 = ON, 0x0000 = OFF (other values illegal) 
    if ((frameSize != 8u) || (rawValue != MODBUS_COIL_ON_VALUE && rawValue != MODBUS_COIL_OFF_VALUE)) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_COIL, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Write coil (1 for ON, 0 for OFF)
    coils[outputAddress] = (uint8_t)(rawValue == MODBUS_COIL_ON_VALUE ? 1u : 0u);
    // Response: echo request (ID..VALUE) + CRC
    modbus_write_crc16_le(frame, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_COIL);
    if (!broadcastFlag) sendPacket(frame, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_COIL);
}
