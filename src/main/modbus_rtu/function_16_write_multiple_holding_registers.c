#include "server.h"
extern Modbus modbus;
/**
 * Handles the Modbus Write Multiple Holding Registers (function code 16) request.
 *
 * This function writes multiple holding registers in the Modbus context (`Modbus* mb`)
 * based on the received request in the Modbus buffer.
 *
 * If `isConfig` is true, the function accesses and writes configuration registers;
 * otherwise, it operates on holding registers.
 *
 * - Validates the request frame size, byte count consistency, and address range
 *   according to the Modbus protocol specifications.
 * - Uses the starting address and number of registers to determine the write location.
 * - Decodes the data from the request payload and stores it in the appropriate register array.
 *
 * In case of an invalid request (e.g., address out of range, incorrect data format),
 * a Modbus exception response is sent using the appropriate exception code.
 *
 * For valid requests, the function builds and sends a response frame containing
 * the starting address and number of registers written. If operating in broadcast mode,
 * no response is sent.
 *
 * @param mb Pointer to the Modbus structure containing context and buffers.
 * @param isConfig Flag indicating whether to operate on configuration registers
 *                 (`true`) or regular holding registers (`false`).
 */
void modbus_write_multiple_holding_registers(uint8_t isConfig)
{
    uint16_t offset = (isConfig)?MODBUS_CONFIGURATION_START:0;
    uint16_t count = (isConfig)?modbus.config.count:modbus.holding.count;
    uint16_t* registers = (isConfig)?modbus.config.registers:modbus.holding.registers;
    // Parse request
    const uint16_t startingAddress = modbus_output_address(modbus.buffer) - offset;
    const uint16_t registerCount  = modbus_quantity_of_registers(modbus.buffer);
    const uint8_t  byteCount      = modbus.buffer[MODBUS_POS_BYTECOUNT];
    const uint16_t expectedBytes = (uint16_t)(registerCount * 2u);
    const uint16_t expectedFrame = (uint16_t)(9u + (uint16_t)byteCount);
    // Minimal fixed part is 9 bytes before CRC depends on byte count; validate later precisely.
    if ((modbus.actual_size < 9u) || (registerCount == 0u) || (byteCount != (uint8_t)expectedBytes) || (modbus.actual_size != expectedFrame) 
        || ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)count)){
        exceptionResponse(MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }    
    // Address range checks
    if (startingAddress >= (uint16_t)count) {
        exceptionResponse( MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Write registers from payload
    uint8_t dataIndex = MODBUS_POS_REGISTERVALUES; // first data byte after byteCount
    uint16_t dst = startingAddress;
    for (uint16_t i = 0; i < registerCount; ++i) {
        const uint16_t value = (uint16_t)(((uint16_t)modbus.buffer[dataIndex] << 8) | (uint16_t)modbus.buffer[(uint8_t)(dataIndex + 1u)]);
        registers[dst++] = value;
        dataIndex = (uint8_t)(dataIndex + 2u);
    }

    // Build response: echo ID..COUNT (first 6 bytes) + CRC
    modbus_write_crc16_le(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_HOLDING_REGISTERS);
    if (!modbus.broadcastFlag) modbus_send(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_HOLDING_REGISTERS);
}
