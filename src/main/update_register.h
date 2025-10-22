#pragma once
#include <avr/io.h>
#include "inttypes.h"
#include "defines.h"
#include "config.h"
#include "register.h"

void update_holding_registers(void);
void update_input_registers(void);
void update_coils(void);
void update_discrete_inputs(void);
void init_update_callbacks(void);
