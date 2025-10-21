#ifdef __cplusplus
#include "Arduino.h"
#endif
#include "main.h"
////////////////////////////////////////////////////////////////////////
// GLOBAL USED VARIABLES
////////////////////////////////////////////////////////////////////////
extern Configuration configuration;

// rename to discrete_inputs
uint16_t config_input_discretes[] = INPUT_DISCRETES; 
uint16_t input_discretes[sizeof(config_input_discretes)/sizeof(uint16_t)];
uint8_t numOfDiscreteInputs= sizeof(config_input_discretes)/sizeof(uint16_t);

uint16_t config_coils[] = COILS; 
uint16_t coils[sizeof(config_coils)/sizeof(uint16_t)];
uint8_t numOfCoils = sizeof(config_coils)/sizeof(uint16_t);

uint16_t config_input_registers[] = INPUT_REGISTERS; 
uint16_t input_registers[sizeof(config_input_registers)/sizeof(uint16_t)]={0};
uint8_t numOfInputRegisters = sizeof(config_input_registers)/sizeof(uint16_t);

uint16_t config_holding_registers[] = HOLDING_REGISTERS; 
uint16_t holding_registers[sizeof(config_holding_registers)/sizeof(uint16_t)];
uint8_t numOfHoldingRegisters = sizeof(config_holding_registers)/sizeof(uint16_t);
////////////////////////////////////////////////////////////////////////

// ISR for TCB0 compare
ISR(TCB0_INT_vect)
{
  TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
  TCB0.CTRLA &= ~TCB_ENABLE_bm; // Stop timer
  TCB0.CNT = 0; // Reset timer count
  modbus_package_ready();
}

void init_peripheral(void){
    init_coils(config_coils,sizeof(config_coils));
    init_discrete_inputs(config_input_discretes,sizeof(config_input_discretes));
    init_holding_registers(config_holding_registers,sizeof(config_holding_registers), holding_registers);   
}

void update_holding_registers(void){
    for (uint8_t i = 0; i < sizeof(config_holding_registers)/sizeof(uint16_t); i++){
        const uint16_t config_holding_register = config_holding_registers[i];
        const uint8_t code = (uint8_t)(config_holding_register >> 8);
        if (code == PWM_CODE){
            if (config_holding_register == HOLDING_PWM_FREQ){
                pwm_set_frequency(holding_registers[i]);
            }
            else{
                const uint8_t channel = (uint8_t)(config_holding_register & 0xFF);
                pwm_set_duty(channel, holding_registers[i]);
            }
        }
    }
}

void update_input_registers(void){
  
}

void update_coils(void)
{
    const uint8_t count = (uint8_t)(sizeof(config_coils) / sizeof(uint16_t));

    for (uint8_t i = 0; i < count; ++i) {
        const uint16_t coil = config_coils[i];

        // Validate "I/O pin" entry
        const uint8_t portCode = (uint8_t)(coil >> 8);         // \0x1x = coils on PORTA/PORTB
        const uint8_t code     = (uint8_t)(portCode & 0xF0);   // high nibble is the class
        if (code != IOPIN_CODE) {
            continue;
        }

        const uint8_t pin_bm = (uint8_t)(coil & 0xFFu);
        const uint16_t value = coils[i]; // 0 = clear, non-zero = set

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

void update_input_discretes(void)
{
    // Update input discretes
    const uint8_t count = (uint8_t)(sizeof(config_input_discretes) / sizeof(uint16_t));

    for (uint8_t i = 0; i < count; i++) {
        const uint16_t input_discrete = config_input_discretes[i];

        // Validate "I/O pin" entry
        const uint8_t portCode = (uint8_t)(input_discrete >> 8);       // \0x2x = input discretes on PORTx
        const uint8_t code     = (uint8_t)(portCode & 0xF0);           // high nibble is the class
        if (code != IOPIN_CODE) {
            continue;
        }

        const uint8_t pin_bm = (uint8_t)(input_discrete & 0xFFu);
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
        input_discretes[i] = pin_val;
    }
}

void update_configuration(void){
    // store modbus id and baudrate in eeprom configuration registers
    const uint8_t id=(uint8_t)(configuration.registers[CFG_MODBUS_ID] & 0xFF);
    configuration.uart_baudrate=(uint32_t)(configuration.registers[CFG_UART_BAUDRATE])*100;
    modbus_set_id(id);
    store_modbus_id_to_eeprom(id);
    store_baudrate_to_eeprom(configuration.uart_baudrate);
    _delay_ms(10);
    asm volatile("jmp 0"); // we have to jump to restart
    // reinit uart with new baudrate
    // init_uart0(configuration.uart_baudrate);  // unlikely  do not work !!
}

#ifndef ARDUINO
    int main() {
#else
    void loop(){}
    void setup(){
#endif
    handle_reset_enable_pin(); // will restart here if reset pin activated and set to high
    // configure alt uart pins    
    PORTMUX.USARTROUTEA = PORTMUX_USART1_NONE_gc | PORTMUX_USART0_ALT1_gc;
    const uint8_t id = load_modbus_id_from_eeprom();
    configuration.uart_baudrate = load_baudrate_from_eeprom();
    configuration.registers[CFG_MODBUS_ID] = (uint16_t)id;
    configuration.registers[CFG_UART_BAUDRATE] = (uint16_t)(configuration.uart_baudrate/100);
    modbus_set_id(id);
    modbus_set_send_package_callback(uartSendPacket);
    // link registers to modbus
    modbus_set_coils_registers(coils, numOfCoils);
    modbus_set_input_registers(input_registers, numOfInputRegisters);
    modbus_set_discrete_inputs_registers(input_discretes, numOfDiscreteInputs);
    modbus_set_holding_registers(holding_registers, numOfHoldingRegisters);
    modbus_set_configuration_registers(configuration.registers, configuration.sizeOfConfigurationRegisters);
    // set callbacks    
    modbus_set_update_coils_callback(update_coils);
    modbus_set_update_configuration_callback(update_configuration);
    modbus_set_update_holding_registers_callback(update_holding_registers);
    modbus_set_update_input_registers_callback(update_input_registers);
    modbus_set_update_input_discretes_callback(update_input_discretes);
   
    init_peripheral();
    init_adc();
    init_tcb0_us(T1_5us(configuration.uart_baudrate));
    init_uart0(configuration.uart_baudrate);

    sei();
    
    while (1){
        adc_sequencer();
        // because modbus_package_ready(); is called from ISR (TCB_CAPT)
        // we must disable appropiate interrupt temporarely
        TCB0.INTCTRL &= ~TCB_CAPT_bm; // disable tcb interrupt
        const uint8_t updateFlag = modbus_need_update();
        TCB0.INTCTRL = TCB_CAPT_bm; // enable tcb interrupt
        if (!updateFlag) continue;
        modbus_update();
    }
}
