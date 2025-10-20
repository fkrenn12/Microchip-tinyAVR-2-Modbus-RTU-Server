#include "server.h"
extern Modbus modbus;
void modbus_write_single_coil()
{
    // Parse request fields
    const uint16_t outputAddress = modbus_output_address(modbus.buffer);
    const uint16_t rawValue = modbus_output_value(modbus.buffer);
    // Address check
    if (outputAddress >= (uint16_t)modbus.coils.count) {
        exceptionResponse( MB_FUNCTION_WRITE_SINGLE_COIL, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    // Modbus spec: 0xFF00 = ON, 0x0000 = OFF (other values illegal) 
    if ((modbus.actual_size != 8u) || (rawValue != MODBUS_COIL_ON_VALUE && rawValue != MODBUS_COIL_OFF_VALUE)) {
        exceptionResponse(MB_FUNCTION_WRITE_SINGLE_COIL, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Write coil (1 for ON, 0 for OFF)
    modbus.coils.registers[outputAddress] = (uint8_t)(rawValue == MODBUS_COIL_ON_VALUE ? 1u : 0u);
    // Response: echo request (ID..VALUE) + CRC
    modbus_write_crc16_le(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_COIL);
    if (!modbus.broadcastFlag) modbus_send(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_SINGLE_COIL);
}
