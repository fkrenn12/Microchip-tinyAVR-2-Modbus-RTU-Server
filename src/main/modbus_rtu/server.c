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
    if (id != modbus_id && id !=0 && id !=MODBUS_CONFIGURATION_ID){
        return -1;
    }
    // Return function code if all checks passed
    return (uint16_t)frame[MODBUS_POS_FUNCTION];
}

void modbus_update(uint8_t* frame, uint16_t frameSize){
    // Check CRC
    if (modbus_crc16(frame, frameSize - 2) != modbus_read_crc16_le(frame, frameSize)) return;

    uint8_t id = frame[MODBUS_POS_ID];
    uint8_t broadcastFlag = (id == 0);
    uint8_t ackFlag = (id == modbus_id);
    uint8_t configFlag = (id == MODBUS_CONFIGURATION_ID);
    uint8_t function = frame[MODBUS_POS_FUNCTION];

    // Serial1.print("Modbus Function = ");
    // Serial1.println(function);
    if (configFlag){
        // Configuration commands
        switch (function) {
            case MB_FUNCTION_READ_HOLDING_REGISTERS:
                //modbus_read_configuration(frame, frameSize, 0);
                modbus_read_holding_registers(frame, frameSize, broadcastFlag, configurationRegisters, numOfConfigurationRegisters, MODBUS_CONFIGURATION_START);
                break;

            case MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS:
                modbus_write_multiple_holding_registers(frame, frameSize, broadcastFlag, configurationRegisters, numOfConfigurationRegisters, MODBUS_CONFIGURATION_START);
                // modbus_write_multiple_configuration_registers(frame, frameSize, 0);
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
                if (!broadcastFlag) modbus_read_holding_registers(frame, frameSize, 0, holding_registers, numOfHoldingRegisters,0);
                break;
            case MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER:
                modbus_write_single_holding_register(frame, frameSize, broadcastFlag);
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS:
                modbus_write_multiple_holding_registers(frame, frameSize, broadcastFlag, holding_registers, numOfHoldingRegisters,0);
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

