#include "pwm.h"
static uint8_t init_done = 0;
static uint16_t pwm_frequency = 0;
static uint8_t pwm_channel_WO0_initialized, pwm_channel_WO1_initialized, pwm_channel_WO2_initialized = 0;
static uint16_t percent_WO0, percent_WO1, percent_WO2  = 0;

/*
static uint8_t tca_single_divider_from_freq_single_slope(uint16_t freqHz) {
  if (freqHz >= 40001 && freqHz <= 65535) {
    return TCA_SINGLE_CLKSEL_DIV2_gc;
  } else if (freqHz >= 20001 && freqHz <= 40000) {
    return TCA_SINGLE_CLKSEL_DIV4_gc;
  } else if (freqHz >= 10001 && freqHz <= 20000) {
    return TCA_SINGLE_CLKSEL_DIV8_gc;
  } else if (freqHz >= 2001 && freqHz <= 10000) {
    return TCA_SINGLE_CLKSEL_DIV16_gc;
  } else if (freqHz >= 501 && freqHz <= 2000) {
    return TCA_SINGLE_CLKSEL_DIV64_gc;
  } else if (freqHz >= 101 && freqHz <= 500) {
    return TCA_SINGLE_CLKSEL_DIV256_gc;
  } else if (freqHz >= 1 && freqHz <= 100) {
    return TCA_SINGLE_CLKSEL_DIV1024_gc;
  }
  return 0xFF; // out of supported range
}
*/
static uint8_t tca_single_divider_from_freq_dual_slope(uint16_t freqHz) {
  if (freqHz >= 40001 && freqHz <= 65535) {
    return TCA_SINGLE_CLKSEL_DIV1_gc;
  } else if (freqHz >= 20001 && freqHz <= 40000) {
    return TCA_SINGLE_CLKSEL_DIV2_gc;
  } else if (freqHz >= 10001 && freqHz <= 20000) {
    return TCA_SINGLE_CLKSEL_DIV4_gc;
  } else if (freqHz >= 2001 && freqHz <= 10000) {
    return TCA_SINGLE_CLKSEL_DIV8_gc;
  } else if (freqHz >= 501 && freqHz <= 2000) {
    return TCA_SINGLE_CLKSEL_DIV16_gc;
  } else if (freqHz >= 101 && freqHz <= 500) {
    return TCA_SINGLE_CLKSEL_DIV256_gc;
  } else if (freqHz >= 1 && freqHz <= 100) {
    return TCA_SINGLE_CLKSEL_DIV1024_gc;
  }
  return 0xFF; // out of supported range
}
// Computes PER for the given freq and chosen CLKSEL; clamps to 16-bit and >=99 for >=1% duty.
// Returns 1 on success, 0 on failure (out of range).
uint16_t tca_compute_periode_for_freq(uint16_t freqHz, uint8_t divider) {
  uint16_t presc;
  if (freqHz < 1) return 0;
  switch (divider) {
    case TCA_SINGLE_CLKSEL_DIV1_gc:    presc = 1;    break;
    case TCA_SINGLE_CLKSEL_DIV2_gc:    presc = 2;    break;
    case TCA_SINGLE_CLKSEL_DIV4_gc:    presc = 4;    break;
    case TCA_SINGLE_CLKSEL_DIV8_gc:    presc = 8;    break;
    case TCA_SINGLE_CLKSEL_DIV16_gc:   presc = 16;   break;
    case TCA_SINGLE_CLKSEL_DIV64_gc:   presc = 64;   break;
    case TCA_SINGLE_CLKSEL_DIV256_gc:  presc = 256;  break;
    case TCA_SINGLE_CLKSEL_DIV1024_gc: presc = 1024; break;
    default: return 0;
  }

  // PER = round(F_CPU / (presc * f)) - 1
  // Use 64-bit math to avoid overflow, then clamp to 16-bit and >=99.
  uint32_t den = (uint32_t)presc * freqHz * 2; // *2 for dual-slope PWM
  uint32_t periodeRounded = ((uint64_t)F_CPU + den / 2) / den; // ≈ round(F_CPU / (presc * f))
  if (periodeRounded == 0) return 0;
  uint32_t periode = periodeRounded  - 1;
  if (periode < 99) periode = 99;           // keep ≥1% duty resolution
  if (periode > 65535) periode = 65535;     // 16-bit limit
  return (uint16_t)periode;
}

