#include "server.h"
// C
uint16_t modbus_read_crc16_le(const uint8_t* frame, uint16_t frameSize) {
    // CRC is stored little-endian: low byte at frame[size-2], high byte at frame[size-1]
    // Caller must ensure frameSize >= 2.
    return (uint16_t)((uint16_t)frame[(uint16_t)(frameSize - 2u)] | ((uint16_t)frame[(uint16_t)(frameSize - 1u)] << 8));
}

void modbus_write_crc16_le(uint8_t* frame, uint16_t totalSize) {
    // totalSize includes the 2 CRC bytes at the end.
    // Writes CRC in little-endian order per Modbus RTU.
    const uint16_t crc = modbus_crc16(frame, (uint16_t)(totalSize - 2u));
    frame[(uint16_t)(totalSize - 2u)] = (uint8_t)(crc & 0xFFu);   // low byte
    frame[(uint16_t)(totalSize - 1u)] = (uint8_t)(crc >> 8);      // high byte
}

#ifdef COILS
void modbus_read_coils(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    /*
    Read Coils (Function 0x01)
    - Request: ID(1) + FUNC(1) + START_ADDR(2) + QUANTITY(2) + CRC(2) = 8 bytes
    - Response: ID + FUNC + BYTE_CNT(1) + DATA(byteCnt) + CRC(2)
      DATA packs coil states LSB-first per byte (bit 0 = first coil).
    */
    // Basic size check for request
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_READ_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Parse request
    const uint16_t startingAddress = (uint16_t)((frame[MODBUS_POS_DATA] << 8) | frame[MODBUS_POS_DATA + 1]);
    const uint16_t quantity        = (uint16_t)((frame[MODBUS_POS_DATA + 2] << 8) | frame[MODBUS_POS_DATA + 3]);

    // Quantity must be >= 1. Spec allows up to 2000; we enforce our table bounds below.
    if (quantity == 0u) {
        exceptionResponse(frame, MB_FUNCTION_READ_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Address bounds against coils table
    if (startingAddress >= numOfCoils) {
        exceptionResponse(frame, MB_FUNCTION_READ_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((uint32_t)startingAddress + (uint32_t)quantity > (uint32_t)numOfCoils) {
        exceptionResponse(frame, MB_FUNCTION_READ_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Compute response payload size in bytes (ceil(quantity / 8))
    const uint8_t byteCount = (uint8_t)((quantity + 7u) >> 3);

    // Build response: ID, FUNC, BYTE_CNT, DATA..., CRC
    frame[MODBUS_POS_FUNCTION] = MB_FUNCTION_READ_COILS;
    frame[MODBUS_POS_DATA]     = byteCount;

    uint8_t outIndex = MODBUS_POS_FUNCTION + 2; // index 3
    uint16_t remaining = quantity;
    uint16_t srcIndex  = startingAddress;

    for (uint8_t b = 0; b < byteCount; ++b) {
        uint8_t packed = 0;
        const uint8_t bitsThisByte = (remaining >= 8u) ? 8u : (uint8_t)remaining;

        for (uint8_t bit = 0; bit < bitsThisByte; ++bit) {
            // LSB-first: coil[srcIndex] -> bit position 'bit'
            const uint8_t val = coils[srcIndex++] ? 1u : 0u;
            packed |= (uint8_t)(val << bit);
        }

        frame[outIndex++] = packed;
        remaining = (uint16_t)(remaining - bitsThisByte);
    }

    // Finalize CRC and send
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u);
    modbus_write_crc16_le(frame, responseSize);

    sendPacket(frame, responseSize);
    return 0u;
}
#endif
#ifdef INPUT_DISCRETES
void modebus_read_inputs(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    /*
    Read Discrete Inputs (Function 0x02)
    - Request: ID(1) + FUNC(1) + START_ADDR(2) + QUANTITY(2) + CRC(2) = 8 bytes
    - Response: ID + FUNC + BYTE_CNT(1) + DATA(byteCnt) + CRC(2)
      DATA packs inputs LSB-first per byte.
    */

    // Basic size check for request
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_READ_DISCRETE_INPUTS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Parse request
    const uint16_t startingAddress = (uint16_t)((frame[MODBUS_POS_DATA] << 8) | frame[MODBUS_POS_DATA + 1]);
    const uint16_t quantity        = (uint16_t)((frame[MODBUS_POS_DATA + 2] << 8) | frame[MODBUS_POS_DATA + 3]);

    // Modbus limits: quantity 1..2000 (spec). We can enforce at least 1 and within our table.
    if (quantity == 0u) {
        exceptionResponse(frame, MB_FUNCTION_READ_DISCRETE_INPUTS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Address bounds against input_discretes array
    if (startingAddress >= numOfDiscreteInputs) {
        exceptionResponse(frame, MB_FUNCTION_READ_DISCRETE_INPUTS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((uint32_t)startingAddress + (uint32_t)quantity > (uint32_t)numOfDiscreteInputs) {
        exceptionResponse(frame, MB_FUNCTION_READ_DISCRETE_INPUTS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return; 
    }

    // Compute response payload size in bytes (ceil(quantity / 8))
    const uint8_t byteCount = (uint8_t)((quantity + 7u) >> 3);

    // Build response
    frame[MODBUS_POS_FUNCTION] = MB_FUNCTION_READ_DISCRETE_INPUTS;
    frame[MODBUS_POS_DATA]     = byteCount;           // byte count at position 2 for read responses
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
    return 0u;
}
#endif
#ifdef COILS
void modbus_write_multiple_coils(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    // Parse request fields
    const uint16_t startCoil = (uint16_t)((frame[MODBUS_POS_DATA] << 8) | frame[MODBUS_POS_DATA + 1]);
    const uint16_t coilCount = (uint16_t)((frame[MODBUS_POS_DATA + 2] << 8) | frame[MODBUS_POS_DATA + 3]);
    const uint8_t byteCount = frame[MODBUS_POS_DATA + 4];

    // Quick sanity checks
    if (coilCount == 0) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return; 
    }

    // Check address range
    if (startCoil >= numOfCoils) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((uint32_t)startCoil + (uint32_t)coilCount > (uint32_t)numOfCoils) {
        // Would overflow the coils array
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Expected payload byte count is ceil(coilCount / 8.0)
    const uint8_t expectedByteCount = (uint8_t)((coilCount + 7u) >> 3);
    if (byteCount != expectedByteCount) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Validate total frame size: ID(1) + FUNC(1) + start(2) + count(2) + byteCount(1)
    // + data(byteCount) + CRC(2) = 9 + byteCount
    if (frameSize != (uint16_t)(9u + byteCount)) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_COILS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
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
    const uint8_t responseSize = 8u;
    modbus_write_crc16_le(frame, responseSize);
    if (!broadcastFlag) sendPacket(frame, responseSize);
    return 0;
}
#endif
#ifdef HOLDING_REGISTERS
void modbus_read_holding_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    /*
    Function 0x03: Read Holding Registers
    Request:  ID(1) + FUNC(1) + START(2) + COUNT(2) + CRC(2) = 8 bytes
    Response: ID + FUNC + BYTE_CNT(1) + DATA(2 * COUNT) + CRC(2)
    */

    // Strict request size
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return; 
    }

    // Parse request
    const uint16_t startingAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) |
                                                (uint16_t)frame[MODBUS_POS_DATA + 1]);
    const uint16_t registerCount  = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) |
                                               (uint16_t)frame[MODBUS_POS_DATA + 3]);

    // Quantity must be at least 1 (spec allows up to 125)
    if (registerCount == 0u) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Bounds checks with overflow-safe math
    if (startingAddress >= (uint16_t)numOfHoldingRegisters) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)numOfHoldingRegisters) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Build response
    const uint8_t byteCount = (uint8_t)(registerCount * 2u);
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u); // ID, FUNC, BYTE_CNT, DATA, CRC

    frame[MODBUS_POS_FUNCTION] = MB_FUNCTION_READ_HOLDING_REGISTERS;
    frame[MODBUS_POS_DATA]     = byteCount;

    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); // start of data (index 3)
    uint16_t idx = startingAddress;
    const uint16_t end = (uint16_t)(startingAddress + registerCount);

    while (idx < end) {
        const uint16_t reg = holding_registers[idx++];
        frame[out++] = (uint8_t)(reg >> 8);
        frame[out++] = (uint8_t)(reg & 0xFFu);
    }

    modbus_write_crc16_le(frame, responseSize);
    sendPacket(frame, responseSize);
    return 0u;
}
#endif

