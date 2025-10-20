#include "server.h"
extern Modbus modbus;
/**
 * Handles the Modbus function to write a single holding register.
 * Extracts the target register address and the value to be written from the Modbus buffer.
 * Performs bounds checking on the specified address against the available holding registers.
 * Verifies the request packet size for correctness to ensure proper formatting.
 * Writes the given value into the specified holding register if checks are passed.
 * Builds a response containing the echoed address/value and appends CRC for validation.
 * Sends the response unless it is a broadcast request, where no response is required.
 */
void modbus_write_single_holding_register()
{
    const uint16_t outputAddress = modbus_output_address(modbus.buffer);
    const uint16_t rawValue = modbus_output_value(modbus.buffer);
    // Address bounds check
    if (outputAddress >= (uint16_t)modbus.holding.count) {
        exceptionResponse( MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    // Strict request size check
    if (modbus.actual_size != 8u) {
        exceptionResponse( MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Perform write
    modbus.holding.registers[outputAddress] = rawValue;
    // Build response: echo ID..VALUE and append CRC
    modbus_write_crc16_le(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_REGISTER);
    if (!modbus.broadcastFlag) modbus_send(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_REGISTER);
}
