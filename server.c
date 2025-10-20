#include "server.h"
#include <avr/io.h>

modbus_frame_callback_t modbus_send_package_callback = 0;

Modbus modbus = {
    .buffer = {0},
    .head   = 0u,
    .crc    = 0u,
    .updateFlag  = 0u,
    .actual_size   = 0u,
    .ackFlag = 0u,
    .broadcastFlag = 0u,
    .configFlag = 0u,
    .function = 0u,
    .actual_id = 0u,
    .id = 0u,
    .config.registers = (uint16_t*)0,
    .config.count = 0,
    .input.registers = (uint16_t*)0,
    .input.count = 0, 
    .discretes.registers = (uint16_t*)0, 
    .discretes.count = 0,
    .coils.registers = (uint16_t*)0, 
    .coils.count = 0,
    .holding.registers = (uint16_t*)0, 
    .holding.count = 0
};

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

uint16_t modbus_read_crc16_le() {
    // CRC is stored little-endian: low byte at frame[size-2], high byte at frame[size-1]
    // Caller must ensure frameSize >= 2.
    return (uint16_t)((uint16_t)modbus.buffer[(uint16_t)(modbus.actual_size - 2u)] | ((uint16_t)modbus.buffer[(uint16_t)(modbus.actual_size - 1u)] << 8));
}

void modbus_write_crc16_le(uint8_t* buffer, uint16_t totalSize) {
    // totalSize includes the 2 CRC bytes at the end.
    // Writes CRC in little-endian order per Modbus RTU.
    const uint16_t crc = modbus_crc16(buffer, (uint16_t)(totalSize - 2u));
    buffer[(uint16_t)(totalSize - 2u)] = (uint8_t)(crc & 0xFFu);   // low byte
    buffer[(uint16_t)(totalSize - 1u)] = (uint8_t)(crc >> 8);      // high byte
}

uint8_t modbus_id_request(){
    return modbus.buffer[MODBUS_POS_ID];
}

// protocol data unit (PDU) address extraction helper
uint16_t modbus_output_address() {
    return (uint16_t)(((uint16_t)modbus.buffer[MODBUS_POS_PDU] << 8) | (uint16_t)modbus.buffer[MODBUS_POS_PDU + 1]);
}

// protocol data unit (PDU) value extraction helper
uint16_t modbus_output_value() {
    return (uint16_t)(((uint16_t)modbus.buffer[MODBUS_POS_PDU + 2] << 8) | (uint16_t)modbus.buffer[MODBUS_POS_PDU + 3]);
}

// protocol data unit (PDU) quantity extraction helper
uint16_t modbus_quantity_of_registers() {
    return (uint16_t)(((uint16_t)modbus.buffer[MODBUS_POS_PDU + 2] << 8) | (uint16_t)modbus.buffer[MODBUS_POS_PDU + 3]);
}

uint8_t modbus_need_refresh_discrete_inputs(){
    const uint8_t mb_function = (uint8_t)modbus.buffer[MODBUS_POS_FUNCTION];
    return  (uint8_t)(mb_function == MB_FUNCTION_READ_DISCRETE_INPUTS);
    } 

uint8_t modbus_need_update_configuration(){
    const uint8_t mb_function = (uint8_t)modbus.buffer[MODBUS_POS_FUNCTION];
    const uint8_t mb_id = (uint8_t)modbus.buffer[MODBUS_POS_ID];
    return (uint8_t)((mb_id == MODBUS_CONFIGURATION_ID) && mb_function==MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS);
}
uint8_t modbus_need_update_holding_registers(){
    const uint8_t mb_function = (uint8_t)modbus.buffer[MODBUS_POS_FUNCTION];
    const uint8_t mb_id = (uint8_t)modbus.buffer[MODBUS_POS_ID];
    return (uint8_t)((mb_id == modbus.id ) && ((mb_function == MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS) || (mb_function == (MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER))));
}

uint8_t modbus_need_update_coils(){
    const uint8_t mb_function = (uint8_t)modbus.buffer[MODBUS_POS_FUNCTION];
    const uint8_t mb_id = (uint8_t)modbus.buffer[MODBUS_POS_ID];
    return (uint8_t)((mb_id == modbus.id) && ((mb_function == MB_FUNCTION_WRITE_MULTIPLE_COILS) || (mb_function == MB_FUNCTION_WRITE_SINGLE_COIL)));
}

int8_t modbus_precheck()
{
    if (modbus.actual_size < 6) return -1;
    // Verify CRC: received is little-endian in frame [low, high]
    const uint16_t receivedCrc = modbus_read_crc16_le(&modbus); //
    if (modbus_crc16(modbus.buffer, (uint16_t)(modbus.actual_size)) == receivedCrc) {
        return -1;
    }
    // Verify slave ID matches our device
    modbus.actual_id = modbus.buffer[MODBUS_POS_ID];
    if (modbus.actual_id != modbus.id && modbus.actual_id !=0 && modbus.actual_id !=MODBUS_CONFIGURATION_ID){
        return -1;
    }
     
    // Return function code if all checks passed
    modbus.broadcastFlag = (modbus.actual_id == 0);
    modbus.ackFlag = (modbus.actual_id == modbus.id);
    modbus.configFlag = (modbus.actual_id == MODBUS_CONFIGURATION_ID);
    modbus.function = modbus.buffer[MODBUS_POS_FUNCTION];
    return (uint16_t)modbus.buffer[MODBUS_POS_FUNCTION];
}

