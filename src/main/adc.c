#include "adc.h"
// #include <Arduino.h>
// #define DEBUG_ADC_READINGS 
extern ModbusAppRegisters g_registers;

void init_adc() { 
  // Serial1.print("ADC Single initalisation");
  /* Setup AC0 DAC reference */
  VREF.CTRLA = AC_REFERENCE;
  AC0.DACREF = (uint8_t)AC_REFERENCE_DIVIDER; 
  ADC0.CTRLA = 0x00; // ADC disabled before configuration
  ADC0.CTRLB = ADC_PRESC_DIV16_gc; // as fast as possible - MAX FCLK_ADC = 2.5 Mhz
  ADC0.CTRLC = ADC_REFERENCE | (uint8_t)(F_CPU*0.000001) << 3;/*ADC reference 4.096V */
  ADC0.CTRLD = ADC_WINCM_NONE_gc; // Window Comparator disabled
  ADC0.CTRLE = 0x10;	// Sample Duration - controls the input sample duration in ADC clock (CLK_ADC) cycles
  ADC0.CTRLF = ADC_SAMPNUM_ACC16_gc; // | ADC_FREERUN_bm; // Sample number, Free running mode
  ADC0.CTRLA = ADC_ENABLE_bm; // ADC enabled
}

void start_adc_single_ended_conversion(uint8_t channel){
    ADC0.MUXPOS = channel; //channel;	
    ADC0.MUXNEG = ADC_MUXNEG_GND_gc; //channel;	/*ADC channel AIN6->PA6*/
    ADC0.COMMAND = ADC_START_IMMEDIATE_gc | ADC_MODE_SINGLE_12BIT_gc;  // 12bit start conversion  
}

void start_adc_differential_conversion(uint8_t p_channel, uint8_t n_channel){
    ADC0.MUXPOS = p_channel; // Positive input
    ADC0.MUXNEG = n_channel; // Negative input
	ADC0.COMMAND = ADC_DIFF_bm | ADC_START_IMMEDIATE_gc | ADC_MODE_SINGLE_12BIT_gc; /* set in single 12-bit mode */ 
}

void start_adc(uint16_t config)
{
    const uint8_t channel = (uint8_t)(config & 0xFFu);
    const uint8_t mode    = (uint8_t)(config >> 8);

    if (mode == ADC_SINGLE_ENDED_CODE) {
        // Single-ended conversion
        start_adc_single_ended_conversion(channel);
        return;
    }

    if (mode == ADC_DIFF_CODE) {
        // Differential conversion: decode configured pair
        uint8_t p_channel = 0u;
        uint8_t n_channel = 0u;
        uint8_t pair_ok   = 0u;

        switch (channel) {
            case PA4_PA5:
                n_channel = ADC_MUXNEG_AIN4_gc;
                p_channel = ADC_MUXPOS_AIN5_gc;
                pair_ok = 1;
                break;
            case PA6_PA7:
                n_channel = ADC_MUXNEG_AIN6_gc;
                p_channel = ADC_MUXPOS_AIN7_gc;
                pair_ok = 1;
                break;
            case ACREF_PA4:
                n_channel = ADC_MUXNEG_DACREF0_gc;
                p_channel = ADC_MUXPOS_AIN4_gc;
                pair_ok = 1;
                break;
            case ACREF_PA5:
                n_channel = ADC_MUXNEG_DACREF0_gc;
                p_channel = ADC_MUXPOS_AIN5_gc;
                pair_ok = 1;
                break;
            case ACREF_PA6:
                n_channel = ADC_MUXNEG_DACREF0_gc;
                p_channel = ADC_MUXPOS_AIN6_gc;
                pair_ok = 1;
                break;
            case ACREF_PA7:
                n_channel = ADC_MUXNEG_DACREF0_gc;
                p_channel = ADC_MUXPOS_AIN7_gc;
                pair_ok = 1;
                break;
            case ACREF_PB0:
                n_channel = ADC_MUXNEG_DACREF0_gc;
                p_channel = ADC_MUXPOS_AIN11_gc;
                pair_ok = 1;
                break;
            case ACREF_PB1:
                n_channel = ADC_MUXNEG_DACREF0_gc;
                p_channel = ADC_MUXPOS_AIN10_gc;
                pair_ok = 1;
                break;
            default:
                // Unknown differential pair; ignore safely
                break;
        }
        if (pair_ok) {
            start_adc_differential_conversion(p_channel, n_channel);
        }
        return;
    }
}

#ifdef INPUT_REGISTERS
void adc_sequencer(void){
    static uint8_t index_input_register = 0;
    static uint8_t initial_start_adc = 0;
    if (g_registers.numOfInputRegisters > 0u) {
        // Kick the first conversion once
        if (!initial_start_adc) {
            initial_start_adc = 1u;
            // Clear any stale flag before starting
            ADC0.INTFLAGS = ADC_RESRDY_bm; // write-1-to-clear
            start_adc(g_registers.config_input_registers[index_input_register]);
        }

        // Wait for a completed conversion and then service it
        if ((ADC0.INTFLAGS & ADC_RESRDY_bm) != 0u) {
            // Latch result atomically and clear RESRDY
            cli();
            g_registers.input_registers[index_input_register] = ADC0.RESULT;
            sei();
            
            #ifdef DEBUG_ADC_READINGS
            Serial1.print("Reading ADC ");
            Serial1.print(index_input_register, HEX);
            Serial1.print(":");
            Serial1.print(ADC0.MUXPOS, HEX);
            Serial1.print(":");
            Serial1.print((uint8_t)(config_input_registers[index_input_register] & 0xFFu), HEX);
            Serial1.print("->");
            Serial1.println(input_registers[index_input_register] );
            #endif
            
            // Advance channel index
            if (++index_input_register >= g_registers.numOfInputRegisters) {
                index_input_register = 0u;
            }

            // Start next conversion:
            ADC0.INTFLAGS = ADC_RESRDY_bm; // ensure clean flag before new start
            start_adc(g_registers.config_input_registers[index_input_register]);
        }
    }
}   
#endif
