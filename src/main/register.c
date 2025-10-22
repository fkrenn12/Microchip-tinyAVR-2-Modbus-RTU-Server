#include "register.h"

ModbusAppRegisters g_registers;

void init_registers(void){
    init_app_registers();
    init_coils();
    init_discrete_inputs();
    init_holding_registers(); 
}

void link_registers(void){
    // link registers to modbus
    modbus_set_coils_registers(g_registers.coils, g_registers.numOfCoils);
    modbus_set_input_registers(g_registers.input_registers, g_registers.numOfInputRegisters);
    modbus_set_discrete_inputs_registers(g_registers.discrete_inputs, g_registers.numOfDiscreteInputs);
    modbus_set_holding_registers(g_registers.holding_registers, g_registers.numOfHoldingRegisters);
}

static void init_app_registers(void) {
    // copy configuration from macro
    // Discrete Inputs
    {
        static const uint16_t tmp[] = DISCRETE_INPUTS;
        g_registers.numOfDiscreteInputs = (uint8_t)(sizeof(tmp)/sizeof(uint16_t));
        for (uint8_t i = 0; i < g_registers.numOfDiscreteInputs; ++i) {
            g_registers.config_discrete_inputs[i] = tmp[i];
            g_registers.config_discrete_inputs[i] = 0;
        }
    }

    // Coils
    {
        static const uint16_t tmp[] = COILS;
        g_registers.numOfCoils = (uint8_t)(sizeof(tmp)/sizeof(uint16_t));
        for (uint8_t i = 0; i < g_registers.numOfCoils; ++i) {
            g_registers.config_coils[i] = tmp[i];
            g_registers.coils[i] = 0;
        }
    }

    // Input Registers
    {
        static const uint16_t tmp[] = INPUT_REGISTERS;
        g_registers.numOfInputRegisters = (uint8_t)(sizeof(tmp)/sizeof(uint16_t));
        for (uint8_t i = 0; i < g_registers.numOfInputRegisters; ++i) {
            g_registers.config_input_registers[i] = tmp[i];
            g_registers.input_registers[i] = 0;
        }
    }

    // Holding Registers
    {
        static const uint16_t tmp[] = HOLDING_REGISTERS;
        g_registers.numOfHoldingRegisters = (uint8_t)(sizeof(tmp)/sizeof(uint16_t));
        for (uint8_t i = 0; i < g_registers.numOfHoldingRegisters; ++i) {
            g_registers.config_holding_registers[i] = tmp[i];
            g_registers.holding_registers[i] = 0;
        }
    }
}

void init_coils(void){
    // Init coils
    for (uint8_t i = 0; i < g_registers.numOfCoils; i++){
       uint16_t config_coil = g_registers.config_coils[i];
       if ((uint8_t)(config_coil>>8) == PORTA_CODE) PORTA.DIRSET = (uint8_t)(config_coil & 0xFF); // Set PORTA coil pin as output
       else if ((uint8_t)(config_coil>>8) == PORTB_CODE) PORTB.DIRSET = (uint8_t)(config_coil & 0xFF); // Set PORTB coil pin as output
       #ifdef PORTC
       else if ((uint8_t)(config_coil>>8) == PORTC_CODE) PORTC.DIRSET = (uint8_t)(config_coil & 0xFF); // Set PORTC coil pin as output
       #endif
       #ifdef PORTD
       else if ((uint8_t)(config_coil>>8) == PORTD_CODE) PORTD.DIRSET = (uint8_t)(config_coil & 0xFF); // Set PORTD coil pin as output
       #endif
       #ifdef PORTE
       else if ((uint8_t)(config_coil>>8) == PORTE_CODE) PORTE.DIRSET = (uint8_t)(config_coil & 0xFF); // Set PORTE coil pin as output
       #endif
       #ifdef PORTF
       else if ((uint8_t)(config_coil>>8) == PORTF_CODE) PORTF.DIRSET = (uint8_t)(config_coil & 0xFF); // Set PORTF coil pin as output
       #endif
    }
}

void init_discrete_inputs(void){
    for (uint8_t i = 0; i < g_registers.numOfDiscreteInputs; i++){
        uint16_t config_discrete_input = g_registers.config_discrete_inputs[i];
        if ((uint8_t)(config_discrete_input>>8) == PORTA_CODE) PORTA.DIRCLR = (uint8_t)(config_discrete_input & 0xFF); // Set PORTA input discrete pin as input
        else if ((uint8_t)(config_discrete_input>>8) == PORTB_CODE) PORTB.DIRCLR = (uint8_t)(config_discrete_input & 0xFF); // Set PORTB input discrete pin as input 
        else if ((uint8_t)(config_discrete_input>>8) == PORTB_CODE) PORTB.DIRCLR = (uint8_t)(config_discrete_input & 0xFF); // Set PORTB input discrete pin as input 
        #ifdef PORTC
        else if ((uint8_t)(config_discrete_input>>8) == PORTC_CODE) PORTC.DIRCLR = (uint8_t)(config_discrete_input & 0xFF); // Set PORTC input discrete pin as input 
        #endif
        #ifdef PORTD
        else if ((uint8_t)(config_discrete_input>>8) == PORTD_CODE) PORTD.DIRCLR = (uint8_t)(config_discrete_input & 0xFF); // Set PORTD input discrete pin as input 
        #endif
        #ifdef PORTE
        else if ((uint8_t)(config_discrete_input>>8) == PORTE_CODE) PORTE.DIRCLR = (uint8_t)(config_discrete_input & 0xFF); // Set PORTE input discrete pin as input 
        #endif
        #ifdef PORTF
        else if ((uint8_t)(config_discrete_input>>8) == PORTF_CODE) PORTF.DIRCLR = (uint8_t)(config_discrete_input & 0xFF); // Set PORTF input discrete pin as input 
        #endif
    }
}

void init_holding_registers(void){
for (uint8_t i = 0; i <  g_registers.numOfHoldingRegisters; i++){
        const uint16_t config_holding_register = g_registers.config_holding_registers[i];
        const uint8_t code = (uint8_t)(config_holding_register >> 8);
        if (code == PWM_CODE){
            const uint8_t channel = (uint8_t)(config_holding_register & 0xFF);
            pwm_set_frequency(DEFAULT_PWM_FREQUENCY);
            if (channel==0){
                g_registers.holding_registers[i] = DEFAULT_PWM_FREQUENCY;               
            }
            if (channel>0){
                pwm_set_duty(channel, DEFAULT_PWM_DUTY);
                g_registers.holding_registers[i] = DEFAULT_PWM_DUTY;
            }
        }
    }
}