#ifdef INPUT_REGISTERS
void modbus_read_input_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    /*
    Function 0x04: Read Input Registers
    Request:  ID(1) + FUNC(1) + START(2) + COUNT(2) + CRC(2) = 8 bytes
    Response: ID + FUNC + BYTE_CNT(1) + DATA(2 * COUNT) + CRC(2)
    */

    // Strict request size
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_READ_INPUT_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Parse request
    const uint16_t startingAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) |
                                                (uint16_t)frame[MODBUS_POS_DATA + 1]);
    const uint16_t registerCount  = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) |
                                               (uint16_t)frame[MODBUS_POS_DATA + 3]);

    // Quantity must be at least 1 (spec allows up to 125)
    if (registerCount == 0u) {
        exceptionResponse(frame, MB_FUNCTION_READ_INPUT_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Bounds checks with overflow-safe math against input_registers table
    if (startingAddress >= (uint16_t)numOfInputRegisters) {
        exceptionResponse(frame, MB_FUNCTION_READ_INPUT_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)numOfInputRegisters) {
        exceptionResponse(frame, MB_FUNCTION_READ_INPUT_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Build response: ID, FUNC, BYTE_CNT, DATA..., CRC
    const uint8_t byteCount = (uint8_t)(registerCount * 2u);
    const uint8_t responseSize = (uint8_t)(3u + byteCount + 2u); // 1(ID)+1(FUNC)+1(BYTE_CNT)+N(DATA)+2(CRC)

    frame[MODBUS_POS_FUNCTION] = MB_FUNCTION_READ_INPUT_REGISTERS;
    frame[MODBUS_POS_DATA]     = byteCount;

    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); // index 3
    uint16_t idx = startingAddress;
    const uint16_t end = (uint16_t)(startingAddress + registerCount);

    while (idx < end) {
        const uint16_t reg = input_registers[idx++];
        frame[out++] = (uint8_t)(reg >> 8);
        frame[out++] = (uint8_t)(reg & 0xFFu);
    }

    modbus_write_crc16_le(frame, responseSize);
    sendPacket(frame, responseSize);
    return 0u;
}
#endif

#ifdef HOLDING_REGISTERS
void modbus_write_single_holding_register(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    /*
    Function 0x06: Write Single Holding Register
    Request:  ID(1) + FUNC(1) + ADDRESS(2) + VALUE(2) + CRC(2) = 8 bytes
    Response: Echo of ID..VALUE + CRC (8 bytes)
    */

    // Strict request size check
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Parse address
    const uint16_t address =
        (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 1]);

    // Address bounds check
    if (address >= (uint16_t)numOfHoldingRegisters) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    // Parse value (big-endian: Hi, Lo)
    const uint16_t value =
        (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 3]);

    // Perform write
    holding_registers[address] = value;

    // Build response: echo ID..VALUE and append CRC
    const uint8_t totalResponseSize = 8u; // fixed size
    modbus_write_crc16_le(frame, totalResponseSize);

    if (!broadcastFlag) sendPacket(frame, 8u);
    return 0u;
}
#endif

