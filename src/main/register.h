#pragma once
#include <avr/io.h>
#include "inttypes.h"
#include "defines.h"
#include "config.h"

// test nochwas
// An aggregate container for all Modbus register areas of your application
typedef struct ModbusAppRegisters {
    // Discrete Inputs
    uint16_t config_discrete_inputs[sizeof((uint16_t[])DISCRETE_INPUTS)/sizeof(uint16_t)];
    uint16_t discrete_inputs[sizeof((uint16_t[])DISCRETE_INPUTS)/sizeof(uint16_t)];
    uint8_t  numOfDiscreteInputs;

    // Coils
    uint16_t config_coils[sizeof((uint16_t[])COILS)/sizeof(uint16_t)];
    uint16_t coils[sizeof((uint16_t[])COILS)/sizeof(uint16_t)];
    uint8_t  numOfCoils;

    // Input Registers
    uint16_t config_input_registers[sizeof((uint16_t[])INPUT_REGISTERS)/sizeof(uint16_t)];
    uint16_t input_registers[sizeof((uint16_t[])INPUT_REGISTERS)/sizeof(uint16_t)];
    uint8_t  numOfInputRegisters;

    // Holding Registers
    uint16_t config_holding_registers[sizeof((uint16_t[])HOLDING_REGISTERS)/sizeof(uint16_t)];
    uint16_t holding_registers[sizeof((uint16_t[])HOLDING_REGISTERS)/sizeof(uint16_t)];
    uint8_t  numOfHoldingRegisters;
} ModbusAppRegisters;

static void init_app_registers(void);
void init_coils(void);
void init_discrete_inputs(void);
void init_holding_registers(void);
void init_registers(void);
void link_registers(void);