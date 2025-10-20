#ifdef __cplusplus
#include "Arduino.h"
#endif
#include "main.h"

////////////////////////////////////////////////////////////////////////
// GLOBAL USED VARIABLES
////////////////////////////////////////////////////////////////////////

extern Configuration configuration;

#ifdef INPUT_DISCRETES
uint16_t config_input_discretes[] = INPUT_DISCRETES; // function 2 register array
uint16_t input_discretes[sizeof(config_input_discretes)/sizeof(uint16_t)];
uint8_t numOfDiscreteInputs= sizeof(config_input_discretes)/sizeof(uint16_t);
#endif
#ifdef COILS
uint16_t config_coils[] = COILS; // function 5 register array
uint16_t coils[sizeof(config_coils)/sizeof(uint16_t)];
uint8_t numOfCoils = sizeof(config_coils)/sizeof(uint16_t);
#endif
#ifdef INPUT_REGISTERS
uint16_t config_input_registers[] = INPUT_REGISTERS; // function 4 register array
uint16_t input_registers[sizeof(config_input_registers)/sizeof(uint16_t)]={0};
uint8_t numOfInputRegisters = sizeof(config_input_registers)/sizeof(uint16_t);
#endif
#ifdef HOLDING_REGISTERS
uint16_t config_holding_registers[] = HOLDING_REGISTERS; // function 3 register array
uint16_t holding_registers[sizeof(config_holding_registers)/sizeof(uint16_t)];
uint8_t numOfHoldingRegisters = sizeof(config_holding_registers)/sizeof(uint16_t);
#endif


/////////////////////////////////////////////////////////////
void update_peripheral(void);
void update_input_discretes(void);
void update_coils(void);
void update_holding_registers(void);
void init_peripheral(void);

// ISR for TCB0 compare
ISR(TCB0_INT_vect)
{
  TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
  TCB0.CTRLA &= ~TCB_ENABLE_bm; // Stop timer
  TCB0.CNT = 0; // Reset timer count
  modbus_package_ready();
}

void system_reset_via_rstctrl(void) {
    cli();
    // Write signature + SWRR (software reset request)
    // Datasheet: write '0x1D' to the upper bits with SWRR bit set.
    RSTCTRL.SWRR = RSTCTRL_SWRE_bm; // Some MCUs name it SWRE/SWRST; check your device header
    for (;;) { /* wait for reset */ }
}

