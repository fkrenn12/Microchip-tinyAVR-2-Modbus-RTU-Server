#include "peripheral.h"

void init_coils(uint16_t* config_coils, uint8_t sizeOfConfigCoils){
    // Init coils
    for (uint8_t i = 0; i < sizeOfConfigCoils/sizeof(uint16_t); i++){
       uint16_t config_coil = config_coils[i];
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
void init_discrete_inputs(uint16_t* config_discrete_inputs, uint8_t sizeOfConfigDiscretesInputs){
    for (uint8_t i = 0; i < sizeOfConfigDiscretesInputs/sizeof(uint16_t); i++){
        uint16_t config_discrete_input = config_discrete_inputs[i];
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

void init_holding_registers(uint16_t* config_holding_registers, uint8_t sizeOfConfigHoldingRegisters, uint16_t* holding_registers){
for (uint8_t i = 0; i < sizeOfConfigHoldingRegisters/sizeof(uint16_t); i++){
        const uint16_t config_holding_register = config_holding_registers[i];
        const uint8_t code = (uint8_t)(config_holding_register >> 8);
        if (code == PWM_CODE){
            const uint8_t channel = (uint8_t)(config_holding_register & 0xFF);
            pwm_set_frequency(DEFAULT_PWM_FREQUENCY);
            if (channel==0){
                holding_registers[i] = DEFAULT_PWM_FREQUENCY;               
            }
            if (channel>0){
                pwm_set_duty(channel, DEFAULT_PWM_DUTY);
                holding_registers[i] = DEFAULT_PWM_DUTY;
            }
        }
    }
}