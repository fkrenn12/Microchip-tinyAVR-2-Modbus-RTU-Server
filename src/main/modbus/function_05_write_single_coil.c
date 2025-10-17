#include "server.h"

#ifdef COILS
void modbus_write_single_coil(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    // Expected request size for Write Single Coil (Function 0x05):
    // ID(1) + FUNC(1) + ADDRESS(2) + VALUE(2) + CRC(2) = 8 bytes
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_COIL, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Parse request fields
    const uint16_t outputAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 1]);

    // Address check
    if (outputAddress >= (uint16_t)numOfCoils) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_COIL, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Modbus spec: 0xFF00 = ON, 0x0000 = OFF (other values illegal)
    const uint16_t rawValue = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 3]);

    // Accept only exact encodings; reject anything else
    const uint16_t MODBUS_COIL_ON_VALUE  = 0xFF00u;
    const uint16_t MODBUS_COIL_OFF_VALUE = 0x0000u;

    if (rawValue != MODBUS_COIL_ON_VALUE && rawValue != MODBUS_COIL_OFF_VALUE) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_COIL, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Write coil (1 for ON, 0 for OFF)
    coils[outputAddress] = (uint8_t)(rawValue == MODBUS_COIL_ON_VALUE ? 1u : 0u);

    // Response: echo request (ID..VALUE) + CRC
    const uint8_t totalResponseSize = 8u; // fixed size
    modbus_write_crc16_le(frame, totalResponseSize);
    if (!broadcastFlag) sendPacket(frame, totalResponseSize);
    return 0u;
}
#endif