#ifndef _CONFIG_H
#define _CONFIG_H    

#include "peripheral.h"

// *** Modbus-ID ***
#define MODBUS_ID 3 // 0:       address set by pymcuprog (eeprom) or modbus_id=254
                    // 1-247:   initial unique address until reprogrammed

// *** Low latency mode ***
// Low latency is not a defined modbus standard  feature.
// Setting to 1 will enable the low latency mode and will decrease the modbus response time
// from approx. 1ms to 0.2ms
#define MODBUS_LOW_LATENCY_MODE 1  // 0 or 1

// *** Baudrate settings ***
// Common settings: 9600 and 19200 bps
// Any value is possible, but cable length, transmission medium, and other MODBUS devices 
// must be taken into account when choosing the value.
#define UART_BAUDRATE (uint32_t)1000000      // this is the default value
                                    // reprogrammable by pymcuprog (eeprom) or modbus_id=254

// *** TX Enable pin ***
#define UART_TX_ENABLE_PORT PORTA   // Set to the port where your Tx Enable pin is connected 
                                    // valid values: PORTA, PORTB, PORTC, PORTD, PORTE, PORTF, ...
#define UART_TX_ENABLE_PIN 0        // 0: no TX_ENABLE_PIN implemented.
                                    // valid values: PIN0_bm, PIN1_bm, PIN2_bm, PIN3_bm, PIN4_bm, PIN5_bm, PIN6_bm, PIN7_bm

// *** Reset pin ***
#define RESET_ENABLE_PORT PORTA     // Set to the port where your Reset pin is connected 
                                    // valid values: PORTA, PORTB, PORTC, PORTD, PORTE, PORTF, ...
#define RESET_ENABLE_PIN 0          // 0: no RESET_ENABLE_PIN implemented. 
                                    // valid values: PIN0_bm, PIN1_bm, PIN2_bm, PIN3_bm, PIN4_bm, PIN5_bm, PIN6_bm, PIN7_bm

// *** ADC Reference voltage ***
// ADC_REF_1024MV, ADC_REF_2048MV, ADC_REF_4096MV, ADC_REF_2500MV, ADC_REF_VCC, ADC_REF_VREFA
#define ADC_REFERENCE ADC_REF_4096MV // ADC reference voltage

// *** AC Reference voltage with divider ***
// The AC reference voltage can be used as minus of differential ADC input.
// AC_REF_2500MV, AC_REF_1024MV, AC_REF_2048MV, AC_REF_4096MV, AC_REF_VDD
#define AC_REFERENCE AC_REF_4096MV // AC reference
// Calculating: voltage = AC_REF_xxxxxx * AC_REF_DIVIDER/256
#define AC_REFERENCE_DIVIDER 128 // AC_REF_xxxxxx * 0.5

// *** PWM defaults at microcontroller reset ***
#define DEFAULT_PWM_FREQUENCY 2500 // Hz
#define DEFAULT_PWM_DUTY 0 // duty cycle = 0%

// ***********************************************************************
// *** Associating MODBUS Data model to microcontrollers peripheral    ***
// ***********************************************************************
// BE CAREFULLY - do not define multiple function on same io port pin

// define single pins for reading the level (0 or 1)
#define DISCRETE_INPUTS {PA3}  
// define single pins for writing the level (0 or 1)
#define COILS {PB3}
// define holding registers
#define HOLDING_REGISTERS {HOLDING_PWM_FREQ, HOLDING_PWM_PB0_DUTY, HOLDING_PWM_PB1_DUTY, HOLDING_PWM_PB2_DUTY}
// define input registers
#define INPUT_REGISTERS {INPUT_ADC_PA4_PA5, INPUT_ADC_ACREF_PA6, INPUT_ADC_ACREF_PA7}

#endif // CONFIG_HF