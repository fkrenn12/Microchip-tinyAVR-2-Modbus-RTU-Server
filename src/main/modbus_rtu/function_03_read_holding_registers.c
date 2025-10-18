#include "server.h"

void modbus_read_holding_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag, uint16_t* registers, uint8_t numOfRegisters, uint16_t offset)
{
    // Parse request
    const uint16_t startingAddress = modbus_output_address(frame) - offset;
    const uint16_t registerCount  = modbus_quantity_of_registers(frame);
    // Strict request size
    if ((registerCount == 0u) || (frameSize != 8u) || ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)numOfRegisters)) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return; 
    }

    // Bounds checks with overflow-safe math
    if (startingAddress >= (uint16_t)numOfRegisters) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Build response
    const uint8_t byteCount = (uint8_t)(registerCount * 2u);
   
    frame[MODBUS_POS_PDU]     = byteCount;

    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); // start of data (index 3)
    uint16_t idx = startingAddress;
    const uint16_t end = (uint16_t)(startingAddress + registerCount);

    while (idx < end) {
        const uint16_t reg = registers[idx++];
        frame[out++] = (uint8_t)(reg >> 8);
        frame[out++] = (uint8_t)(reg & 0xFFu);
    }

    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u); // ID, FUNC, BYTE_CNT, DATA, CRC
    modbus_write_crc16_le(frame, responseSize);
    sendPacket(frame, responseSize);
}
