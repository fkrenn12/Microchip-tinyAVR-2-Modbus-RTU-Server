#include "server.h"

void modbus_write_single_holding_register(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    const uint16_t outputAddress = modbus_output_address(frame);
    const uint16_t rawValue = modbus_output_value(frame);
    // Address bounds check
    if (outputAddress >= (uint16_t)numOfHoldingRegisters) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    // Strict request size check
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Perform write
    holding_registers[outputAddress] = rawValue;
    // Build response: echo ID..VALUE and append CRC
    modbus_write_crc16_le(frame, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_REGISTER);
    if (!broadcastFlag) sendPacket(frame, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_REGISTER);
    return 0u;
}
