#include "uart.h"

// Function to initialize UART with specified baud rate
void init_uart0(uint32_t baudrate)
{
  if ((PORTMUX.USARTROUTEA & 0x03) == PORTMUX_USART0_ALT1_gc){
    PORTA.DIRSET = PIN1_bm; // Set TX (RA1) as output
  }
  else PORTB.DIRSET = PIN2_bm; // Set TX (RB2) as output
  #if (UART_TX_ENABLE_PIN > 0)
      UART_TX_ENABLE_PORT.DIRSET = UART_TX_ENABLE_PIN; // Set TxEnablePin as output
      UART_TX_ENABLE_PORT.OUTCLR = UART_TX_ENABLE_PIN; // Set TxEnablePin low
  #endif
  USART0.BAUD = (uint16_t)USART0_BAUD_RATE(baudrate); // Set baud rate
  USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
  USART0.CTRLA = USART_RXCIE_bm; // Enable RX Complete Interrupt
}

ISR(USART0_RXC_vect) {
    uint8_t data = USART0.RXDATAL; // Read received byte
    modbus_char_received(data);
    TCB0.CNT = 0; // Reset timer count
    TCB0.CTRLA |= TCB_ENABLE_bm;
}

// ISR for TCB0 compare
ISR(TCB0_INT_vect)
{
  TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
  TCB0.CTRLA &= ~TCB_ENABLE_bm; // Stop timer
  TCB0.CNT = 0; // Reset timer count
  modbus_package_ready();
}

// Initialize TCB0 timer to trigger compare interrupt after 'us' microseconds
void init_tcb0_us(uint16_t us)
{
  if (us > 6553) us = 6553; // Limit to maximum value for 16-bit timer
  TCB0.CTRLA = 0; // Stop timer
  TCB0.CNT = 0; // Reset timer count
  TCB0.CCMP = (uint16_t)(us * 10); // Set compare value for desired microseconds
  TCB0.CTRLA = TCB_CLKSEL_DIV2_gc; // Use clock prescaler DIV2 (10MHz Timer clock@20Mhz systeme clock) and enable timer
  TCB0.INTCTRL = TCB_CAPT_bm; // Enable compare interrupt
  TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
}

void uart_sendPacket(uint8_t* buffer , uint16_t len)
{   
    #if (UART_TX_ENABLE_PIN > 0)
      UART_TX_ENABLE_PORT.OUTSET = UART_TX_ENABLE_PIN; // Set TxEnablePin high
    #endif

    USART0.CTRLB &= ~USART_RXEN_bm; // Disable receiver during transmission
    _delay_us(1); // allow line to stabilize before transmitting (optional, may depend on hardware)
    
    for (uint8_t i = 0; i < len; i++){
        while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for empty transmit buffer
        USART0.TXDATAL = buffer[i];
    }
    while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for empty transmit buffer
    while (!(USART0.STATUS & USART_TXCIF_bm)); // Wait for transmission to complete
    USART0.STATUS  |= USART_TXCIF_bm; // It is cleared by writing a ‘1’ to it

    #if (UART_TX_ENABLE_PIN > 0)
    UART_TX_ENABLE_PORT.OUTCLR = UART_TX_ENABLE_PIN; // Set TxEnablePin low
    #endif
    USART0.CTRLB |= USART_RXEN_bm; // Re-enable receiver
}

uint16_t T3_5us(uint32_t baudrate){
  int16_t T3_5;
  // LOW_LATENCY_MODE, allow lower than 1.75ms inter-frame gap
  // congig.h: #define MODBUS_LOW_LATENCY_MODE 0  // 0 or 1
  uint8_t lowLatency = MODBUS_LOW_LATENCY_MODE;
  if (lowLatency){ 
    // this is NOT standard compliant, but allows faster frame rates for high baud rates
    T3_5 = 18000000/baudrate; 
    if (T3_5 < 75) T3_5 = 75; // limit to 75us minimum
  }
  else{
    // this is standard compliant
    T3_5 = 35000000/baudrate; 
    // The inter-frame gap is limit 1.75 ms minimum (= 1750 us)
    if (T3_5 < 1750) T3_5 = 1750;
  }
  return T3_5;
}

// T1_5us is not used in this implementation but it is provided for completeness. 
// It calculates the inter-character timeout based on the baud rate, following the modbus specification. 
uint16_t T1_5us(uint32_t baudrate){
  uint16_t T1_5 = 15000000/baudrate; // // defined in modbus standard 1.5T = 1T * 1.5
  return T1_5;
}

