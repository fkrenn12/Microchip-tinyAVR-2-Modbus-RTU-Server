#include "server.h"
/**
 * Handles the Modbus "Read Coils" function (Function Code 0x01).
 * Reads a specified number of coil statuses starting at a given address
 * from the Modbus server's coil table and sends the response back to the client.
 *
 * The response includes the coil values packed into bytes, with each bit
 * representing the LSB-first status of a coil. Validates the request parameters such as
 * the starting address, quantity of coils, and overall request size before processing.
 *
 * If the request is invalid (e.g., illegal data value, address exceeds bounds,
 * or improper message size), an appropriate exception response is sent to the client.
 * Builds the response payload, including the byte count, packed coil data,
 * and appends a CRC checksum before sending the response.
 */
extern Modbus modbus;
void modbus_read_coils()
{
    const uint16_t startingAddress = modbus_output_address();
    const uint16_t numOfRegisters = modbus_quantity_of_registers();
    const uint8_t invalidStartAdress = (startingAddress >= modbus.coils.count) ? 1 : 0;
    const uint8_t incorrectPackageSize = (modbus.actual_size == 8u) ? 0 : 1;
    const uint8_t invalidNumberOfRegisters = (uint8_t)((!numOfRegisters) || ((uint32_t)startingAddress + numOfRegisters > modbus.coils.count));
    // Basic size check for request
    uint8_t exception = (invalidNumberOfRegisters || incorrectPackageSize) ? MB_EXCEPTION_ILLEGAL_DATA_VALUE : 0;
    exception = (invalidStartAdress) ? MB_EXCEPTION_ILLEGAL_DATA_ADDRESS : exception;
    if (exception){
        exceptionResponse(MB_FUNCTION_READ_COILS, modbus.broadcastFlag, exception);
        return;
    }
    // Compute response payload size in bytes (ceil(quantity / 8))
    const uint8_t byteCount = (uint8_t)((numOfRegisters + 7u) >> 3);
    // Build response: ID, FUNC, BYTE_CNT, DATA..., CRC
    modbus.buffer[MODBUS_POS_PDU] = byteCount;
    uint8_t outIndex = MODBUS_POS_FUNCTION + 2; // index 3
    uint16_t remaining = numOfRegisters;
    uint16_t srcIndex  = startingAddress;

    for (uint8_t b = 0; b < byteCount; ++b) {
        uint8_t packed = 0;
        const uint8_t bitsThisByte = (remaining >= 8u) ? 8u : (uint8_t)remaining;

        for (uint8_t bit = 0; bit < bitsThisByte; ++bit) {
            // LSB-first: coil[srcIndex] -> bit position 'bit'
            const uint8_t val = modbus.coils.registers[srcIndex++] ? 1u : 0u;
            packed |= (uint8_t)(val << bit);
        }
        modbus.buffer[outIndex++] = packed;
        remaining = (uint16_t)(remaining - bitsThisByte);
    }

    // Finalize CRC and send
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u);
    modbus_write_crc16_le(modbus.buffer, responseSize);
    modbus_send(modbus.buffer, responseSize);
}