#ifdef HOLDING_REGISTERS
void modbus_write_multiple_holding_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag)
{
    /*
    Function 0x10: Write Multiple Holding Registers
    Request:  ID(1) + FUNC(1) + START(2) + COUNT(2) + BYTE_CNT(1) + DATA(N) + CRC(2)
    Response: ID(1) + FUNC(1) + START(2) + COUNT(2) + CRC(2) => 8 bytes
    */

    // Minimal fixed part is 9 bytes before CRC depends on byte count; validate later precisely.
    if (frameSize < 9u) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Parse request header fields
    const uint16_t startingAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 1]);
    const uint16_t registerCount  = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 3]);
    const uint8_t  byteCount      = frame[MODBUS_POS_BYTECOUNT];

    // Spec: COUNT 1..123. Enforce >=1; upper bound effectively enforced by our table and byteCount below.
    if (registerCount == 0u) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Byte count must match exactly COUNT * 2
    const uint16_t expectedBytes = (uint16_t)(registerCount * 2u);
    if (byteCount != (uint8_t)expectedBytes) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Validate total frame size = 9 + byteCount (ID,FUNC,START(2),COUNT(2),BYTE_CNT(1),DATA(byteCount),CRC(2))
    const uint16_t expectedFrame = (uint16_t)(9u + (uint16_t)byteCount);
    if (frameSize != expectedFrame) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Address range checks
    if (startingAddress >= (uint16_t)numOfHoldingRegisters) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }
    if ((uint32_t)startingAddress + (uint32_t)registerCount > (uint32_t)numOfHoldingRegisters) {
        exceptionResponse(frame, MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Write registers from payload
    uint8_t dataIndex = MODBUS_POS_REGISTERVALUES; // first data byte after byteCount
    uint16_t dst = startingAddress;
    for (uint16_t i = 0; i < registerCount; ++i) {
        const uint16_t value = (uint16_t)(((uint16_t)frame[dataIndex] << 8) | (uint16_t)frame[(uint8_t)(dataIndex + 1u)]);
        holding_registers[dst++] = value;
        dataIndex = (uint8_t)(dataIndex + 2u);
    }

    // Build response: echo ID..COUNT (first 6 bytes) + CRC
    const uint8_t totalResponseSize = 8u; // fixed size
    modbus_write_crc16_le(frame, totalResponseSize);
    if (!broadcastFlag) sendPacket(frame, 8u);
    return 0u;
}
#endif

uint8_t modbus_id_request(uint8_t* frame, uint16_t frameSize){
    return frame[MODBUS_POS_ID];
}

int8_t modbus_precheck(uint8_t* frame, uint16_t frameSize)
{
    // Validate pointers and minimal frame size: at least ID(1)+FUNC(1)+CRC(2)
    if (frameSize < 4u || frameSize > (uint16_t)UART_BUFFER_SIZE) {
        return -1;
    }

    // Verify CRC: received is little-endian in frame [low, high]
    const uint16_t receivedCrc =
        (uint16_t)((uint16_t)frame[(uint16_t)(frameSize - 2u)] |
                   ((uint16_t)frame[(uint16_t)(frameSize - 1u)] << 8));

    if (modbus_crc16(frame, (uint16_t)(frameSize - 2u)) != receivedCrc) {
        return -1;
    }

    // Verify slave ID matches our device
    const uint8_t id = frame[MODBUS_POS_ID];
    if (id != modbus_id && id !=0 && id !=254){
        return -1;
    }

    // Return function code if all checks passed
    return (uint16_t)frame[MODBUS_POS_FUNCTION];
}
void modbus_write_multiple_configuration_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag){
    // Parse request header fields
    const uint16_t startingAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 1]);
    const uint16_t registerCount  = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 3]);
    const uint8_t  byteCount      = frame[MODBUS_POS_BYTECOUNT];
}

