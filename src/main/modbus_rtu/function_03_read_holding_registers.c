#include "server.h"
/**
 * Handles the Modbus function code for reading holding registers.
 *
 * @param isConfig Specifies whether configuration registers or holding
 *                 registers are read. Pass 1 for configuration registers
 *                 or 0 for standard holding registers.
 *
 * This function parses the request received via the Modbus protocol,
 * determines the starting address and number of registers to read,
 * performs strict size and bounds checks to ensure the request is
 * valid, and generates an appropriate response.
 *
 * If the request is invalid (due to out-of-bound addresses, invalid
 * register count, or incorrect packet size), an exception response
 * is sent and the function terminates early to prevent errors.
 *
 * For valid requests, the function builds a response containing the
 * requested register values, calculates the CRC checksum, and sends
 * the response over the Modbus interface.
 */
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
