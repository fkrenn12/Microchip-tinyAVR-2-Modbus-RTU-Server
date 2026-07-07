#include "server.h"
/**
 * Handles Modbus function code 0x02: Read Discrete Inputs.
 *
 * This function processes a request to read a range of discrete input registers
 * and generates a response containing the binary states of the requested inputs.
 *
 * Steps performed:
 * 1. Parses the starting address and quantity of discrete inputs from the Modbus
 *    request buffer.
 * 2. Validates the request for proper format, size, and address range.
 * 3. Computes the response payload size based on the number of registers requested.
 * 4. Reads the specified discrete inputs and packs their values into bytes in
 *    the response buffer.
 * 5. Calculates the CRC and sends the response to the master.
 *
 * Error handling:
 * - If the request data is invalid (e.g., zero quantity, invalid size, out of bounds),
 *   an exception response is generated and sent.
 *
 * Note:
 * - The packed discrete input values are sent LSB-first in each byte.
 * - The response structure includes the function code, byte count, data, and CRC.
 */
extern Modbus modbus;
void modebus_read_discrete_inputs()
{   
    const uint16_t startingAddress = modbus_output_address();
    const uint16_t numOfRegisters = modbus_quantity_of_registers();
    const uint8_t invalidStartAdress = (startingAddress >= modbus.discretes.count) ? 1 : 0;
    const uint8_t incorrectPackageSize = (modbus.actual_size == 8u) ? 0 : 1;
    const uint8_t invalidNumberOfRegisters = (uint8_t)((!numOfRegisters) || ((uint32_t)startingAddress + numOfRegisters > modbus.discretes.count));
    // Basic size check for request
    uint8_t exception = (invalidNumberOfRegisters || incorrectPackageSize) ? MB_EXCEPTION_ILLEGAL_DATA_VALUE : 0;
    exception = (invalidStartAdress) ? MB_EXCEPTION_ILLEGAL_DATA_ADDRESS : exception;
    if (exception){
        exceptionResponse(MB_FUNCTION_READ_DISCRETE_INPUTS, modbus.broadcastFlag, exception);
        return;
    }
    
    // Compute response payload size in bytes (ceil(quantity / 8))
    const uint8_t byteCount = (uint8_t)((numOfRegisters + 7u) >> 3);

    // Build response
    modbus.buffer[MODBUS_POS_FUNCTION] = MB_FUNCTION_READ_DISCRETE_INPUTS;
    modbus.buffer[MODBUS_POS_PDU] = byteCount;           // byte count at position 2 for read responses
    uint8_t outIndex = MODBUS_POS_FUNCTION + 2;       // start writing packed data at frame[3]

    uint16_t remaining = numOfRegisters;
    uint16_t srcIndex  = startingAddress;

    for (uint8_t b = 0; b < byteCount; ++b) {
        uint8_t packed = 0;
        uint8_t bitsThisByte = (remaining >= 8u) ? 8u : (uint8_t)remaining;

        for (uint8_t bit = 0; bit < bitsThisByte; ++bit) {
            // Pack LSB-first: input at srcIndex goes to bit position 'bit'
            const uint8_t val = modbus.discretes.registers[srcIndex++] ? 1u : 0u;
            packed |= (uint8_t)(val << bit);
        }

        modbus.buffer[outIndex++] = packed;
        remaining = (uint16_t)(remaining - bitsThisByte);
    }

    // Response size: ID(1) + FUNC(1) + BYTE_CNT(1) + DATA(byteCount) + CRC(2)
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u);
    modbus_write_crc16_le(modbus.buffer, responseSize);
    modbus_send(modbus.buffer, responseSize);
}