void modbus_write_single_configuration(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag){
    uint16_t address = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 1]);
    uint16_t value = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 3]);

    if (address == HOLDING_REGISTER_MODBUS_ID){
        // Change Modbus ID write to eeprom
        store_modbus_id_to_eeprom((uint8_t)(value & 0xFF));
        // eeprom_write_byte(0,value & 0xFF); 
    }
    else if (address == HOLDING_REGISTER_BAUDRATE){
        // baudrate
        uint32_t new_uart_baudrate = (uint32_t)value * 100;
        // write to eeprom
        store_baudrate_to_eeprom(new_uart_baudrate);
        // eeprom_write_byte((uint8_t *)EEPROM_OFFSET_BAUDRATE,(uint8_t)((new_uart_baudrate >> 8) & 0xFF)); 
        // eeprom_write_byte((uint8_t *)EEPROM_OFFSET_BAUDRATE+1,(uint8_t)((new_uart_baudrate >> 0) & 0xFF)); 
    }

    // Build response: echo ID..VALUE and append CRC
    const uint8_t totalResponseSize = 8u; // fixed size
    modbus_write_crc16_le(frame, totalResponseSize);
    sendPacket(frame_buffer, totalResponseSize);
    // reset microcontroller
    _delay_ms(2);
    cli();
    _delay_ms(10);
    asm volatile ("jmp 0");
}

