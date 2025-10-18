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

extern uint8_t frame_buffer[];
extern uint16_t frame_head;
extern uint16_t configurationRegisters[];
extern uint8_t numOfConfigurationRegisters;

extern uint16_t coils[];
extern uint16_t input_discretes[];
extern uint16_t input_registers[];
extern uint16_t holding_registers[];

extern uint8_t numOfCoils;
extern uint8_t numOfDiscreteInputs;
extern uint8_t numOfInputRegisters;
extern uint8_t numOfHoldingRegisters;

extern uint8_t modbus_id;

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

// external function definitions
extern void sendPacket(uint8_t* frame, uint16_t frameSize);
// helper function definitions
uint16_t modbus_output_address(const uint8_t* frame);
uint16_t modbus_output_value(const uint8_t* frame);
uint16_t modbus_quantity_of_registers(const uint8_t* frame);
// function definitions
void modbus_update(uint8_t* frame,uint16_t frameSize);
uint16_t modbus_crc16(uint8_t *buf, uint16_t len);
int8_t modbus_precheck(uint8_t* frame, uint16_t frameSize);
uint8_t modbus_id_request(uint8_t* frame, uint16_t frameSize);
uint16_t modbus_read_crc16_le(const uint8_t* frame, uint16_t frameSize);
void modbus_write_crc16_le(uint8_t* frame, uint16_t totalSize);
void modbus_write_single_coil(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag);
void modbus_write_single_holding_register(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag);
void modbus_read_coils(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag);
void modebus_read_discrete_inputs(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag);
void modbus_write_multiple_coils(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag);
void modbus_read_input_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag);
void modbus_read_holding_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag, uint16_t* registers, uint8_t numOfRegisters, uint16_t offset);
void modbus_write_multiple_holding_registers(uint8_t* frame, uint16_t frameSize, uint8_t broadcastFlag, uint16_t* registers, uint8_t numOfRegisters, uint16_t offset);
void exceptionResponse(uint8_t* frame, uint8_t function, uint8_t broadcastFlag, uint8_t  exception);

#ifdef __cplusplus
}
#endif

#endif