uint16_t duty_from_percent(uint8_t percent) {
  if (percent > 100) percent = 100;
  // Map 0..100% to 0..PER with rounding
  return (uint16_t)((((uint32_t)percent) * ((uint16_t)TCA0.SINGLE.PER + 1) + 50) / 100);
}

// One-time initialization: TCA0 single mode PWM on WO0..WO2
void tca0_init_single(uint32_t frequency) {
  // Stop TCA and select single mode defaults
  TCA0.SINGLE.CTRLA = 0;
  TCA0.SINGLE.CTRLB = 0;

  // single-slope PWM
  // TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
  TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_DSBOTH_gc;
  TCA0.SINGLE.CTRLD = 0;
  TCA0.SINGLE.CTRLC = 0;

  // Safe default period and duty before enabling the clock
  TCA0.SINGLE.PER = 1999;  // placeholder
  TCA0.SINGLE.CMP0 = 0;
  TCA0.SINGLE.CMP1 = 0;
  TCA0.SINGLE.CMP2 = 0;

  uint8_t divider= tca_single_divider_from_freq_dual_slope(frequency);
  if (divider != 0xFF) {
    uint16_t periode = tca_compute_periode_for_freq(frequency, divider);
    if (periode){
        TCA0.SINGLE.CTRLA = 0;
        TCA0.SINGLE.PER = periode;
        TCA0.SINGLE.CTRLA = divider | TCA_SINGLE_ENABLE_bm;
    }
  }
}

void pwm_set_frequency(uint16_t frequency){
    
    if (!init_done){
        tca0_init_single(frequency);
        pwm_frequency = frequency;
        init_done = 1;   
    }
    else{
        if (pwm_frequency != frequency){
            uint8_t divider= tca_single_divider_from_freq_dual_slope(frequency);
            if (divider != 0xFF) {
              uint16_t periode = tca_compute_periode_for_freq(frequency, divider);
              if (periode){
                  TCA0.SINGLE.PER = periode;
                  TCA0.SINGLE.CTRLA = divider | TCA_SINGLE_ENABLE_bm;
              }
              // recalculate duty cycle
              TCA0.SINGLE.CMP0 = duty_from_percent(percent_WO0);
              TCA0.SINGLE.CMP1 = duty_from_percent(percent_WO1);
              TCA0.SINGLE.CMP2 = duty_from_percent(percent_WO2);
            }
            pwm_frequency = frequency;
        }
    }
}

void pwm_set_duty(uint8_t channel, uint8_t percent)
{
    if (!init_done) pwm_set_frequency(DEFAULT_PWM_FREQUENCY);
    if (percent>100) percent=100;
    if (channel==((HOLDING_PWM_PB0_DUTY) & 0xFF)){
        if (!pwm_channel_WO0_initialized ){
            pwm_channel_WO0_initialized  = 1;
            TCA0.SINGLE.CMP0 = duty_from_percent(percent);
            percent_WO0 = percent;
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP0EN_bm;
            PORTB.DIRSET = PIN0_bm;
        }
        else{
            TCA0.SINGLE.CMP0 = duty_from_percent(percent);
            percent_WO0 = percent;
        }
    }
    else if (channel==((HOLDING_PWM_PB1_DUTY) & 0xFF)){
        if (!pwm_channel_WO1_initialized ){
            pwm_channel_WO1_initialized = 1;
            TCA0.SINGLE.CMP1 = duty_from_percent(percent);
            percent_WO1 = percent;
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP1EN_bm;
            PORTB.DIRSET = PIN1_bm;
        }
        else{
            TCA0.SINGLE.CMP1 = duty_from_percent(percent);
            percent_WO1 = percent;
        }
    }
    else if (channel==((HOLDING_PWM_PB2_DUTY) & 0xFF)){
        if (!pwm_channel_WO2_initialized ){
            pwm_channel_WO2_initialized = 1;
            TCA0.SINGLE.CMP2 = duty_from_percent(percent);
            percent_WO2 = percent;
            TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm;
            PORTB.DIRSET = PIN2_bm;
        }
        else{
            TCA0.SINGLE.CMP2 = duty_from_percent(percent);
            percent_WO2 = percent;
        }
    }
}

