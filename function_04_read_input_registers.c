#include "server.h"
/**
 * Reads Modbus input registers and builds a response frame.
 *
 * The function processes Modbus communication frames to handle the "Read Input Registers" request.
 * It validates the request size, starting address, and register count. Upon validation success, it reads
 * the requested registers, constructs a response containing data, and transmits it back.
 *
 * Steps:
 * - Parse the request from the Modbus frame.
 * - Validate the frame size and ensure bounds are within input registers' range.
 * - Populate the response buffer with requested register values.
 * - Build the response with CRC validation and send it back.
 *
 * Exception responses are sent for errors including invalid data value or address.
 */
extern Modbus modbus;
void modbus_read_input_registers()
{
    const uint16_t startingAddress = modbus_output_address();
    const uint16_t numOfRegisters = modbus_quantity_of_registers();
    const uint8_t invalidStartAdress = (startingAddress >= modbus.input.count) ? 1 : 0;
    const uint8_t incorrectPackageSize = (modbus.actual_size == 8u) ? 0 : 1;
    const uint8_t invalidNumberOfRegisters = (uint8_t)((!numOfRegisters) || ((uint32_t)startingAddress + numOfRegisters > modbus.input.count));
    // Basic size check for request
    uint8_t exception = (invalidNumberOfRegisters || incorrectPackageSize) ? MB_EXCEPTION_ILLEGAL_DATA_VALUE : 0;
    exception = (invalidStartAdress) ? MB_EXCEPTION_ILLEGAL_DATA_ADDRESS : exception;
    if (exception){
        exceptionResponse(MB_FUNCTION_READ_INPUT_REGISTERS, modbus.broadcastFlag, exception);
        return;
    }
    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); 
    for (uint16_t index = startingAddress, end = (uint16_t)(startingAddress + numOfRegisters);index < end;++index) {
        const uint16_t reg = modbus.input.registers[index];
        modbus.buffer[out++] = (uint8_t)(reg >> 8);
        modbus.buffer[out++] = (uint8_t)(reg & 0xFFu);
    }
    // Build response: ID, FUNC, BYTE_CNT, DATA..., CRC
    const uint8_t byteCount = (uint8_t)(numOfRegisters * 2u);
    modbus.buffer[MODBUS_POS_PDU] = byteCount;
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u); // 1(ID)+1(FUNC)+1(BYTE_CNT)+N(DATA)+2(CRC)
    modbus_write_crc16_le(modbus.buffer, responseSize);
    modbus_send(modbus.buffer, responseSize);
}
