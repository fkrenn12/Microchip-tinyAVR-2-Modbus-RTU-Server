#ifndef _MAIN_H
#define _MAIN_H

#include "avr/io.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>

#include "config.h"
#include "uart.h"
#include "modbus_rtu/server.h"
#include "adc.h"
#include "pwm.h"
#include "configuration.h"

#ifdef __cplusplus
extern "C" {
#endif

void update_peripheral(void);
void update_input_discretes(void);
void update_coils(void);
void update_holding_registers(void);
void init_peripheral(void);

#ifdef __cplusplus
}
#endif
#endif