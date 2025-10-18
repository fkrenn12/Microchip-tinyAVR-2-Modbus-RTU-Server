#include "server.h"

void modbus_write_multiple_coils(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    // Parse request fields
    const uint16_t startCoil = modbus_output_address(frame);
    const uint16_t coilCount = modbus_quantity_of_registers(frame);
    const uint8_t byteCount = frame[MODBUS_POS_BYTECOUNT];
    const uint8_t expectedByteCount = (uint8_t)((coilCount + 7u) >> 3);
    const uint16_t totalExpectedSize = (uint16_t)(9u + byteCount); // ID(1)+FUNC(1)+START(2)+COUNT(2)+BYTE_CNT(1)+DATA(N)+CRC(2)

    // Quick sanity checks
    if ((byteCount != expectedByteCount) ||(coilCount == 0) ||(frameSize != totalExpectedSize)|| ((uint32_t)startCoil + (uint32_t)coilCount > (uint32_t)numOfCoils)) {
        // Would overflow the coils array
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }
    // Check address range
    if (startCoil >= numOfCoils) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Write coils from packed bytes
    const uint8_t* data = &frame[MODBUS_POS_REGISTERVALUES];
    uint16_t written = 0;

    for (uint16_t byteIdx = 0; byteIdx < byteCount; ++byteIdx) {
        const uint8_t packed = data[byteIdx];
        for (uint8_t bit = 0; bit < 8 && written < coilCount; ++bit, ++written) {
            const uint16_t coilIndex = (uint16_t)(startCoil + written);
            // Each bit LSB first maps to successive coil addresses
            coils[coilIndex] = (uint8_t)((packed >> bit) & 0x01u);
        }
    }

    // Build response: echo first 6 bytes (ID..count) + CRC
    modbus_write_crc16_le(frame, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_COILS);
    if (!broadcastFlag) sendPacket(frame, MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_COILS);
}