void init_peripheral(void){
    // Init coils
    #ifdef COILS
    for (uint8_t i = 0; i < sizeof(config_coils)/sizeof(uint16_t); i++){
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
    #endif
    // Init input discretes
    #ifdef INPUT_DISCRETES
    for (uint8_t i = 0; i < sizeof(config_input_discretes)/sizeof(uint16_t); i++){
        uint16_t config_input_discrete = config_input_discretes[i];
        if ((uint8_t)(config_input_discrete>>8) == PORTA_CODE) PORTA.DIRCLR = (uint8_t)(config_input_discrete & 0xFF); // Set PORTA input discrete pin as input
        else if ((uint8_t)(config_input_discrete>>8) == PORTB_CODE) PORTB.DIRCLR = (uint8_t)(config_input_discrete & 0xFF); // Set PORTB input discrete pin as input 
        else if ((uint8_t)(config_input_discrete>>8) == PORTB_CODE) PORTB.DIRCLR = (uint8_t)(config_input_discrete & 0xFF); // Set PORTB input discrete pin as input 
        #ifdef PORTC
        else if ((uint8_t)(config_input_discrete>>8) == PORTC_CODE) PORTC.DIRCLR = (uint8_t)(config_input_discrete & 0xFF); // Set PORTC input discrete pin as input 
        #endif
        #ifdef PORTD
        else if ((uint8_t)(config_input_discrete>>8) == PORTD_CODE) PORTD.DIRCLR = (uint8_t)(config_input_discrete & 0xFF); // Set PORTD input discrete pin as input 
        #endif
        #ifdef PORTE
        else if ((uint8_t)(config_input_discrete>>8) == PORTE_CODE) PORTE.DIRCLR = (uint8_t)(config_input_discrete & 0xFF); // Set PORTE input discrete pin as input 
        #endif
        #ifdef PORTF
        else if ((uint8_t)(config_input_discrete>>8) == PORTF_CODE) PORTF.DIRCLR = (uint8_t)(config_input_discrete & 0xFF); // Set PORTF input discrete pin as input 
        #endif
    }
    #endif

    // Init holding registers
    #ifdef HOLDING_REGISTERS
    for (uint8_t i = 0; i < sizeof(config_holding_registers)/sizeof(uint16_t); i++){
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
    #endif

}

#ifdef HOLDING_REGISTERS
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
    adc_sequencer();
}

#endif
#ifdef COILS
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
#endif

#ifdef INPUT_DISCRETES
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
#endif

#ifndef ARDUINO
    int main() {
#else
    void loop(){}
    void setup(){
#endif
    #if (RESET_ENABLE_PIN > 0)
        // set as input enable pullup
        // check RESET_ENABLE_PIN should be done here !!
        RESET_ENABLE_PORT.DIRCLR = RESET_ENABLE_PIN;
        if (RESET_ENABLE_PIN==PIN0_bm) RESET_ENABLE_PORT.PIN0CTRL = PORT_PULLUPEN_bm; 
        else if (RESET_ENABLE_PIN==PIN1_bm) RESET_ENABLE_PORT.PIN1CTRL = PORT_PULLUPEN_bm; 
        else if (RESET_ENABLE_PIN==PIN2_bm) RESET_ENABLE_PORT.PIN2CTRL = PORT_PULLUPEN_bm;
        else if (RESET_ENABLE_PIN==PIN3_bm) RESET_ENABLE_PORT.PIN3CTRL = PORT_PULLUPEN_bm;
        else if (RESET_ENABLE_PIN==PIN4_bm) RESET_ENABLE_PORT.PIN4CTRL = PORT_PULLUPEN_bm;
        else if (RESET_ENABLE_PIN==PIN5_bm) RESET_ENABLE_PORT.PIN5CTRL = PORT_PULLUPEN_bm;
        else if (RESET_ENABLE_PIN==PIN6_bm) RESET_ENABLE_PORT.PIN6CTRL = PORT_PULLUPEN_bm;
        else if (RESET_ENABLE_PIN==PIN7_bm) RESET_ENABLE_PORT.PIN7CTRL = PORT_PULLUPEN_bm;
        _delay_ms(1);
        // read status
        if (RESET_ENABLE_PORT.IN & RESET_ENABLE_PIN == 0) {
            // low detected -> clear baudrate (= set to 0xffffff)
            eeprom_write_byte(EEPROM_OFFSET_BAUDRATE,0xFF); 
            eeprom_write_byte(EEPROM_OFFSET_BAUDRATE+1,0xFF); 
            // wait releasing 
            _delay_ms(10);
            while (RESET_ENABLE_PORT.IN & RESET_ENABLE_PIN == 0);
            _delay_ms(100);
            // reset
            asm volatile ("jmp 0");
        }
        RESET_ENABLE_PORT.PIN0CTRL = 0;
        RESET_ENABLE_PORT.PIN1CTRL = 0;
        RESET_ENABLE_PORT.PIN2CTRL = 0;
        RESET_ENABLE_PORT.PIN3CTRL = 0;
        RESET_ENABLE_PORT.PIN4CTRL = 0;
        RESET_ENABLE_PORT.PIN5CTRL = 0;
        RESET_ENABLE_PORT.PIN6CTRL = 0;
        RESET_ENABLE_PORT.PIN7CTRL = 0;
    #endif
    // configure alt uart pins    
    PORTMUX.USARTROUTEA = PORTMUX_USART1_NONE_gc | PORTMUX_USART0_ALT1_gc;
    const uint8_t id = load_modbus_id_from_eeprom();
    modbus_set_id(id);
    configuration.uart_baudrate = load_baudrate_from_eeprom();
    modbus_set_send_package_callback(uartSendPacket);
    // transfer registers to modbus
    modbus_set_coils_registers(coils, numOfCoils);
    modbus_set_input_registers(input_registers, numOfInputRegisters);
    modbus_set_discrete_inputs_registers(input_discretes, numOfDiscreteInputs);
    modbus_set_holding_registers(holding_registers, numOfHoldingRegisters);
    configuration.registers[CFG_MODBUS_ID] = (uint16_t)id;
    configuration.registers[CFG_UART_BAUDRATE] = (uint16_t)(configuration.uart_baudrate/100);
    modbus_set_configuration_registers(configuration.registers, configuration.sizeOfConfigurationRegisters);
    
    uint16_t T1_5; // 1.5 charcter time
    uint8_t lowLatency = MODBUS_LOW_LATENCY_MODE;
    if (lowLatency && configuration.uart_baudrate >= 115200){
        T1_5 = 75; // not defined in modbus standart
    } else if (configuration.uart_baudrate > 19200){
        T1_5 = 750; // defined in modbus standart
    } else{
        T1_5 = 15000000/configuration.uart_baudrate; // // defined in modbus standart 1.5T = 1T * 1.5
    }
    
    // read from eeprom
    init_peripheral();
    init_adc();
    init_tcb0_us(T1_5);
    init_uart0(configuration.uart_baudrate);

    sei();
    
    while (1){
        TCB0.INTCTRL &= ~TCB_CAPT_bm; // disable tcb interrupt
        const uint8_t updateFlag = modbus_need_update();
        TCB0.INTCTRL = TCB_CAPT_bm; // enable tcb interrupt
        update_input_registers(); // update input registers continuously
        if (!updateFlag) continue;
        if (modbus_need_refresh_discrete_inputs()) update_input_discretes();
        modbus_update();
        if (modbus_need_update_configuration()){
            // store modbus id and baudrate in eeprom configuration registers
            const uint8_t id=(uint8_t)(configuration.registers[CFG_MODBUS_ID] & 0xFF);
            configuration.uart_baudrate=(uint32_t)(configuration.registers[CFG_UART_BAUDRATE])*100;
            modbus_set_id(id);
            store_modbus_id_to_eeprom(id);
            store_baudrate_to_eeprom(configuration.uart_baudrate);
            _delay_ms(10);
            asm volatile("jmp 0"); // we have to jump to restart
            // reinit uart with new baudrate
            // system_reset_via_rstctrl();  // unlikely  do not work !!
            // init_uart0(configuration.uart_baudrate);  // unlikely  do not work !!
        }
        else if (modbus_need_update_holding_registers()) update_holding_registers();
        else if (modbus_need_update_coils()) update_coils();
    }
}
