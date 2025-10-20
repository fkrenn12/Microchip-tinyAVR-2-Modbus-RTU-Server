#include "server.h"
extern Modbus modbus;

void modbus_read_holding_registers(uint8_t isConfig)
{
    // Parse request
    uint16_t offset = (isConfig)?MODBUS_CONFIGURATION_START:0;
    uint16_t count = (isConfig)?modbus.config.count:modbus.holding.count;
    uint16_t* registers = (isConfig)?modbus.config.registers:modbus.holding.registers;

    const uint16_t startingAddress = modbus_output_address(modbus.buffer) - offset;
    const uint16_t registerCount  = modbus_quantity_of_registers(modbus.buffer);
    // Strict request size
    if ((registerCount == 0u) || (modbus.actual_size != 8u) || ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)count)) {
        exceptionResponse(MB_FUNCTION_READ_HOLDING_REGISTERS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return; 
    }

    // Bounds checks with overflow-safe math
    if (startingAddress >= (uint16_t)count) {
        exceptionResponse(MB_FUNCTION_READ_HOLDING_REGISTERS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Build response
    const uint8_t byteCount = (uint8_t)(registerCount * 2u);
   
    modbus.buffer[MODBUS_POS_PDU]     = byteCount;

    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); // start of data (index 3)
    uint16_t idx = startingAddress;
    const uint16_t end = (uint16_t)(startingAddress + registerCount);

    while (idx < end) {
        const uint16_t reg = registers[idx++];
        modbus.buffer[out++] = (uint8_t)(reg >> 8);
        modbus.buffer[out++] = (uint8_t)(reg & 0xFFu);
    }

    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u); // ID, FUNC, BYTE_CNT, DATA, CRC
    modbus_write_crc16_le(modbus.buffer, responseSize);
    modbus_send(modbus.buffer, responseSize);
}
