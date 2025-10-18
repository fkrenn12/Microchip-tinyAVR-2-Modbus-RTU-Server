#include "server.h"

uint16_t modbus_crc16(uint8_t *buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (uint8_t bit = 0; bit < 8; bit++)
            crc = (crc & 0x0001) ? (crc >> 1) ^ 0xA001 : crc >> 1;
    }
    return crc;
}

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

// protocol data unit (PDU) address extraction helper
uint16_t modbus_output_address(const uint8_t* frame) {
    return (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 1]);
}

// protocol data unit (PDU) value extraction helper
uint16_t modbus_output_value(const uint8_t* frame) {
    return (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU + 2] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 3]);
}

// protocol data unit (PDU) quantity extraction helper
uint16_t modbus_quantity_of_registers(const uint8_t* frame) {
    return (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU + 2] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 3]);
}

uint8_t modbus_id_request(uint8_t* frame, uint16_t frameSize){
    return frame[MODBUS_POS_ID];
}

int8_t modbus_precheck(uint8_t* frame, uint16_t frameSize)
{
    // Verify CRC: received is little-endian in frame [low, high]
    const uint16_t receivedCrc = modbus_read_crc16_le(frame, frameSize); //
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
    const uint16_t startingAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 1]);
    const uint16_t registerCount  = (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU + 2] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 3]);
    const uint8_t  byteCount      = frame[MODBUS_POS_BYTECOUNT];
}

void modbus_write_single_configuration(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag){
    uint16_t address = (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 1]);
    uint16_t value = (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU + 2] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 3]);

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
    const uint16_t startingAddress = (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 1]);
    const uint16_t registerCount  = (uint16_t)(((uint16_t)frame[MODBUS_POS_PDU + 2] << 8) | (uint16_t)frame[MODBUS_POS_PDU + 3]);

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
    frame[MODBUS_POS_PDU] = byteCount;
    const uint8_t totalResponseSize = (uint8_t)(3u + byteCount + 2u); // ID, FUNC, BYTE_CNT, DATA, CRC
    modbus_write_crc16_le(frame, totalResponseSize);
    sendPacket(frame, totalResponseSize);
    return 0u;
}


void modbus_update(uint8_t* frame, uint16_t frameSize){
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
            case MB_FUNCTION_READ_COILS:
                if (!broadcastFlag) modbus_read_coils(frame, frameSize, 0);
                break;
            case MB_FUNCTION_WRITE_SINGLE_COIL:
                modbus_write_single_coil(frame, frameSize, broadcastFlag);
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_COILS:
                modbus_write_multiple_coils(frame, frameSize, broadcastFlag);
                break;
            case MB_FUNCTION_READ_HOLDING_REGISTERS:
                if (!broadcastFlag) modbus_read_holding_registers(frame, frameSize, 0);
                break;
            case MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER:
                modbus_write_single_holding_register(frame, frameSize, broadcastFlag);
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS:
                modbus_write_multiple_holding_registers(frame, frameSize, broadcastFlag);
                break;
            case MB_FUNCTION_READ_DISCRETE_INPUTS:
                if (!broadcastFlag) modebus_read_discrete_inputs(frame, frameSize, 0);
                break;
            case MB_FUNCTION_READ_INPUT_REGISTERS:
                if (!broadcastFlag) modbus_read_input_registers(frame, frameSize, 0);
                break;
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
        modbus_write_crc16_le(frame, MODBUS_RESPONSE_SIZE_EXCEPTION);
        sendPacket(frame,MODBUS_RESPONSE_SIZE_EXCEPTION);
    }
    return;
}

