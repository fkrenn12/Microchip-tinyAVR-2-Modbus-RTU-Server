#ifndef _MAIN_H
#define _MAIN_H

#include "avr/io.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>

#include "config.h"
#include "uart.h"
#include "modbusServer.h"
#include "adc.h"
#include "pwm.h"
#include "eeprom_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVT_UPDATE_COILS  (1u << 0)  // Bit 0 in GPIOR0 reserved for update coils
#define EVT_UPDATE_HOLDING_REGISTER  (1u << 1)  // Bit 1 in GPIOR0 reserved for update holding registers

#ifdef GPIOR0
static inline void gpior0_set_bit(uint8_t bit) {
    asm volatile ("sbi %0, %1" :: "I" (_SFR_IO_ADDR(GPIOR0)), "I" (bit));
}
static inline void gpior0_clear_bit(uint8_t bit) {
    asm volatile ("cbi %0, %1" :: "I" (_SFR_IO_ADDR(GPIOR0)), "I" (bit));
}
#else
// MPLAB XC8 compiler does not support GPIOR0, so it uses GPR.GPR0
static inline void gpior0_set_bit(uint8_t bit) {
    asm volatile ("sbi %0, %1" :: "I" (_SFR_IO_ADDR(GPR.GPR0)), "I" (bit));
}
static inline void gpior0_clear_bit(uint8_t bit) {
    asm volatile ("cbi %0, %1" :: "I" (_SFR_IO_ADDR(GPR.GPR0)), "I" (bit));
}
#endif


#ifdef __cplusplus
}
#endif
#endif