void modbus_read_configuration(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag){
    // Read configuration values and send response
    /*
    Function 0x03: Read Holding Registers
    Request:  ID(1) + FUNC(1) + START(2) + COUNT(2) + CRC(2) = 8 bytes
    Response: ID + FUNC + BYTE_CNT(1) + DATA(2 * COUNT) + CRC(2)
    */

    // Strict request size
    if (frameSize != 8u) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Parse request
    const uint16_t startingAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 1]);
    const uint16_t registerCount  = (uint16_t)(((uint16_t)frame[MODBUS_POS_DATA + 2] << 8) | (uint16_t)frame[MODBUS_POS_DATA + 3]);

    // Quantity must be at least 1 (spec allows up to 125)
    // if (registerCount == 0u || registerCount > 2u) {
    if (registerCount == 0u) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_VALUE);
        return;
    }

    // Bounds checks with overflow-safe math
    if (startingAddress != HOLDING_REGISTER_MODBUS_ID && 
        startingAddress != HOLDING_REGISTER_BAUDRATE && 
        startingAddress != HOLDING_REGISTER_SERVER_NAME) {
        exceptionResponse(frame, MB_FUNCTION_READ_HOLDING_REGISTERS, broadcastFlag, MB_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        return;
    }

    uint8_t out = (uint8_t)(MODBUS_POS_FUNCTION + 2); // start of data (index 3)
    uint16_t idx = startingAddress;
    const uint16_t end = (uint16_t)(startingAddress + registerCount);
    uint8_t byteCount = 0;
    while (idx < end) {
        uint16_t reg = 0;
        if (idx == HOLDING_REGISTER_MODBUS_ID){
            reg = (uint16_t)modbus_id;
            frame[out++] = (uint8_t)(reg >> 8);
            frame[out++] = (uint8_t)(reg & 0xFFu);
            byteCount += 2;
        }
        else if (idx == HOLDING_REGISTER_BAUDRATE){
            reg = (uint16_t)(uart_baudrate / 100);
            frame[out++] = (uint8_t)(reg >> 8);
            frame[out++] = (uint8_t)(reg & 0xFFu);
            byteCount += 2;
        }
        else if (idx ==  HOLDING_REGISTER_SERVER_NAME){
            uint8_t server_name[MAX_SERVERNAME_LENGTH+2] = {0};
            load_modbus_server_name_from_eeprom(server_name, MAX_SERVERNAME_LENGTH);
            if (strlen(server_name) % 2 != 0){
                // make even length
                server_name[strlen(server_name)+1] = 0;
            }
            for (uint8_t i = 0; i < MAX_SERVERNAME_LENGTH+2; i+=2){
                uint8_t c = server_name[i];
                frame[out++] = server_name[i];
                frame[out++] = server_name[i+1];
                byteCount += 2;
                if (c == 0) break;
            }
        }
        idx++;
    }

    // Build response
    frame[MODBUS_POS_DATA] = byteCount;
    const uint8_t totalResponseSize = (uint8_t)(3u + byteCount + 2u); // ID, FUNC, BYTE_CNT, DATA, CRC
    modbus_write_crc16_le(frame, totalResponseSize);
    sendPacket(frame, totalResponseSize);
    return 0u;
}


