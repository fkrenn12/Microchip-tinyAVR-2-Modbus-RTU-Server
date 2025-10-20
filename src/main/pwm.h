#ifndef PWM_H
#define PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Public API declarations go here.
void pwm_set_frequency(uint16_t frequency);
void pwm_set_duty(uint8_t channel, uint8_t percent);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // PWM_H
