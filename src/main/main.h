#pragma once

#include "avr/io.h"
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdint.h>

#include "config.h"
#include "uart.h"
#include "modbus_rtu/server.h"
#include "adc.h"
#include "pwm.h"
#include "configuration.h"
#include "register.h"
#include "update_register.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_peripheral(void);
/*
void update_peripheral(void);
void update_input_discretes(void);
void update_coils(void);
void update_holding_registers(void);
*/


#ifdef __cplusplus
}
#endif
