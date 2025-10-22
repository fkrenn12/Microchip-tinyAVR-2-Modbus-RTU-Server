#ifdef __cplusplus
#include "Arduino.h"
#endif
#include "main.h"
////////////////////////////////////////////////////////////////////////
// GLOBAL USED VARIABLES
////////////////////////////////////////////////////////////////////////
extern Configuration g_configuration;
////////////////////////////////////////////////////////////////////////

// ISR for TCB0 compare
ISR(TCB0_INT_vect)
{
  TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
  TCB0.CTRLA &= ~TCB_ENABLE_bm; // Stop timer
  TCB0.CNT = 0; // Reset timer count
  modbus_package_ready();
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
    init_configuration();
    init_registers();
    init_update_callbacks();
    
    modbus_set_id(load_modbus_id_from_eeprom());
    modbus_set_send_package_callback(uart_sendPacket);
    modbus_set_update_configuration_callback(update_configuration);
    modbus_set_configuration_registers(g_configuration.registers, g_configuration.sizeOfConfigurationRegisters);

    init_adc();
    init_tcb0_us(T1_5us(g_configuration.uart_baudrate));
    init_uart0(g_configuration.uart_baudrate);

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