void modbus_update(uint8_t* frame, uint16_t frameSize){
    // Serial1.print("Modbus update called with frameSize = ");
    // Serial1.println(frameSize);

    // Check frame size validity
    if (frameSize >= UART_BUFFER_SIZE || frameSize < 4) return;

    // Check CRC
    if (modbus_crc16(frame, frameSize - 2) != modbus_read_crc16_le(frame, frameSize)) return;

    uint8_t id = frame[MODBUS_POS_ID];
    uint8_t broadcastFlag = (id == 0);
    uint8_t ackFlag = (id == modbus_id);
    uint8_t configFlag = (id == 254);
    uint8_t function = frame[MODBUS_POS_FUNCTION];

    // Serial1.print("Modbus Function = ");
    // Serial1.println(function);
    if (configFlag){
        // Configuration commands
        switch (function) {
            case MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER:
                // Will not return due to MCU reset
                modbus_write_single_configuration(frame, frameSize, 0);
                break;

            case MB_FUNCTION_READ_HOLDING_REGISTERS:
                modbus_read_configuration(frame, frameSize, 0);
                break;

            case MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS:
                modbus_write_multiple_configuration_registers(frame, frameSize, 0);
                break;

            default:
                // Unknown configuration function: reply with exception (function code 0x01 per Modbus)
                exceptionResponse(frame, function, 0 /*broadcastFlag*/, MB_EXCEPTION_ILLEGAL_FUNCTION);
                break;
        }
        return;
    }
    
    if (ackFlag || broadcastFlag) {
        // Only process if addressed to us or broadcast
        switch (function) {
            #ifdef COILS
            case MB_FUNCTION_READ_COILS:
                if (!broadcastFlag) modbus_read_coils(frame, frameSize, 0);
                break;
            case MB_FUNCTION_WRITE_SINGLE_COIL:
                modbus_write_single_coil(frame, frameSize, broadcastFlag);
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_COILS:
                modbus_write_multiple_coils(frame, frameSize, broadcastFlag);
                break;
            #endif
            #ifdef HOLDING_REGISTERS
            case MB_FUNCTION_READ_HOLDING_REGISTERS:
                if (!broadcastFlag) modbus_read_holding_registers(frame, frameSize, 0);
                break;
            case MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER:
                modbus_write_single_holding_register(frame, frameSize, broadcastFlag);
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS:
                modbus_write_multiple_holding_registers(frame, frameSize, broadcastFlag);
                break;
            #endif
            #ifdef INPUT_DISCRETES
            case MB_FUNCTION_READ_DISCRETE_INPUTS:
                if (!broadcastFlag) modebus_read_inputs(frame, frameSize, 0);
                break;
            #endif
            #ifdef INPUT_REGISTERS
            case MB_FUNCTION_READ_INPUT_REGISTERS:
                if (!broadcastFlag) modbus_read_input_registers(frame, frameSize, 0);
                break;
            #endif
            default:
                exceptionResponse(frame, function, broadcastFlag, MB_EXCEPTION_ILLEGAL_FUNCTION);
                break;
        }
            
    }
    return;
}

void exceptionResponse(uint8_t* frame, uint8_t function, uint8_t broadcastFlag, uint8_t  exception)
{
    if (!broadcastFlag) { // don't respond if its a broadcast message
        frame[1] = (function | 0x80); // set the MSB bit high, informs the master of an exception
        frame[2] = exception;
        const uint8_t responseSize = 5u; // ID(1) + FUNC(1) + EXC(1) + CRC(2)
        modbus_write_crc16_le(frame, responseSize);
        sendPacket(frame,responseSize);
    }
    return;
}

