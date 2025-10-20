#include "server.h"
extern Modbus modbus;
void modbus_read_input_registers()
{
    // Parse request
    const uint16_t startingAddress = modbus_output_address(modbus.buffer);
    const uint16_t registerCount  = modbus_quantity_of_registers(modbus.buffer);
    // Strict request size
    if (modbus.actual_size != 8u) {
        exceptionResponse( MB_FUNCTION_READ_INPUT_REGISTERS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Bounds checks with overflow-safe math against input_registers table
    if (startingAddress >= (uint16_t)modbus.input.count) {
        exceptionResponse(MB_FUNCTION_READ_INPUT_REGISTERS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((registerCount == 0u) || ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)modbus.input.count)) {
        exceptionResponse( MB_FUNCTION_READ_INPUT_REGISTERS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); 
    for (uint16_t index = startingAddress, end = (uint16_t)(startingAddress + registerCount);index < end;++index) {
        const uint16_t reg = modbus.input.registers[index];
        modbus.buffer[out++] = (uint8_t)(reg >> 8);
        modbus.buffer[out++] = (uint8_t)(reg & 0xFFu);
    }
    // Build response: ID, FUNC, BYTE_CNT, DATA..., CRC
    const uint8_t byteCount = (uint8_t)(registerCount * 2u);
    modbus.buffer[MODBUS_POS_PDU] = byteCount;
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u); // 1(ID)+1(FUNC)+1(BYTE_CNT)+N(DATA)+2(CRC)
    modbus_write_crc16_le(modbus.buffer, responseSize);
    modbus_send(modbus.buffer, responseSize);
}
