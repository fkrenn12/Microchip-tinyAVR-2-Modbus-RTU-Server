#ifndef _MODBUS_SLAVE_H
#define _MODBUS_SLAVE_H
/*
  Modbus Specifications and Implementation Guides
  
  http://www.modbus.org/docs/Modbus_over_serial_line_V1_02.pdf
  http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b.pdf
  http://www.modbus.org/docs/PI_MBUS_300.pdf
*/

#include <inttypes.h>
#include "exceptions.h"
#include "functions.h"

#ifdef __cplusplus
extern "C" {
#endif



#define MODBUS_BUFFER_SIZE 256
#define MODBUS_POS_ID 0 // position in the frame where the address starts
#define MODBUS_POS_FUNCTION 1 // position in the frame where the frame size is stored
#define MODBUS_POS_PDU 2 // position in the frame where the protovol data unit  starts
#define MODBUS_POS_BYTECOUNT 6
#define MODBUS_POS_REGISTERVALUES 7

#define MODBUS_COIL_ON_VALUE   0xFF00u
#define MODBUS_COIL_OFF_VALUE  0x0000u

#define MODBUS_RESPONSE_SIZE_EXCEPTION 5u // ID(1) + FUNC(1) + EXC(1) + CRC(2)
#define MODBUS_RESPONSE_SIZE_WRITE_SINGLE_COIL 8u // ID(1) + FUNC(1) + BYTECOUNT(1) + CRC(2)
#define MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_COILS 8u // ID(1) + FUNC(1) + BYTECOUNT(1) + CRC(2)
#define MODBUS_RESPONSE_SIZE_WRITE_MULTIPLE_HOLDING_REGISTERS 8u // ID(1) + FUNC(1) + BYTECOUNT(1) + CRC(2)
#define MODBUS_RESPONSE_SIZE_WRITE_SINGLE_REGISTER 8u // ID(1) + FUNC(1) + BYTECOUNT(1) + CRC(2)

#define MODBUS_CONFIGURATION_ID 254
#define MODBUS_CONFIGURATION_START (uint16_t)40000 // Starting address for holding registers used for configuration
#define MODBUS_ISHOLDING 0
#define MODBUS_ISCONFIG 1

typedef void (*modbus_frame_callback_t)(uint8_t* frame, uint16_t len);
typedef void (*update_callback_t)(void);

typedef struct {
    uint16_t* registers;
    uint16_t count;
} Registers;

typedef struct _callback{
  modbus_frame_callback_t send_package;
  update_callback_t update_holding_registers;
  update_callback_t update_input_registers;
  update_callback_t update_input_discretes;
  update_callback_t update_coils;
  update_callback_t update_configuration;
}Callback;

typedef struct _modbus{
    uint8_t  buffer[MODBUS_BUFFER_SIZE];
    uint16_t head;
    uint16_t crc_appended;
    uint8_t  updateFlag;
    uint16_t actual_size;
    uint8_t actual_id;
    uint8_t broadcastFlag;
    uint8_t ackFlag;
    uint8_t configFlag;
    uint8_t function;
    uint8_t id;
    Registers input; 
    Registers discretes; 
    Registers coils; 
    Registers holding; 
    Registers config; 
} Modbus;



void modbus_send(uint8_t* buffer, uint16_t len);

// helper function definitions
uint16_t modbus_output_address();
uint16_t modbus_output_value();
uint16_t modbus_quantity_of_registers();
// function definitions
// public
void modbus_char_received(uint8_t c);
void modbus_package_ready();
int8_t modbus_precheck();
uint8_t modbus_need_update();
void modbus_update();
void modbus_set_id(uint8_t new_id);
void modbus_set_coils_registers(uint16_t* registers, uint16_t count);
void modbus_set_input_registers(uint16_t* registers, uint16_t count);
void modbus_set_discrete_inputs_registers(uint16_t* registers, uint16_t count);
void modbus_set_holding_registers(uint16_t* registers, uint16_t count);
void modbus_set_configuration_registers(uint16_t* registers, uint16_t count);
void modbus_set_send_package_callback(modbus_frame_callback_t callback);
void modbus_set_update_holding_registers_callback(update_callback_t callback);
void modbus_set_update_input_registers_callback(update_callback_t callback);
void modbus_set_update_input_discretes_callback(update_callback_t callback);
void modbus_set_update_coils_callback(update_callback_t callback);
void modbus_set_update_configuration_callback(update_callback_t callback);

// private
uint16_t modbus_crc16(uint8_t *buffer, uint16_t len);
uint8_t modbus_id_request();
uint16_t modbus_read_crc16_le();
void modbus_write_crc16_le(uint8_t* buffer, uint16_t totalSize);
void modbus_write_single_coil();
void modbus_write_single_holding_register();
void modbus_read_coils();
void modebus_read_discrete_inputs();
void modbus_write_multiple_coils();
void modbus_read_input_registers();
void modbus_read_holding_registers(uint8_t isConfig);
void modbus_write_multiple_holding_registers(uint8_t isConfig);
void exceptionResponse(uint8_t function, uint8_t broadcastFlag, uint8_t  exception);

#ifdef __cplusplus
}
#endif

#endif