void modbus_package_ready(){
    if (modbus.head > 0) {
        modbus.actual_size = modbus.head;
        const int8_t function = modbus_precheck();
        if (function > 0){    
            modbus.updateFlag = 1;      
        }  
    modbus.head=0;
  }
}

// Interrupt driven function
void modbus_char_received(uint8_t c){
    if (modbus.head < MODBUS_BUFFER_SIZE ) modbus.buffer[modbus.head++] = c;

}

uint8_t modbus_need_update(){
    const uint8_t state = modbus.updateFlag;
    modbus.updateFlag = 0;
    return state;
}

void modbus_set_id(uint8_t new_id){
    modbus.id = new_id;
}

void modbus_set_coils_registers(uint16_t* registers, uint16_t count){
    modbus.coils.registers = registers;
    modbus.coils.count = count;
}

void modbus_set_input_registers(uint16_t* registers, uint16_t count){
    modbus.input.registers = registers;
    modbus.input.count = count;
}
void modbus_set_discrete_inputs_registers(uint16_t* registers, uint16_t count){
    modbus.discretes.registers = registers;
    modbus.discretes.count = count;
}

void modbus_set_holding_registers(uint16_t* registers, uint16_t count){
    modbus.holding.registers = registers;
    modbus.holding.count = count;
}
void modbus_set_configuration_registers(uint16_t* registers, uint16_t count){
    modbus.config.registers = registers;
    modbus.config.count = count;
}

void modbus_set_send_package_callback(modbus_frame_callback_t callback){
    modbus_send_package_callback = callback;
}

void modbus_update(){
    // Check CRC
    if (modbus_crc16(modbus.buffer, modbus.actual_size - 2) != modbus_read_crc16_le(&modbus)) return;

    if (modbus.configFlag){
        // Configuration commands
        switch (modbus.function) {
            case MB_FUNCTION_READ_HOLDING_REGISTERS:
                modbus_read_holding_registers( MODBUS_ISCONFIG);
                break;

            case MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS:
                modbus_write_multiple_holding_registers( MODBUS_ISCONFIG);
                break;
            default:
                // Unknown configuration function: reply with exception (function code 0x01 per Modbus)
                exceptionResponse(modbus.function, 0 /*broadcastFlag*/, MB_EXCEPTION_ILLEGAL_FUNCTION);
                break;
        }
        return;
    }
    
    if (modbus.ackFlag || modbus.broadcastFlag) {
        // Only process if addressed to us or broadcast
        switch (modbus.function) {
            case MB_FUNCTION_READ_COILS:
                if (!modbus.broadcastFlag) modbus_read_coils();
                break;
            case MB_FUNCTION_WRITE_SINGLE_COIL:
                modbus_write_single_coil();
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_COILS:
                modbus_write_multiple_coils();
                break;
            case MB_FUNCTION_READ_HOLDING_REGISTERS:
                if (!modbus.broadcastFlag) modbus_read_holding_registers( MODBUS_ISHOLDING);
                break;
            case MB_FUNCTION_WRITE_SINGLE_HOLDING_REGISTER:
                modbus_write_single_holding_register();
                break;
            case MB_FUNCTION_WRITE_MULTIPLE_HOLDING_REGISTERS:
                modbus_write_multiple_holding_registers(MODBUS_ISHOLDING);
                break;
            case MB_FUNCTION_READ_DISCRETE_INPUTS:
                if (!modbus.broadcastFlag) modebus_read_discrete_inputs();
                break;
            case MB_FUNCTION_READ_INPUT_REGISTERS:
                if (!modbus.broadcastFlag) modbus_read_input_registers();
                break;
            default:
                exceptionResponse(modbus.function, modbus.broadcastFlag, MB_EXCEPTION_ILLEGAL_FUNCTION);
                break;
        }
    }
    return;
}

void modbus_send(uint8_t* buffer, uint16_t size){
    if (modbus_send_package_callback) modbus_send_package_callback(buffer, size);
}

void exceptionResponse(uint8_t function, uint8_t broadcastFlag, uint8_t  exception)
{
    if (!broadcastFlag) { // don't respond if its a broadcast message
        modbus.buffer[1] = (function | 0x80); // set the MSB bit high, informs the master of an exception
        modbus.buffer[2] = exception;
        modbus_write_crc16_le(modbus.buffer, MODBUS_RESPONSE_SIZE_EXCEPTION);
        modbus_send(modbus.buffer,MODBUS_RESPONSE_SIZE_EXCEPTION);
    }
    return;
}

