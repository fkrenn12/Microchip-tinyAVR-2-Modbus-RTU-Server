#ifndef ADC_H
#define ADC_H

// adc.h - ADC (Analog to Digital Converter) interface header
// Author: Franz Krenn
// Date: 2025-10-02
// Description: Declarations for ADC initialization and reading functions.

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "register.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t input_registers[];
extern uint8_t numOfInputRegisters;
extern uint16_t config_input_registers[];

void init_adc(void) ;
void adc_sequencer(void);

#ifdef __cplusplus
}
#endif

#endif // ADC_H