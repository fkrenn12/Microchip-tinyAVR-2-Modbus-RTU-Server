#include <avr/io.h>
#include "inttypes.h"
#ifndef _HARDWARE_H
#define _HARDWARE_H


#define BIT_POSITION(x) (__builtin_ctz(x))
#define EEPROM_OFFSET_MODBUS_ID (uint8_t)0x00 // Offset in user row where the modbus_id is stored
#define EEPROM_OFFSET_BAUDRATE (uint8_t)0x01 // Offset in user row where the baudrate isstored
// #define EEPROM_OFFSET_MODBUS_SERVER_NAME (uint8_t)0x03 // Offset in user row where the modbus server name is stored
// #define MAX_SERVERNAME_LENGTH 32 // Maximum length of the modbus server name

#define IOPIN_CODE 0x10
#define PWM_CODE 0x20
#define ADC_SINGLE_ENDED_CODE 0x30
#define ADC_DIFF_CODE 0x40

// *** Reference Voltage for ADC ***
#define ADC_REF_2500MV ADC_REFSEL_2500MV_gc
#define ADC_REF_1024MV ADC_REFSEL_1024MV_gc
#define ADC_REF_2048MV ADC_REFSEL_2048MV_gc
#define ADC_REF_4096MV ADC_REFSEL_4096MV_gc
#define ADC_REF_VDD ADC_REFSEL_VDD_gc
#define ADC_REF_VREFA ADC_REFSEL_VREFA_gc

// *** Reference Voltage for Analog Comperator ***
#define AC_REF_2500MV VREF_AC0REFSEL_2V5_gc
#define AC_REF_1024MV VREF_AC0REFSEL_1V024_gc
#define AC_REF_2048MV VREF_AC0REFSEL_2V048_gc
#define AC_REF_4096MV VREF_AC0REFSEL_4V096_gc
#define AC_REF_VDD VREF_AC0REFSEL_AVDD_gc

#define PORTA_CODE IOPIN_CODE
#define PORTB_CODE (IOPIN_CODE+1)
#define PORTC_CODE (IOPIN_CODE+2)
#define PORTD_CODE (IOPIN_CODE+3)
#define PORTE_CODE (IOPIN_CODE+4)
#define PORTF_CODE (IOPIN_CODE+5)
#define PORTG_CODE (IOPIN_CODE+6)

#define PA4_PA5 1
#define PA6_PA7 2
#define ACREF_PA4 4
#define ACREF_PA5 5
#define ACREF_PA6 6
#define ACREF_PA7 7
#define ACREF_PB0 8
#define ACREF_PB1 9

#endif