#include "update_register.h"

extern ModbusAppRegisters g_registers;

void init_update_callbacks(void){
    // set callbacks    
    modbus_set_update_coils_callback(update_coils);

    modbus_set_update_holding_registers_callback(update_holding_registers);
    modbus_set_update_input_registers_callback(update_input_registers);
    modbus_set_update_input_discretes_callback(update_discrete_inputs);  
}
void update_holding_registers(void){
    for (uint8_t i = 0; i < g_registers.numOfHoldingRegisters; i++){
        const uint16_t config_holding_register = g_registers.config_holding_registers[i];
        const uint8_t code = (uint8_t)(config_holding_register >> 8);
        if (code == PWM_CODE){
            if (config_holding_register == HOLDING_PWM_FREQ){
                pwm_set_frequency(g_registers.holding_registers[i]);
            }
            else{
                const uint8_t channel = (uint8_t)(config_holding_register & 0xFF);
                pwm_set_duty(channel, g_registers.holding_registers[i]);
            }
        }
    }
}

void update_input_registers(void){
  
}

void update_coils(void)
{
    const uint8_t count = (uint8_t)g_registers.numOfCoils;

    for (uint8_t i = 0; i < count; ++i) {
        const uint16_t coil = g_registers.config_coils[i];

        // Validate "I/O pin" entry
        const uint8_t portCode = (uint8_t)(coil >> 8);         // \0x1x = coils on PORTA/PORTB
        const uint8_t code     = (uint8_t)(portCode & 0xF0);   // high nibble is the class
        if (code != IOPIN_CODE) {
            continue;
        }

        const uint8_t pin_bm = (uint8_t)(coil & 0xFFu);
        const uint16_t value = g_registers.coils[i]; // 0 = clear, non-zero = set

        if (portCode == PORTA_CODE) {
            if (value) {
                PORTA.OUTSET = pin_bm;
            } else {
                PORTA.OUTCLR = pin_bm;
            }
        } else if (portCode == PORTB_CODE) {
            if (value) {
                PORTB.OUTSET = pin_bm;
            } else {
                PORTB.OUTCLR = pin_bm;
            }
        } else if (portCode == PORTC_CODE) {
            if (value) {
                #ifdef PORTC
                PORTC.OUTSET = pin_bm;
                #endif
            } else {
                #ifdef PORTC
                PORTC.OUTCLR = pin_bm;
                #endif
            }
        }
        else if (portCode == PORTD_CODE) {
            if (value) {
                #ifdef PORTD
                PORTD.OUTSET = pin_bm;
                #endif
            } else {
                #ifdef PORTD
                PORTD.OUTCLR = pin_bm;
                #endif
            }
        }
        else if (portCode == PORTE_CODE) {
            if (value) {
                #ifdef PORTE
                PORTE.OUTSET = pin_bm;
                #endif
            } else {
                #ifdef PORTE
                PORTE.OUTCLR = pin_bm;
                #endif
            }
        }
        else if (portCode == PORTF_CODE) {
            if (value) {
                #ifdef PORTF
                PORTC.OUTSET = pin_bm;
                #endif
            } else {
                #ifdef PORTF
                PORTC.OUTCLR = pin_bm;
                #endif
            }
        }
        else {
            // Unknown port code for a coil entry; ignore safely
            continue;
        }
    }
}

void update_discrete_inputs(void)
{
    // Update input discretes
    const uint8_t count = (uint8_t)g_registers.numOfDiscreteInputs;

    for (uint8_t i = 0; i < count; i++) {
        const uint16_t discrete_input = g_registers.config_discrete_inputs[i];

        // Validate "I/O pin" entry
        const uint8_t portCode = (uint8_t)(discrete_input >> 8);       // \0x2x = input discretes on PORTx
        const uint8_t code     = (uint8_t)(portCode & 0xF0);           // high nibble is the class
        if (code != IOPIN_CODE) {
            continue;
        }

        const uint8_t pin_bm = (uint8_t)(discrete_input & 0xFFu);
        uint8_t pin_val = 0u;

        if (portCode == PORTA_CODE) {
            #ifdef PORTA
            pin_val = (uint8_t)((PORTA.IN & pin_bm) ? 1u : 0u);
            #endif
        } else if (portCode == PORTB_CODE) {
            #ifdef PORTB
            pin_val = (uint8_t)((PORTB.IN & pin_bm) ? 1u : 0u);
            #endif
        } else if (portCode == PORTC_CODE) {
            #ifdef PORTC
            pin_val = (uint8_t)((PORTC.IN & pin_bm) ? 1u : 0u);
            #endif
        } else if (portCode == PORTD_CODE) {
            #ifdef PORTD
            pin_val = (uint8_t)((PORTD.IN & pin_bm) ? 1u : 0u);
            #endif
        } else if (portCode == PORTE_CODE) {
            #ifdef PORTE
            pin_val = (uint8_t)((PORTE.IN & pin_bm) ? 1u : 0u);
            #endif
        } else if (portCode == PORTF_CODE) {
            #ifdef PORTF
            pin_val = (uint8_t)((PORTF.IN & pin_bm) ? 1u : 0u);
            #endif
        } else if (portCode == PORTG_CODE) {
            #ifdef PORTF
            pin_val = (uint8_t)((PORTG.IN & pin_bm) ? 1u : 0u);
            #endif
        } else {
            // Unknown port code; ignore entry
            continue;
        }
        g_registers.config_discrete_inputs[i] = pin_val;
    }
}