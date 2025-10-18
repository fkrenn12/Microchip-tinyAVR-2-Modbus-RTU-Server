#include "server.h"

void modbus_read_input_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    // Parse request
    const uint16_t startingAddress = modbus_output_address(frame);
    const uint16_t registerCount  = modbus_quantity_of_registers(frame);
    // Strict request size
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_READ_INPUT_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Bounds checks with overflow-safe math against input_registers table
    if (startingAddress >= (uint16_t)numOfInputRegisters) {
        exceptionResponse(frame, MB_FUNCTION_READ_INPUT_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((registerCount == 0u) || ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)numOfInputRegisters)) {
        exceptionResponse(frame, MB_FUNCTION_READ_INPUT_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); 
    for (uint16_t index = startingAddress, end = (uint16_t)(startingAddress + registerCount);index < end;++index) {
        const uint16_t reg = input_registers[index];
        frame[out++] = (uint8_t)(reg >> 8);
        frame[out++] = (uint8_t)(reg & 0xFFu);
    }
    // Build response: ID, FUNC, BYTE_CNT, DATA..., CRC
    const uint8_t byteCount = (uint8_t)(registerCount * 2u);
    frame[MODBUS_POS_PDU] = byteCount;
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u); // 1(ID)+1(FUNC)+1(BYTE_CNT)+N(DATA)+2(CRC)
    modbus_write_crc16_le(frame, responseSize);
    sendPacket(frame, responseSize);
    return 0u;
}
