#include <avr/io.h>
#include "inttypes.h"
#include "defines.h"
#include "config.h"
#pragma once

#if defined (__AVR_ATtiny424__) || (__AVR_ATtiny824__) || defined(__AVR_ATtiny1624__) || defined(__AVR_ATtiny3224__)
#define DEVICE_PIN14
#define ATTINY
#endif
#if defined (__AVR_ATtiny426__) || (__AVR_ATtiny826__) || defined(__AVR_ATtiny1626__) || defined(__AVR_ATtiny3226__)
#define DEVICE_PIN20
#define ATTINY
#endif
#if defined (__AVR_ATtiny427__) || (__AVR_ATtiny827__) || defined(__AVR_ATtiny1627__) || defined(__AVR_ATtiny3227__)
#define DEVICE_PIN24
#define ATTINY
#endif
#if defined (__AVR_AVR128DB64__)
#define AVR
#endif

// COILS and INPUT_DISCRETES 
#define PA3 (PORTA_CODE<<8)+PIN3_bm
#define PA4 (PORTA_CODE<<8)+PIN4_bm
#define PA5 (PORTA_CODE<<8)+PIN5_bm
#define PA6 (PORTA_CODE<<8)+PIN6_bm
#define PA7 (PORTA_CODE<<8)+PIN7_bm
#define PB0 (PORTB_CODE<<8)+PIN0_bm
#define PB1 (PORTB_CODE<<8)+PIN1_bm
#define PB2 (PORTB_CODE<<8)+PIN2_bm
#define PB3 (PORTB_CODE<<8)+PIN3_bm

// HOLDING_REGISTERS
#define HOLDING_PWM_FREQ (PWM_CODE<<8)
#define HOLDING_PWM_PB0_DUTY (PWM_CODE<<8)+1
#define HOLDING_PWM_PB1_DUTY (PWM_CODE<<8)+2
#define HOLDING_PWM_PB2_DUTY (PWM_CODE<<8)+3

// INPUT_REGISTERS 
#define INPUT_ADC_PA3 (ADC_SINGLE_ENDED_CODE<<8)+ADC_MUXPOS_AIN3_gc	
#define INPUT_ADC_PA4 (ADC_SINGLE_ENDED_CODE<<8)+ADC_MUXPOS_AIN4_gc	
#define INPUT_ADC_PA5 (ADC_SINGLE_ENDED_CODE<<8)+ADC_MUXPOS_AIN5_gc	
#define INPUT_ADC_PA6 (ADC_SINGLE_ENDED_CODE<<8)+ADC_MUXPOS_AIN6_gc	
#define INPUT_ADC_PA7 (ADC_SINGLE_ENDED_CODE<<8)+ADC_MUXPOS_AIN7_gc	
#define INPUT_ADC_PB0 (ADC_SINGLE_ENDED_CODE<<8)+ADC_MUXPOS_AIN11_gc	
#define INPUT_ADC_PB1 (ADC_SINGLE_ENDED_CODE<<8)+ADC_MUXPOS_AIN10_gc	

#define INPUT_ADC_PA4_PA5 (ADC_DIFF_CODE<<8)+PA4_PA5
#define INPUT_ADC_PA6_PA7 (ADC_DIFF_CODE<<8)+PA6_PA7
#define INPUT_ADC_ACREF_PA4 (ADC_DIFF_CODE<<8)+ACREF_PA4
#define INPUT_ADC_ACREF_PA5 (ADC_DIFF_CODE<<8)+ACREF_PA5
#define INPUT_ADC_ACREF_PA6 (ADC_DIFF_CODE<<8)+ACREF_PA6
#define INPUT_ADC_ACREF_PA7 (ADC_DIFF_CODE<<8)+ACREF_PA7
#define INPUT_ADC_ACREF_PB0 (ADC_DIFF_CODE<<8)+ACREF_PB0
#define INPUT_ADC_ACREF_PB1 (ADC_DIFF_CODE<<8)+ACREF_PB1

#if defined (DEVICE_PIN20) || defined(DEVICE_PIN24) 
#define PB4 (PORTB_CODE<<8)+PIN4_bm
#define PB5 (PORTB_CODE<<8)+PIN5_bm
#define RC0 (PORTC_CODE<<8)+PIN0_bm
#define RC1 (PORTC_CODE<<8)+PIN1_bm
#define RC2 (PORTC_CODE<<8)+PIN2_bm
#define RC3 (PORTC_CODE<<8)+PIN3_bm
#define INPUT_ADC_PB4 (ADC_CODE<<8)+ADC_MUXPOS_AIN9_gc	
#define INPUT_ADC_PB5 (ADC_CODE<<8)+ADC_MUXPOS_AIN8_gc	
#define INPUT_ADC_PC0 (ADC_CODE<<8)+ADC_MUXPOS_AIN12_gc	
#define INPUT_ADC_PC1 (ADC_CODE<<8)+ADC_MUXPOS_AIN13_gc	
#define INPUT_ADC_PC2 (ADC_CODE<<8)+ADC_MUXPOS_AIN14_gc	
#define INPUT_ADC_PC3 (ADC_CODE<<8)+ADC_MUXPOS_AIN15_gc	
#endif
#if defined (DEVICE_PIN24)  
#define PB6 (PORTB_CODE<<8)+PIN6_bm
#define PB7 (PORTB_CODE<<8)+PIN7_bm
#define RC4 (PORTC_CODE<<8)+PIN4_bm
#define RC5 (PORTC_CODE<<8)+PIN5_bm
#endif

