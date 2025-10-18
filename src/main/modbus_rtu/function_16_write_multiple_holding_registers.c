#include "server.h"

void modbus_write_multiple_holding_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag, uint16_t* registers, uint8_t numOfRegisters, uint16_t offset)
{
    // Parse request
    const uint16_t startingAddress = modbus_output_address(frame) - offset;
    const uint16_t registerCount  = modbus_quantity_of_registers(frame);
    const uint8_t  byteCount      = frame[MODBUS_POS_BYTECOUNT];
    const uint16_t expectedBytes = (uint16_t)(registerCount * 2u);
    const uint16_t expectedFrame = (uint16_t)(9u + (uint16_t)byteCount);
    // Minimal fixed part is 9 bytes before CRC depends on byte count; validate later precisely.
    if ((frameSize < 9u) || (registerCount == 0u) || (byteCount != (uint8_t)expectedBytes) || (frameSize != expectedFrame) 
        || ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)numOfRegisters)){
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }    
    // Address range checks
    if (startingAddress >= (uint16_t)numOfRegisters) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Write registers from payload
    uint8_t dataIndex = MODBUS_POS_REGISTERVALUES; // first data byte after byteCount
    uint16_t dst = startingAddress;
    for (uint16_t i = 0; i < registerCount; ++i) {
        const uint16_t value = (uint16_t)(((uint16_t)frame[dataIndex] << 8) | (uint16_t)frame[(uint8_t)(dataIndex + 1u)]);
        registers[dst++] = value;
        dataIndex = (uint8_t)(dataIndex + 2u);
    }

    // Build response: echo ID..COUNT (first 6 bytes) + CRC
    modbus_write_crc16_le(frame, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_HOLDING_REGISTERS);
    if (!broadcastFlag) sendPacket(frame, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_HOLDING_REGISTERS);
}
