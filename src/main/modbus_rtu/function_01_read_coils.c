#include "server.h"
extern Modbus modbus;
void modbus_read_coils()
{
    const uint16_t startingAddress = modbus_output_address(modbus.buffer);
    const uint16_t quantity        = modbus_quantity_of_registers(modbus.buffer);
    // Basic size check for request
    if ((quantity == 0u) || ((uint32_t)startingAddress + (uint32_t)quantity > (uint32_t)modbus.coils.count) || (modbus.actual_size != 8u)) {
        exceptionResponse(MB_FUNCTION_READ_COILS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Address bounds against coils table
    if (startingAddress >= modbus.coils.count) {
        exceptionResponse( MB_FUNCTION_READ_COILS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Compute response payload size in bytes (ceil(quantity / 8))
    const uint8_t byteCount = (uint8_t)((quantity + 7u) >> 3);

    // Build response: ID, FUNC, BYTE_CNT, DATA..., CRC
    modbus.buffer[MODBUS_POS_FUNCTION] = MB_FUNCTION_READ_COILS;
    modbus.buffer[MODBUS_POS_PDU]     = byteCount;

    uint8_t outIndex = MODBUS_POS_FUNCTION + 2; // index 3
    uint16_t remaining = quantity;
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
