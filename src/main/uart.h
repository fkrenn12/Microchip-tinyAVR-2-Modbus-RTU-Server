#ifndef UART_H
#define UART_H
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "modbus_rtu/server.h"
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
// Maximum size of a Modbus RTU frame is 256 bytes (including address, function code, data, and CRC).
#define UART_BUFFER_SIZE 256 // size of the UART buffer

#ifdef __cplusplus
extern "C" {
#endif


void init_uart0(uint32_t baudrate);
void init_tca0_us(uint16_t us);
void init_tcb0_us(uint16_t us);
void uartSendPacket(uint8_t* buffer , uint16_t len);
#ifdef __cplusplus
}
#endif

#endif
