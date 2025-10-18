#include "server.h"

void modebus_read_discrete_inputs(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{   
    // Parse request
    const uint16_t startingAddress = modbus_output_address(frame);
    const uint16_t quantity        = modbus_quantity_of_registers(frame);
    // Basic size check for request
    if ((quantity == 0u) || (frameSize != 8u) || ((uint32_t)startingAddress + (uint32_t)quantity > (uint32_t)numOfDiscreteInputs)){
            exceptionResponse(frame, MB_FUNCTION_READ_DISCRETE_INPUTS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
            return; 
    }
    // Address bounds against input_discretes array
    if (startingAddress >= numOfDiscreteInputs) {
        exceptionResponse(frame, MB_FUNCTION_READ_DISCRETE_INPUTS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    
    // Compute response payload size in bytes (ceil(quantity / 8))
    const uint8_t byteCount = (uint8_t)((quantity + 7u) >> 3);

    // Build response
    frame[MODBUS_POS_FUNCTION] = MB_FUNCTION_READ_DISCRETE_INPUTS;
    frame[MODBUS_POS_PDU]     = byteCount;           // byte count at position 2 for read responses
    uint8_t outIndex = MODBUS_POS_FUNCTION + 2;       // start writing packed data at frame[3]

    uint16_t remaining = quantity;
    uint16_t srcIndex  = startingAddress;

    for (uint8_t b = 0; b < byteCount; ++b) {
        uint8_t packed = 0;
        uint8_t bitsThisByte = (remaining >= 8u) ? 8u : (uint8_t)remaining;

        for (uint8_t bit = 0; bit < bitsThisByte; ++bit) {
            // Pack LSB-first: input at srcIndex goes to bit position 'bit'
            const uint8_t val = input_discretes[srcIndex++] ? 1u : 0u;
            packed |= (uint8_t)(val << bit);
        }

        frame[outIndex++] = packed;
        remaining = (uint16_t)(remaining - bitsThisByte);
    }

    // Response size: ID(1) + FUNC(1) + BYTE_CNT(1) + DATA(byteCount) + CRC(2)
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u);
    modbus_write_crc16_le(frame, responseSize);
    sendPacket(frame, responseSize);
}
