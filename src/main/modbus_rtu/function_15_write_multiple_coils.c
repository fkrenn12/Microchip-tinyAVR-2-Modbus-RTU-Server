#include "server.h"
extern Modbus modbus;
/**
 * Handles a Modbus "Write Multiple Coils" request.
 *
 * This function processes incoming Modbus requests to write multiple
 * coils starting at a specific address. It performs the following steps:
 *
 * 1. Parses the request fields, including starting address, coil count,
 *    byte count, and calculates the total expected size of the request.
 *
 * 2. Validates the request by ensuring the byte count matches the coil count,
 *    the coil count is non-zero, the request size is correct, and the address
 *    range is within the valid bounds of the coil register array.
 *
 * 3. If validation fails, an exception response is sent based on the type of
 *    error (illegal data value or illegal data address).
 *
 * 4. If validation passes, extracts individual coil values from the packed
 *    bytes within the request buffer and updates the corresponding coil
 *    register values sequentially.
 *
 * 5. Constructs a response that echoes the first six bytes of the request
 *    (transaction ID, protocol function code, starting address, and coil
 *    count) followed by a CRC checksum. If the request is not a broadcast,
 *    the response is sent to the requester.
 **/
void modbus_write_multiple_coils()
{
    // Parse request fields
    const uint16_t startCoil = modbus_output_address(modbus.buffer);
    const uint16_t coilCount = modbus_quantity_of_registers(modbus.buffer);
    const uint8_t byteCount = modbus.buffer[MODBUS_POS_BYTECOUNT];
    const uint8_t expectedByteCount = (uint8_t)((coilCount + 7u) >> 3);
    const uint16_t totalExpectedSize = (uint16_t)(9u + byteCount); // ID(1)+FUNC(1)+START(2)+COUNT(2)+BYTE_CNT(1)+DATA(N)+CRC(2)

    // Quick sanity checks
    if ((byteCount != expectedByteCount) ||(coilCount == 0) ||(modbus.actual_size != totalExpectedSize)|| ((uint32_t)startCoil + (uint32_t)coilCount > (uint32_t)modbus.coils.count)) {
        // Would overflow the coils array
        exceptionResponse(MB_FUNCTION_WRITE_MULTIPLE_COILS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Check address range
    if (startCoil >= modbus.coils.count) {
        exceptionResponse( MB_FUNCTION_WRITE_MULTIPLE_COILS, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Write coils from packed bytes
    const uint8_t* data = &modbus.buffer[MODBUS_POS_REGISTERVALUES];
    uint16_t written = 0;

    for (uint16_t byteIdx = 0; byteIdx < byteCount; ++byteIdx) {
        const uint8_t packed = data[byteIdx];
        for (uint8_t bit = 0; bit < 8 && written < coilCount; ++bit, ++written) {
            const uint16_t coilIndex = (uint16_t)(startCoil + written);
            // Each bit LSB first maps to successive coil addresses
            modbus.coils.registers[coilIndex] = (uint8_t)((packed >> bit) & 0x01u);
        }
    }

    // Build response: echo first 6 bytes (ID..count) + CRC
    modbus_write_crc16_le(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_COILS);
    if (!modbus.broadcastFlag) modbus_send(modbus.buffer, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_COILS);
}
