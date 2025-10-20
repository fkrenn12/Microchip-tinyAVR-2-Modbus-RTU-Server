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

// Initialize TCB0 timer to trigger compare interrupt after 'us' microseconds
void init_tcb0_us(uint16_t us)
{
  TCB0.CTRLA = 0; // Stop timer
  TCB0.CNT = 0; // Reset timer count
  TCB0.CCMP = (uint16_t)(us * 20); // Set compare value for desired microseconds
  TCB0.CTRLA = TCB_CLKSEL_DIV1_gc; // Use clock prescaler DIV1 (20MHz system clock) and enable timer
  TCB0.INTCTRL = TCB_CAPT_bm; // Enable compare interrupt
  TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
}

void uartSendPacket(uint8_t* buffer , uint16_t len)
{
    #if (UART_TX_ENABLE_PIN > 0)
      UART_TX_ENABLE_PORT.OUTSET = UART_TX_ENABLE_PIN; // Set TxEnablePin high
    #endif
    USART0.CTRLB &= ~USART_RXEN_bm; // Disable receiver during transmission
    _delay_us(300);
    
    for (uint8_t i = 0; i < len; i++){
        while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for empty transmit buffer
        USART0.TXDATAL = buffer[i];
    }
    while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for empty transmit buffer
    
    // allow a frame delay to indicate end of transmission
    _delay_us(300);

    #if (UART_TX_ENABLE_PIN > 0)
    UART_TX_ENABLE_PORT.OUTCLR = UART_TX_ENABLE_PIN; // Set TxEnablePin low
    #endif
    USART0.CTRLB |= USART_RXEN_bm; // Re-enable receiver
}


