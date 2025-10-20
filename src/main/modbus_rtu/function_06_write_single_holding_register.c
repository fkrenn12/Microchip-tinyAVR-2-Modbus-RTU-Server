#include "server.h"
extern Modbus modbus;
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
